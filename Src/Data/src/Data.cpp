#include <string>
#include <unordered_map>
#include <algorithm>
#include <memory>
using namespace std;

#include "SQLiteDB.h"
using namespace SQLite;

#include "Data.h"

//class DataCore
//{
//public:
//    DataCore()
//    {
//        m_db.Open(Filesystem::GetDataFilepath(), true);
//    }
//    std::vector<unsigned char> GetFont(const std::string& fontName)
//    {
//        auto q = m_db.ExecQuery("SELECT Data FROM Fonts WHERE Name = '%1%'", fontName);
//        return q.GetBlobField(0);
//    }
//    std::vector<std::string> GetFontNames()
//    {
//        std::vector<std::string> res;
//        auto q = m_db.ExecQuery("SELECT Name FROM Fonts;");
//        for (; !q.IsEOF(); q.NextRow())
//        {
//            res.emplace_back(q.GetStringField(0));
//        }
//        return res;
//    }
//private:
//    SQLite::DB m_db;
//};
//
//namespace
//{
//    DataCore& GetDataCore()
//    {
//        thread_local static DataCore dc;
//        return dc;
//    }
//}

Data::Source::Source(const boost::filesystem::path& path)
{
    m_db.Open(path.string(), true);
}

Data::Bytes Data::Source::GetBlob(const int id)
{
  auto q = m_db.ExecQuery("SELECT Data FROM Blobs WHERE Id = '%1%' LIMIT 1", id);
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

const Data::Slot& Data::Slot::operator[] (const std::string& key) const
{ 
    auto iter = m_slots.find(key);
    if (iter == m_slots.end())
    {
        // todo: throw error
        return *this;
    }
    return iter->second;
}

Data::Slot& Data::Slot::Add(std::string& name)
{
    return m_slots[name];
}

Data::Blob& Data::Slot::Add(std::string& name, const int type, std::shared_ptr<Source>& source, const int id)
{
  auto blob = std::make_shared<Blob>(source, id);
  m_blobs.emplace(name, type, blob);
  return *blob;
}

void Data::AddBlobsFromSource(Slot & slot, std::shared_ptr<Source>& source, const int slotId)
{
    auto& db = source->DB();
    auto q = db.ExecQuery("SELECT Id,Name FROM Slots WHERE Slot = '%1%'", slotId);
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
        Blob & blob = slot.Add(name,type,source,id);
    }

}

void Data::AddDataPath(const boost::filesystem::path& datapath)
{
    auto source = std::make_shared<Source>(datapath);
    AddBlobsFromSource(m_root,source);
}



