#pragma once

#include <tuple>
#include <array>
#include <unordered_set>
#include <utility>
#include <cmath>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <cassert>
#include <vector>
#include <unordered_set>

template<std::size_t N, typename ROW, std::size_t... I>
class grid_tuple
{
public:
  using next = grid_tuple<N - 1, ROW, N - 1, I...>;
  using type = typename next::type;
  template <int INDEX>
  using grid_element = typename next::template grid_element<INDEX>;

  static void clear(type& t)
  {
    std::get<N - 1>(t).clear();
    next::clear(t);
  }
  static auto insert(type& t, ROW* row)
  {
    next::insert(t, row);
    return std::get<N - 1>(t).emplace(row);
  }
  static void erase(type& t, ROW const* const row)
  {
    std::get<N - 1>(t).erase(grid_element<N - 1>(row, true));
    next::erase(t, row);
  }
  static void to_stream(std::ostream& s, const ROW* row)
  {
    next::to_stream(s, row);
    s << ((N > 1) ? "," : "") << std::get<N - 1>(*row);
  }
};
template<typename ROW, std::size_t... I>
class grid_tuple<0, ROW, I...>
{
public:
  template <int INDEX>
  class grid_element
  {
  public:
    using type = typename std::tuple_element<INDEX, ROW>::type;
    enum class Usage
    {
      Row,
      Key,
      Ptr
    };
    grid_element(ROW const* const row, const bool useRawPtr = false)
      : m_usage(useRawPtr ? Usage::Ptr : Usage::Row)
      , m_row(row)
    {}
    grid_element(const type& key)
      : m_usage(Usage::Key)
      , m_key(&key)
    {}
    ~grid_element()
    {
      if (INDEX == 0 && m_usage == Usage::Row)
      {
        delete m_row;
      }
    }
    bool operator == (const grid_element<INDEX>& other) const
    {
      if (m_usage == Usage::Ptr || other.m_usage == Usage::Ptr)
      {
        assert(m_usage != Usage::Key && other.m_usage != Usage::Key);
        return m_row == other.m_row;
      }
      return **this == *other;
    }
    const type& operator * () const
    {
      return m_usage == Usage::Key ? *m_key : std::get<INDEX>(*m_row);
    }
    auto& operator * ()
    {
      return m_usage == Usage::Key ? *m_key : std::get<INDEX>(*m_row);
    }
    ROW const* const row() const
    {
      assert(m_usage != Usage::Key);
      return m_row;
    }
    template<int INDEX>
    const auto& get() const
    {
      assert(m_usage != Usage::Key);
      return std::get<INDEX>(*m_row);
    }
  private:
    Usage m_usage;
    union
    {
      ROW const* const m_row;
      type const* const m_key;
    };
  };

  class grid_hash
  {
  public:
    template<typename ELEMENT>
    auto operator() (const ELEMENT& element) const
    {
      return std::hash<typename ELEMENT::type>()(*element);
    }
  };

  class grid_equal
  {
  public:
    template<typename T0, typename T1>
    bool operator() (const T0& t0, const T1& t1) const
    {
      return t0 == t1;
    }
  };

  using type = typename std::tuple<std::unordered_multiset<grid_element<I>, grid_hash, grid_equal>...>;

  static void clear(type&)
  {}
  static void insert(type&, ROW*)
  {}
  static void erase(type&, ROW const* const)
  {}
  static void to_stream(std::ostream&, const ROW*)
  {}
};

template <typename ... TYPES>
class grid_applicator
{
public:
  const static size_t column_count = sizeof...(TYPES);
  using row_type = std::tuple<TYPES...>;
  using grid_type = typename grid_tuple<sizeof...(TYPES), row_type>;
  using data_type = typename grid_type::type;
  template<int INDEX>
  using grid_element = typename grid_type::template grid_element<INDEX>;

  static auto insert(data_type& data, row_type* row)
  {
    grid_type::insert(data, row);
  }

  static void erase(data_type& data, row_type const* const row)
  {
    grid_type::erase(data, row);
  }

