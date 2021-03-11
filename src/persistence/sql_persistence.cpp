#include "sql_persistence.h"
#include <sstream>

SQLPersistence::SQLPersistence(const char* dbFilename)
    : db(dbFilename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
    db.exec("CREATE TABLE IF NOT EXISTS words (id INTEGER PRIMARY KEY, language_id INTEGER, word TEXT)");
    db.exec("CREATE TABLE IF NOT EXISTS synonyms (id INTEGER PRIMARY KEY, word_id INTEGER, synonym_word_id INTEGER)");

    db.exec("CREATE TABLE IF NOT EXISTS decks (id INTEGER PRIMARY KEY, name TEXT)");
    db.exec("CREATE TABLE IF NOT EXISTS cards (id INTEGER PRIMARY KEY, deck_id INTEGER, source_word_id INTEGER, target_word_id INTEGER)");

    db.exec("CREATE TABLE IF NOT EXISTS attempts (id INTEGER PRIMARY KEY, time BIGINT, card_id INTEGER, result INT8)");

    SQLite::Statement fetchDecks(db, "SELECT * FROM decks");
    while(fetchDecks.executeStep())
    {
        RowId deckId = fetchDecks.getColumn(0);
        auto deckName = fetchDecks.getColumn(1).getString();
        decks[deckId] = { deckId, QString::fromUtf8(deckName.c_str(), deckName.size()), {} };
    }

    SQLite::Statement fetchCards(db, "SELECT * FROM cards");
    while(fetchCards.executeStep())
    {
        RowId cardId = fetchCards.getColumn(0);
        RowId deckId = fetchCards.getColumn(1);
        RowId sourceWordId = fetchCards.getColumn(2);
        RowId targetWordId = fetchCards.getColumn(3);
        decks[deckId].cards.push_back({ cardId, deckId, sourceWordId, targetWordId });
    }
}

const QMap<RowId, Deck>& SQLPersistence::getDecks() const
{
    return decks;
}

OptionalRowId SQLPersistence::getDeckIdByName(const QString& name) const
{
    for(auto it = decks.constKeyValueBegin(); it != decks.constKeyValueEnd(); ++it)
    {
        if(it->second.name == name)
            return { it->first };
    }
    return { };
}

const QVector<CardRecord>& SQLPersistence::getCardsOfDeck(RowId deckId) const
{
    auto deck = decks.constFind(deckId);
    if(deck == decks.constEnd())
        throw std::runtime_error("Tried to retrieve cards of non-existing deck! (id " + std::to_string(deckId) + ")");

    return deck->cards;
}

void SQLPersistence::fetchWord(RowId wordId, LanguageWordPair& result)
{
    SQLite::Statement statement(db, "SELECT language_id, word FROM words WHERE id = ?");
    statement.bind(1, wordId);
    if(!statement.executeStep())
        throw std::runtime_error("Tried to fetch word #" + std::to_string(wordId) + " which is not present in database!");

    result.first = static_cast<QLocale::Language>(statement.getColumn(0).getInt());
    result.second = QString::fromUtf8(statement.getColumn(0).getString());
}

void SQLPersistence::fetchSynonyms(RowId wordId, QVector<LanguageWordPair>& result)
{
    SQLite::Statement statement(db, "SELECT synonym_word_id FROM synonyms WHERE word_id = ?");
    statement.bind(1, wordId);
    while(statement.executeStep())
    {
        LanguageWordPair synonym;
        fetchWord(statement.getColumn(0), synonym);
        result.push_back(synonym);
    }
}

RowId SQLPersistence::insertWord(QLocale::Language language, const QString& word)
{
    QByteArray wordUtf8 = word.toUtf8();

    SQLite::Statement selectStatement(db, "SELECT id FROM words WHERE language_id = ? AND word = ?");
    selectStatement.bind(1, language);
    selectStatement.bind(2, wordUtf8.constData());
    if(selectStatement.executeStep())
    {
        RowId wordId = selectStatement.getColumn(0);
        if(!selectStatement.executeStep())
            return wordId;

        RowId duplicateWordId = selectStatement.getColumn(0);
        throw std::runtime_error("Duplicate word in database: " + wordUtf8.toStdString() + " (" + std::to_string(wordId) + ", " + std::to_string(duplicateWordId) + ")");
    }

    SQLite::Statement insertStatement(db, "INSERT INTO words (language_id, word) VALUES (?, ?)");
    insertStatement.bind(1, language);
    insertStatement.bind(2, wordUtf8.constData());
    insertStatement.exec();
    return db.getLastInsertRowid();
}

