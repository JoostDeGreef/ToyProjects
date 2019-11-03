#pragma once

#include <vector>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

#include "SQLiteDB.h"
#include "grid.h"

class Data
{
public:
    typedef std::vector<unsigned char> Bytes;

    class Source
    {
    public:
        Source(const boost::filesystem::path& path);
        Bytes GetBlob(const int id);
        auto& DB() { return m_db; }
    private:
        SQLite::DB m_db;
    };

    class Blob
    {
    public:
        Blob(std::shared_ptr<Source>& source, int id)
            : m_source(std::move(source))
            , m_id(id)
        {}
        Bytes Get() const { return m_source->GetBlob(m_id); }
    private:
        std::shared_ptr<Source> m_source;
        int m_id;
    };

    class Slot
    {
    public:
        using Slots = std::map<std::string, Slot>;
        using Blobs = grid<std::string, int, std::shared_ptr<Blob>>;
        using iterator = Blobs::iterator;

        const Slot& operator[] (const std::string& key) const;

        //iterator over files
        iterator begin() const { return m_blobs.begin(); }
        iterator end() const { return m_blobs.end(); }

        Slot& Add(std::string& name);
        Blob& Add(std::string& name, const int type, std::shared_ptr<Source>& source, const int id);
    private:
        Slots m_slots;
        Blobs m_blobs;
    };

    template<typename ...Paths>
    Data(Paths& ...datapaths)
    {
        std::vector<boost::filesystem::path> dataPaths{ datapaths... };
        for (const auto& datapath : dataPaths)
        {
            AddDataPath(datapath);
        }
    }

    const Slot& operator[] (const std::string& key)
    {
        return m_root[key];
    }
private:
    void AddDataPath(const boost::filesystem::path& datapath);
    void AddBlobsFromSource(Slot & slot, std::shared_ptr<Source>& source, const int slotId = 0);

    Slot m_root;
    std::multimap<std::string, std::tuple<std::string, Blob*>> m_blobsByName;
    std::map<std::string, std::map<std::string, Blob*>> m_blobsByTypeByName;
};
