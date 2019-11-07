#include "gtest/gtest.h"

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

using namespace std;
using namespace testing;

class IOStreamsTest : public Test
{
protected:

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

std::string string_compress(const std::string& data)
{
    std::stringstream compressed;
    std::stringstream original;
    original << data;
    boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
    out.push(boost::iostreams::zlib_compressor());
    out.push(original);
    boost::iostreams::copy(out, compressed);
    return compressed.str();
}

std::string string_decompress(const std::string& data)
{
    std::stringstream compressed;
    std::stringstream decompressed;
    compressed << data;
    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::zlib_decompressor());
    in.push(compressed);
    boost::iostreams::copy(in, decompressed);
    return decompressed.str();
}

TEST_F(IOStreamsTest, BasicTest)
{
    std::string input = "This is a small test string. This is a small test string. This is a small test string.";
    std::string compressed = string_compress(input);
    std::string output = string_decompress(compressed);
    EXPECT_EQ(input, output);
    EXPECT_LE(compressed.size(), input.size());
}

