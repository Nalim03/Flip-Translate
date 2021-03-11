#include "sql_persistence.h"

SQLPersistence::SQLPersistence(const char* dbFilename)
    : db(dbFilename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
    db.exec("CREATE TABLE IF NOT EXISTS words (id INTEGER PRIMARY KEY, language_id INTEGER, word TEXT)");
    db.exec("CREATE TABLE IF NOT EXISTS synonyms (id INTEGER PRIMARY KEY, word_id INTEGER, synonym_word_id INTEGER)");

    db.exec("CREATE TABLE IF NOT EXISTS decks (id INTEGER PRIMARY KEY, name TEXT)");
    db.exec("CREATE TABLE IF NOT EXISTS cards (id INTEGER PRIMARY KEY, deck_id INTEGER, source_word_id INTEGER, target_word_id INTEGER)");

    db.exec("CREATE TABLE IF NOT EXISTS attempts (id INTEGER PRIMARY KEY, time UNSIGNED BIG INT, card_id INTEGER, result INT8)");

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

RowId SQLPersistence::insertDeck(const QString& name)
{
    QByteArray nameUtf8 = name.toUtf8();
    SQLite::Statement statement(db, "INSERT INTO decks (name) VALUES (?)");
    statement.bind(1, nameUtf8.constData());
    statement.exec();

    auto id = db.getLastInsertRowid();
    decks[id] = { id, name, {} };
    return id;
}
