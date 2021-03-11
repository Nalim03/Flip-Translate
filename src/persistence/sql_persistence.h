#ifndef SQL_PERSISTENCE
#define SQL_PERSISTENCE

#include "iflip_persistence.h"

#include <SQLiteCpp/SQLiteCpp.h>

class SQLPersistence : public IFlipPersistence
{
public:
    SQLPersistence(const char* dbFilename);

private:
    SQLite::Database db;
};

#endif
