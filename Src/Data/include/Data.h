#pragma once

#include <vector>
#include <boost/filesystem.hpp>

class Data
{
public:
    template<typename ...Paths>
    Data(Paths& ...dataPaths)
        : m_dataPaths{dataPaths...}
    {}

private:
    std::vector<boost::filesystem::path> m_dataPaths;
};

