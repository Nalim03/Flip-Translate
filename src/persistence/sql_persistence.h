#ifndef SQL_PERSISTENCE
#define SQL_PERSISTENCE

#include <QtGlobal>
#include <QLocale>
#include <QString>
#include <SQLiteCpp/SQLiteCpp.h>

#include "iflip_persistence.h"

struct WordRecordData
{
    qint32 id;
    QLocale::Language language;
    QString word;
};

struct CardRecordData
{
    qint32 id;
    qint32 deckId;
    qint32 sourceWordId;
    qint32 targetWordId;
};

enum class AttemptResult : qint8
{
    CouldNotRecall,
    Easy,
    Moderate,
    Difficult
};

struct AttemptRecordData
{
    qint32 id;
    quint64 time;
    qint32 cardId;
    AttemptResult result;
};

class SQLPersistence : public IFlipPersistence
{
public:
    SQLPersistence(const char* dbFilename);

private:
    SQLite::Database db;
};

#endif