  static void clear(data_type& data)
  {
    grid_type::clear(data);
  }

  static void to_stream(std::ostream& s, const row_type* row)
  {
    grid_type::to_stream(s, row);
  }
};

template <typename TYPE, typename ... TYPES>
class grid
{
public:
  const static size_t column_count = sizeof...(TYPES) + 1;
  using this_type = grid<TYPE, TYPES...>;
  using applicator = grid_applicator<TYPE, TYPES...>;
  using row_type = typename applicator::row_type;
  using data_type = typename applicator::data_type;
  using grid_type = typename applicator::grid_type;
  template<int INDEX>
  using grid_element = typename applicator::template grid_element<INDEX>;

public:
  ~grid()
  {
    clear();
  }

  grid()
    : m_data()
  {}

  grid(const this_type& other)
    : grid()
  {
    for (const auto& element : other)
    {
      auto p = new row_type(*element.row());
      applicator::insert(m_data, p);
    }
  }

  grid(this_type&& other)
    : grid()
  {
    m_data.swap(other.m_data);
  }

  grid(const std::initializer_list<row_type>& values)
    : grid()
  {
    for (const auto& value : values)
    {
      auto p = new row_type(value);
      applicator::insert(m_data, p);
    }
  }

  template<int INDEX = 0>
  auto begin() const
  {
    return std::get<INDEX>(m_data).begin();
  }
  template<int INDEX = 0>
  auto end() const
  {
    return std::get<INDEX>(m_data).end();
  }

  size_t size()
  {
    return std::get<0>(m_data).size();
  }

  void clear()
  {
    applicator::clear(m_data);
  }

  void emplace(TYPE t0, TYPES ... ts)
  {
    auto p = new row_type(t0, ts...);
    applicator::insert(m_data, p);
  }

  // get the value for a column by searching on another column
  template< size_t I, size_t J>
  const auto& get(const typename std::tuple_element<I, row_type>::type& key) const
  {
    return find<I>(key)->get<J>();
  }

  // find the first row with a value in a column (iterator to grid element)
  template<int INDEX = 0>
  auto find(const typename std::tuple_element<INDEX, row_type>::type& key) const
  {
    return std::get<INDEX>(m_data).find(grid_element<INDEX>(key));
  }

  // find all rows with a value in a column (iterator to begin/end grid element)
  template<int INDEX = 0>
  auto find_all(const typename std::tuple_element<INDEX, row_type>::type& key) const
  {
    return std::get<INDEX>(m_data).equal_range(grid_element<INDEX>(key));
  }

  // erase the first matching value and return true if a value was removed
  template<int INDEX = 0>
  bool erase(const typename std::tuple_element<INDEX, row_type>::type& key)
  {
    auto iter = find<INDEX>(key);
    return erase(iter);
  }
  template<int INDEX = 0>
  bool erase(const typename std::tuple_element<INDEX, data_type>::type::iterator& iter)
  {
    if (iter != end<INDEX>())
    {
      applicator::erase(m_data, iter->row());
      return true;
    }
    return false;
  }

  // erase all matching values and return number of removed rows
  template<int INDEX = 0>
  int erase_all(const typename std::tuple_element<INDEX, row_type>::type& key)
  {
    auto iters = find_all<INDEX>(key);
    std::vector<row_type const*> rows;
    for (auto iter = iters.first; iter != iters.second; ++iter)
    {
      rows.push_back(iter->row());
    }
    for (auto row : rows)
    {
      applicator::erase(m_data, row);
    }
    return rows.size();
  }

  std::ostream& to_stream(std::ostream& s) const
  {
    s << "{";
    char* open = "{";
    for (const auto& row : std::get<0>(m_data))
    {
      s << open;
      applicator::to_stream(s, row.row());
      s << "}";
      open = ",{";
    }
    s << "}";
    return s;
  }

private:
  data_type m_data;
};

template<typename ... TYPES>
std::ostream& operator << (std::ostream& s, const grid<TYPES...>& g)
{
  return g.to_stream(s);
}

