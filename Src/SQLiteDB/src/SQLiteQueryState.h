#pragma once

class Query::State
{
public:
    State(sqlite3* db, sqlite3_stmt* statement, bool eof,bool owner = true,int columnCount = -1)
        : m_db(db)
        , m_statement(statement)
        , m_eof(eof)
        , m_owner(owner)
        , m_columnCount(columnCount)
    {
        if (-1 == m_columnCount)
        {
            m_columnCount = sqlite3_column_count(m_statement);
        }
    }
    ~State()
    {
        Finalize();
    }

    void Check()
    {
        if (nullptr == m_statement)
        {
            ThrowError("Statement not active");
        }
    }
    void CheckField(int field)
    {
        if (field < 0 || field > m_columnCount - 1)
        {
            ThrowError("Invalid field index requested");
        }
    }

    void Finalize()
    {
        if (m_statement && m_owner)
        {
            int ret = sqlite3_finalize(m_statement);
            ThrowErrorIfNotOK(m_db, ret);
        }
        m_statement = nullptr;
    }

    sqlite3_stmt* m_statement;
    sqlite3* m_db;
    bool m_eof;
    bool m_owner;
    int m_columnCount;
    std::vector<std::string> m_fieldNames;
};


