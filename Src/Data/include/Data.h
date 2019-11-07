#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

#include "SQLiteDB.h"
#include "grid.h"

class Data
{
protected:
    static void lowercase(std::string& str);
public:
    using Bytes = std::vector<unsigned char>;

    class Source
    {
    public:
        Source(const boost::filesystem::path& path);
        Bytes GetBlob(const int id);
        std::string GetName(const int id);
        int GetType(const int id);
        auto& DB() { return m_db; }
    private:
        SQLite::DB m_db;
    };

    using Sources = std::map<boost::filesystem::path, Source>;

    class Blob
    {
    public:
        Blob(Source& source, int id)
            : m_source(source)
            , m_id(id)
        {}
        Bytes Get() const { return m_source.GetBlob(m_id); }
        std::string Name() const { return m_source.GetName(m_id); }
        int Type() const { return m_source.GetType(m_id); }
    private:
        Source& m_source;
        int m_id;
    };

    using Blobs = grid<std::string, int, std::shared_ptr<Blob>>;

    class Slot
    {
    public:
        using Slots = std::map<std::string, Slot>;

        const Slot& operator[] (std::string key) const;

        // iterator over files
        auto begin() const { return m_blobs.begin(); }
        auto end() const { return m_blobs.end(); }

        Bytes GetFont(std::string name) const;
        std::pair<Bytes, Bytes> GetShader(std::string name) const;

        Slot& Add(std::string name);
        const std::shared_ptr<Blob>& Add(std::string name, const int type, Source& source, const int id);
    private:
        Slots m_slots;
        Blobs m_blobs;
    };

    Data(const std::initializer_list<boost::filesystem::path>& datapaths)
    {
        for (const auto& datapath : datapaths)
        {
            AddDataPath(datapath);
        }
    }

    ~Data();

    void AddDataPath(const boost::filesystem::path& datapath);

    const Slot& operator[] (std::string key)
    {
        Data::lowercase(key);
        return m_root[key];
    }
private:
    void AddBlobsFromSource(Slot& slot, Source& source, const int slotId = 0);

    Sources m_sources;
    Slot m_root;
    Blobs m_blobs;
};

