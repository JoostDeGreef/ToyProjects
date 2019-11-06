#pragma once

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/dll.hpp>

#include "SQLiteDB.h"
#include "grid.h"

class Data
{
public:
    using Bytes = std::vector<unsigned char>;

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
        Blob(const std::shared_ptr<Source>& source, int id)
            : m_source(source)
            , m_id(id)
        {}
        Bytes Get() const { return m_source->GetBlob(m_id); }
    private:
        std::shared_ptr<Source> m_source;
        int m_id;
    };

    using Blobs = grid<std::string, int, std::shared_ptr<Blob>>;

    class Slot
    {
    public:
        using Slots = std::map<std::string, Slot>;

        const Slot& operator[] (const std::string& key) const;

        //iterator over files
        auto begin() const { return m_blobs.begin(); }
        auto end() const { return m_blobs.end(); }

        Slot& Add(const std::string& name);
        Blob& Add(const std::string& name, const int type, const std::shared_ptr<Source>& source, const int id);
    private:
        Slots m_slots;
        Blobs m_blobs;
    };

    template<typename ...Paths>
    Data(Paths& ...datapaths)
    {
        std::vector<boost::filesystem::path> datapaths_vector{ datapaths... };
        std::set<boost::filesystem::path> datapaths_set;
        auto exe_path = boost::dll::program_location().parent_path();
        for (const auto & path : datapaths_vector)
        {
          const auto full_path = boost::filesystem::canonical(path.is_relative() ? exe_path / path : path);
          datapaths_set.insert(full_path);
        }
        for (const auto& datapath : datapaths_set)
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
    Blobs m_blobs;
};

