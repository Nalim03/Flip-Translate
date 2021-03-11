#ifndef SQL_PERSISTENCE
#define SQL_PERSISTENCE

#include <QtGlobal>
#include <QLocale>
#include <QString>
#include <optional>
#include <functional>
#include <SQLiteCpp/SQLiteCpp.h>

#include "iflip_persistence.h"

using RowId = long long;
using OptionalRowId = std::optional<RowId>;

using LanguageWordPair = QPair<QLocale::Language, QString>;

struct CardRecord
{
    RowId id;
    RowId deckId;
    RowId sourceWordId;
    RowId targetWordId;
};

struct Deck
{
    RowId id;
    QString name;
    QVector<CardRecord> cards;
};

enum class AttemptResult : qint8
{
    CouldNotRecall,
    Easy,
    Moderate,
    Difficult
};

struct AttemptRecord
{
    RowId id;
    qint64 time;
    RowId cardId;
    AttemptResult result;
};

class SQLPersistence : public IFlipPersistence
{
public:
    SQLPersistence(const char* dbFilename);

    const QMap<RowId, Deck>& getDecks() const;
    const QVector<CardRecord>& getCardsOfDeck(RowId deckId) const;
    OptionalRowId getDeckIdByName(const QString& name) const;

    void fetchWord(RowId wordId, LanguageWordPair& result);
    void fetchSynonyms(RowId wordId, QVector<LanguageWordPair>& synonyms);

    RowId insertWord(QLocale::Language, const QString& word);
    RowId insertSynonym(RowId wordId, RowId synonymWordId);
    RowId insertCard(RowId deckId, RowId sourceWordId, RowId targetWordId);
    RowId insertDeck(const QString& name);
    RowId insertAttempt(qint64 time, RowId cardId, AttemptResult result);

    void forEachCardInDeck(RowId deckId, std::function<void(const CardRecord&)> callback) const;
    void forEachDeck(std::function<void(const Deck&)> callback) const;

private:
    SQLite::Database db;
    QMap<RowId, Deck> decks;
};

#endif
