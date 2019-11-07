#include <string>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <iostream>
using namespace std;

#include <boost/dll.hpp>

#include "SQLiteDB.h"
using namespace SQLite;

#include "Data.h"

Data::Source::Source(const boost::filesystem::path& path)
{
    m_db.Open(path.string(), true);
}

Data::Bytes Data::Source::GetBlob(const int id)
{
    auto q = m_db.ExecQuery("SELECT Data FROM Data WHERE Id = '%1%' LIMIT 1", id);
    if (!q.IsEOF())
    {
        return q.GetBlobField(0);
    }
    else
    {
        // todo: throw error?
        return {};
    }
}

std::string Data::Source::GetName(const int id)
{
    auto q = m_db.ExecQuery("SELECT Name FROM Data WHERE Id = '%1%' LIMIT 1", id);
    if (!q.IsEOF())
    {
        return q.GetStringField(0);
    }
    else
    {
        // todo: throw error?
        return {};
    }
}

Data::Bytes Data::Slot::GetFont(std::string name) const
{
    Data::lowercase(name);
    return m_blobs.find<0,1>(name,1)->get<2>()->Get();
}

std::pair<Data::Bytes, Data::Bytes> Data::Slot::GetShader(std::string name) const
{
    Data::lowercase(name);
    return std::make_pair(
        m_blobs.find<0, 1>(name, 2)->get<2>()->Get(),
        m_blobs.find<0, 1>(name, 3)->get<2>()->Get());
}

const Data::Slot& Data::Slot::operator[] (std::string key) const
{
    Data::lowercase(key);
    auto iter = m_slots.find(key);
    if (iter == m_slots.end())
    {
        // todo: throw error
        return *this;
    }
    return iter->second;
}

Data::Slot& Data::Slot::Add(std::string name)
{
    Data::lowercase(name);
    return m_slots[name];
}

// todo make this a reference after grid.emplace returns the inserted item
std::shared_ptr<Data::Blob> Data::Slot::Add(std::string name, const int type, Source& source, const int id)
{
    Data::lowercase(name);
    auto blob = std::make_shared<Blob>(source, id);
    m_blobs.emplace(name, type, blob);
    return blob;
}

void Data::AddBlobsFromSource(Slot& slot, Source& source, const int slotId)
{
    auto& db = source.DB();
    auto q = db.ExecQuery("SELECT Id,Name FROM Slot WHERE Slot = '%1%'", slotId);
    for (; !q.IsEOF(); q.NextRow())
    {
        int id = q.GetIntField(0);
        std::string name = q.GetStringField(1);
        AddBlobsFromSource(slot.Add(name), source, id);
    }
    q = db.ExecQuery("SELECT Id,Type,Name FROM Data WHERE Slot = '%1%'", slotId);
    for (; !q.IsEOF(); q.NextRow())
    {
        int id = q.GetIntField(0);
        int type = q.GetIntField(1);
        std::string name = q.GetStringField(2);
        Data::lowercase(name);
        m_blobs.emplace(name, type, slot.Add(name, type, source, id));
    }
}

void Data::AddDataPath(const boost::filesystem::path& datapath)
{
    static auto exe_path = boost::dll::program_location().parent_path();
    const auto full_path = boost::filesystem::canonical(datapath.is_relative() ? exe_path / datapath : datapath);
    auto res = m_sources.try_emplace(full_path, full_path);
    if (res.second)
    {
        AddBlobsFromSource(m_root, res.first->second);
    }
}

Data::~Data()
{}

void Data::lowercase(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}

