#include <memory>
#include <string>
#include <vector>

using namespace std;

#include "boost/format.hpp"

#include "sqlite/sqlite3.h"
#include "SQLiteDB.h"

using namespace SQLite;

#include "SQLiteSupport.h"
#include "SQLiteDBState.h"
#include "SQLiteQueryState.h"
#include "SQLiteStatementState.h"

DB::DB()
    : m_state(std::make_shared<State>())
{}

DB::~DB()
{}

void DB::Open(const std::string& fileName, bool readOnly)
{
    int ret = sqlite3_open_v2(fileName.c_str(), &m_state->m_db, readOnly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, nullptr);
    ThrowErrorIfNotOK(m_state->m_db, ret);
    SetBusyTimeout(m_state->m_busyTimeoutMs);
}

void DB::Close()
{
    m_state->Close();
}

bool DB::TableExists(const std::string& tableName)
{
    return 0 < ExecSingleInt64("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='%1%'",tableName);
}

int DB::ExecDML(const std::string& sql)
{
    m_state->Check();

    char* szError = 0;

    int ret = sqlite3_exec(m_state->m_db, sql.c_str(), 0, 0, &szError);

    ThrowErrorIfNotOK(szError, ret);

    return sqlite3_changes(m_state->m_db);
}

Query DB::ExecQuery(const std::string& sql)
{
    m_state->Check();

    sqlite3_stmt* statement = m_state->Compile(sql);

    int ret = sqlite3_step(statement);

    if (ret == SQLITE_DONE)
    {
        // no rows
        return std::make_shared<Query::State>(m_state->m_db, statement, true);
    }
    else if (ret == SQLITE_ROW)
    {
        // at least 1 row
        return std::make_shared<Query::State>(m_state->m_db, statement, false);
    }
    else
    {
        ret = sqlite3_finalize(statement);
        ThrowError(m_state->m_db, ret);
        return nullptr;
    }
}

int64_t DB::ExecSingleInt64(const std::string& sql)
{
    Query q = ExecQuery(sql);

    if (q.IsEOF() || q.GetFieldCount() != 1)
    {
        ThrowError("Invalid single int64 query");
    }

    return q.GetInt64Field(0);
}

string DB::ExecSingleString(const std::string& sql)
{
    Query q = ExecQuery(sql);

    if (q.IsEOF() || q.GetFieldCount() != 1)
    {
        ThrowError("Invalid single string query");
    }

    return q.GetStringField(0);
}

Statement DB::CompileStatement(const std::string& sql)
{
    sqlite3_stmt* statement = m_state->Compile(sql);
    return std::make_shared<Statement::State>(m_state->m_db, statement, false);
}

int64_t DB::LastRowId() const
{
    return sqlite3_last_insert_rowid(m_state->m_db);
}

void DB::Interrupt()
{
    sqlite3_interrupt(m_state->m_db);
}

void DB::SetBusyTimeout(int milliSecs)
{
    m_state->m_busyTimeoutMs = milliSecs;
    sqlite3_busy_timeout(m_state->m_db, milliSecs);
}

std::string DB::SQLiteVersion()
{
    return SQLITE_VERSION;;
}

void DB::BeginTransaction()
{
    ExecDML("BEGIN TRANSACTION");
}

void DB::CommitTransaction()
{
    ExecDML("END TRANSACTION");
}