RowId SQLPersistence::insertSynonym(RowId wordId, RowId synonymWordId)
{
    SQLite::Statement selectStatement(db, "SELECT id FROM synonyms WHERE word_id = ? AND synonym_word_id = ?");
    selectStatement.bind(1, wordId);
    selectStatement.bind(2, synonymWordId);
    if(selectStatement.executeStep())
    {
        RowId synonymId = selectStatement.getColumn(0);
        if(!selectStatement.executeStep())
            return synonymId;

        RowId duplicateSynonymId = selectStatement.getColumn(0);
        throw std::runtime_error("Duplicate synonym in database: " + std::to_string(synonymId) + ", " + std::to_string(duplicateSynonymId));
    }

    SQLite::Statement insertStatement(db, "INSERT INTO synonyms (word_id, synonym_word_id) VALUES (?, ?)");
    insertStatement.bind(1, wordId);
    insertStatement.bind(2, synonymWordId);
    insertStatement.exec();
    return db.getLastInsertRowid();
}

RowId SQLPersistence::insertCard(RowId deckId, RowId sourceWordId, RowId targetWordId)
{
    auto deck = decks.find(deckId);
    if(deck == decks.end())
        throw std::runtime_error("Tried to insert a card to a non-existing deck! (id " + std::to_string(deckId) + ")");

    for(const auto& card : deck->cards)
    {
        if(card.sourceWordId == sourceWordId && card.targetWordId == targetWordId)
        {
            std::ostringstream oss;
            oss << "Duplicate card in deck #" << deckId << " \"" << deck->name.toUtf8().toStdString() << "\"";
            oss << " (words: #" << sourceWordId << " => #" << targetWordId << ")";
            throw std::runtime_error(oss.str());
        }
    }

    SQLite::Statement statement(db, "INSERT INTO cards (deck_id, source_word_id, target_word_id) VALUES (?, ?, ?)");
    statement.bind(1, deckId);
    statement.bind(2, sourceWordId);
    statement.bind(3, targetWordId);
    statement.exec();

    RowId cardId = db.getLastInsertRowid();
    decks[deckId].cards.push_back({ cardId, deckId, sourceWordId, targetWordId });
    return cardId;
}

RowId SQLPersistence::insertDeck(const QString& name)
{
    QByteArray nameUtf8 = name.toUtf8();

    if(getDeckIdByName(name).has_value())
        throw std::runtime_error("Tried to insert deck \"" + nameUtf8.toStdString() + "\" which already exists!");

    SQLite::Statement statement(db, "INSERT INTO decks (name) VALUES (?)");
    statement.bind(1, nameUtf8.constData());
    statement.exec();

    auto id = db.getLastInsertRowid();
    decks[id] = { id, name, {} };
    return id;
}

RowId SQLPersistence::insertAttempt(qint64 time, RowId cardId, AttemptResult result)
{
    SQLite::Statement statement(db, "INSERT INTO attempts (time, card_id, result) VALUES (?, ?, ?)");
    statement.bind(1, time);
    statement.bind(2, cardId);
    statement.bind(3, static_cast<int>(result));
    statement.exec();
    return db.getLastInsertRowid();
}

void SQLPersistence::forEachCardInDeck(RowId deckId, std::function<void(const CardRecord&)> callback) const
{
    auto deck = decks.constFind(deckId);
    if(deck == decks.constEnd())
        throw std::runtime_error("Tried to iterate through cards of a non-existing deck! (id " + std::to_string(deckId) + ")");

    for(const auto& card : deck->cards)
        callback(card);
}

void SQLPersistence::forEachDeck(std::function<void(const Deck&)> callback) const
{
    for(const auto& deck : decks)
        callback(deck);
}
