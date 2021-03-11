#include "sql_persistence.h"

SQLPersistence::SQLPersistence(const char* dbFilename)
    : db(dbFilename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
    db.exec("CREATE TABLE IF NOT EXISTS words (id INTEGER PRIMARY KEY, language_id INTEGER, word TEXT)");
    db.exec("CREATE TABLE IF NOT EXISTS synonyms (id INTEGER PRIMARY KEY, word_id INTEGER, synonym TEXT)");

    db.exec("CREATE TABLE IF NOT EXISTS decks (id INTEGER PRIMARY KEY, name TEXT)");
    db.exec("CREATE TABLE IF NOT EXISTS cards (id INTEGER PRIMARY KEY, deck_id INTEGER, source_word_id INTEGER, target_word_id INTEGER)");

    db.exec("CREATE TABLE IF NOT EXISTS attempts (id INTEGER PRIMARY KEY, time INTEGER, card_id INTEGER, result INTEGER)");
}
