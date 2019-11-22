#pragma once

template<typename ELEMENT,unsigned int ROWS = 0,unsigned int COLUMNS = 0>
class TMatrix : public TMatrixFunctions<ELEMENT, TMatrix<ELEMENT,ROWS,COLUMNS>>
{
public:
    using Element = ELEMENT;
    using ThisType = TMatrix<ELEMENT,ROWS,COLUMNS>;
    using Functions = TMatrixFunctions<Element, ThisType>;

protected:
    friend Functions;
    auto Instance() const { return TMatrix<ELEMENT, ROWS, COLUMNS>(); }
    auto InstanceDiag() const { return TMatrix<ELEMENT, std::min(COLUMNS, ROWS),1>(); }
    auto InstanceTransposed() const { return TMatrix<ELEMENT, COLUMNS, ROWS>(); }
    template<unsigned int R,unsigned int C>
    auto InstanceOuter(const TMatrix<ELEMENT,R,C>& m) const { return TMatrix<ELEMENT,ROWS,C>(); }
    auto InstanceOuter(const TMatrix<ELEMENT,0,0>& m) const { return TMatrix<ELEMENT,0,0>(ROWS, m.Columns()); }
    auto InstanceLogic() const { return TMatrix<bool, ROWS, COLUMNS>(); }
    auto InstanceMinor() const { return TMatrix<ELEMENT, (ROWS>0)?ROWS-1:0, (COLUMNS > 0) ? COLUMNS - 1 : 0>(); }
    // todo: if M is static, these should be static asserts
    template<typename M> void assert_inner(const M& m) const { assert(Columns()==m.Rows()); }
    template<typename M> void assert_outer(const M& m) const { assert(Rows()==m.Columns()); }
    template<typename M> void assert_size(const M& m) const { assert(Rows()==m.Rows() && Columns()==m.Columns()); }
                         void assert_square() const { assert(Rows()==Columns()); }

public:
    TMatrix() : TMatrix(Element()) {}
    TMatrix(const std::array<Element,ROWS*COLUMNS> & data)
        : m_data(data)
    {}
    TMatrix(const Element def)
    {
        m_data.fill(def);
    }
    TMatrix(const ThisType& other)
        : m_data(other.m_data)
    {}
    TMatrix(ThisType&& other)
        : m_data(other.m_data)
    {}

    ThisType& operator = (const ThisType& other)
    {
        m_data = other.m_data;
        return *this;
    }
    ThisType& operator = (ThisType&& other)
    {
        m_data = std::move(other.m_data);
        return *this;
    }

    inline unsigned int Columns() const { return COLUMNS; }
    inline unsigned int Rows() const { return ROWS; }
    inline unsigned int Elements() const { return ROWS * COLUMNS; }
    inline unsigned int Index(const unsigned int row, const unsigned int column) const { return row * COLUMNS + column; }

    inline Element& operator () (const unsigned int row, const unsigned int column) { return m_data[Index(row, column)]; }
    inline Element operator () (const unsigned int row, const unsigned int column) const { return m_data[Index(row, column)]; }
    inline Element& operator () (const unsigned int index) { return m_data[index]; }
    inline Element operator () (const unsigned int index) const { return m_data[index]; }

    const Element* GetData() const
    {
        return m_data.data();
    }
    Element* GetData()
    {
        return m_data.data();
    }

private:
    std::array<Element,ROWS*COLUMNS> m_data;
};

template<typename ELEMENT>
class TMatrix<ELEMENT,0,0> : public TMatrixFunctions<ELEMENT,TMatrix<ELEMENT,0,0>>
{
public:
    using Element = ELEMENT;
    using ThisType = TMatrix<Element,0,0>;
    using Functions = TMatrixFunctions<Element,ThisType>;

protected:
    friend Functions;
                         auto Instance() const { return ThisType(Rows(), Columns()); }
                         auto InstanceDiag() const { return ThisType(std::min(Columns(),Rows()),1); }
                         auto InstanceTransposed() const { return ThisType(Columns(), Rows()); }
    template<typename M> auto InstanceOuter(const M& m) const { return ThisType(Rows(), m.Columns()); }
                         auto InstanceLogic() const { return TMatrix<bool,0,0>(Rows(), Columns()); }
                         auto InstanceMinor() const { return ThisType(Rows()>0?Rows()-1:0,Columns()>0?Columns()-1:0); }
    template<typename M> void assert_inner(const M& m) const { assert(Columns()==m.Rows()); }
    template<typename M> void assert_outer(const M& m) const { assert(Rows()==m.Columns()); }
    template<typename M> void assert_size(const M& m) const { assert(Rows()==m.Rows() && Columns()==m.Columns()); }
                         void assert_square() const { assert(Rows()==Columns()); }
    
public:
    TMatrix() : TMatrix(0, 0) {}
    TMatrix(const unsigned int rows, const unsigned int columns, const std::initializer_list<Element> & elements)
        : m_data(elements)
        , m_columns(columns)
        , m_rows(rows)
    {
      assert(columns*rows==m_data.size());
    }
    TMatrix(const unsigned int rows, const unsigned int columns, const Element def)
        : m_data(columns* rows, def)
        , m_columns(columns)
        , m_rows(rows)
    {}
    TMatrix(const unsigned int rows, const unsigned int columns)
        : TMatrix(rows, columns, Element())
    {}
    TMatrix(const ThisType& other)
        : m_data(other.m_data)
        , m_columns(other.m_columns)
        , m_rows(other.m_rows)
    {}
    TMatrix(ThisType&& other)
        : m_data(other.m_data)
        , m_columns(other.m_columns)
        , m_rows(other.m_rows)
    {}

    ThisType & operator = (const ThisType& other)
    {
        m_data = other.m_data;
        m_columns = other.m_columns;
        m_rows = other.m_rows;
        return *this;
    }
    ThisType& operator = (ThisType&& other)
    {
        m_data = std::move(other.m_data);
        m_columns = std::move(other.m_columns);
        m_rows = std::move(other.m_rows);
        return *this;
    }

    inline unsigned int Columns() const { return m_columns; }
    inline unsigned int Rows() const { return m_rows; }
    inline unsigned int Elements() const { return m_rows * m_columns; }
    inline unsigned int Index(const unsigned int row, const unsigned int column) const { return row * m_columns + column; }

    inline typename std::vector<Element>::reference operator () (const unsigned int row, const unsigned int column) { return m_data[Index(row, column)]; }
    inline typename std::vector<Element>::const_reference operator () (const unsigned int row, const unsigned int column) const { return m_data[Index(row, column)]; }
    inline typename std::vector<Element>::reference operator () (const unsigned int index) { return m_data[index]; }
    inline typename std::vector<Element>::const_reference operator () (const unsigned int index) const { return m_data[index]; }

    const Element* GetData() const
    {
        return m_data.data();
    }
    Element* GetData()
    {
        return m_data.data();
    }

private:
    std::vector<Element> m_data;
    unsigned int m_columns;
    unsigned int m_rows;
};

// TODO:
// - extract TVectorFunctions CRTP
// - create row vector
template<typename ELEMENT, unsigned int ROWS>
class TMatrix<ELEMENT, ROWS, 1> : public TMatrixFunctions<ELEMENT, TMatrix<ELEMENT, ROWS, 1>>
{
public:
    using Element = ELEMENT;
    using ThisType = TMatrix<ELEMENT, ROWS, 1>;
    using Functions = TMatrixFunctions<Element, ThisType>;

protected:
    friend Functions;
    auto Instance() const { return TMatrix<ELEMENT, ROWS, 1>(); }
    auto InstanceDiag() const { return TMatrix<ELEMENT, 1, 1>(); }
    auto InstanceTransposed() const { return TMatrix<ELEMENT, 1, ROWS>(); }
    template<unsigned int R, unsigned int C>
    auto InstanceOuter(const TMatrix<ELEMENT, R, C>& m) const { return TMatrix<ELEMENT, ROWS, C>(); }
    auto InstanceOuter(const TMatrix<ELEMENT, 0, 0>& m) const { return TMatrix<ELEMENT, 0, 0>(ROWS, m.Columns()); }
    auto InstanceLogic() const { return TMatrix<bool, ROWS, 1>(); }
    auto InstanceMinor() const { return TMatrix<ELEMENT, (ROWS > 0) ? ROWS - 1 : 0, 0>(); }
    // todo: if M is static, these should be static asserts
    template<typename M> void assert_inner(const M& m) const { assert(Columns() == m.Rows()); }
    template<typename M> void assert_outer(const M& m) const { assert(Rows() == m.Columns()); }
    template<typename M> void assert_size(const M& m) const { assert(Rows() == m.Rows() && Columns() == m.Columns()); }
    void assert_square() const { assert(Rows() == Columns()); }

private:
    template<unsigned int index>
    void Set(const Element& value)
    {
        m_data[index] = value;
    }
    template<unsigned int index, typename ...Args>
    void Set(const Element& value, const Args&... values)
    {
        m_data[index] = value;
        Set<index + 1>(values...);
    }

public:
    TMatrix() : TMatrix(Element()) {}
    TMatrix(const std::array<Element, ROWS>& data)
        : m_data(data)
    {}
    template<typename ...Args>
    TMatrix(const Element & value, const Args& ...values)
    {
        Set<0>(value, values...);
    }
    TMatrix(const Element def)
    {
        m_data.fill(def);
    }
    TMatrix(const ThisType& other)
        : m_data(other.m_data)
    {}
    TMatrix(ThisType&& other)
        : m_data(other.m_data)
    {}

    ThisType& operator = (const ThisType& other)
    {
        m_data = other.m_data;
        return *this;
    }
    ThisType& operator = (ThisType&& other)
    {
        m_data = std::move(other.m_data);
        return *this;
    }

    inline unsigned int Columns() const { return 1; }
    inline unsigned int Rows() const { return ROWS; }
    inline unsigned int Dimension() const { return ROWS; }
    inline unsigned int Elements() const { return ROWS; }
    inline unsigned int Index(const unsigned int row, const unsigned int column) const { return row + column; }

    inline Element& operator () (const unsigned int row, const unsigned int column) { return m_data[Index(row, column)]; }
    inline Element operator () (const unsigned int row, const unsigned int column) const { return m_data[Index(row, column)]; }
    inline Element& operator () (const unsigned int index) { return m_data[index]; }
    inline Element operator () (const unsigned int index) const { return m_data[index]; }
    inline Element& operator [] (const unsigned int index) { return m_data[index]; }
    inline Element operator [] (const unsigned int index) const { return m_data[index]; }

    const Element* GetData() const
    {
        return m_data.data();
    }
    Element* GetData()
    {
        return m_data.data();
    }

    template<typename V>
    typename std::enable_if<is_vector<V>::value && is_vector<ThisType>::value, Element>::type
    InnerProduct(const V& other) const
    {
        auto res = m_data[0] * other[0];
        for (unsigned int i = 1; i < Dimension(); ++i)
        {
            res += m_data[i] * other[i];
        }
        return res;
    }

    template<typename V>
    typename std::enable_if<is_vector<V>::value && is_vector<ThisType>::value, ThisType>::type
    OuterProduct(const V& other) const
    {
        ThisType res(*this);
        for (unsigned int i = 0; i < Dimension(); ++i)
        {
            res[i] *= other[i];
        }
        return res;
    }

    template<typename V>
    typename std::enable_if<is_vector_3D<V>::value && is_vector_3D<ThisType>::value, ThisType>::type
    Cross(const V& other) const
    {
        return ThisType(
            m_data[1] * other[2] - m_data[2] * other[1],
            m_data[2] * other[0] - m_data[0] * other[2],
            m_data[0] * other[1] - m_data[1] * other[0]
        );
    }

    Element Length() const
    {
        return sqrt(LengthSquared());
    }
    Element LengthSquared() const
    {
        Element res = pow(m_data[0], 2);
        for (unsigned int i = 1; i < Dimension(); ++i)
        {
            res += pow(m_data[i], 2);
        }
        return res;
    }

    auto& Normalize()
    {
        return *this /= Length();
    }
    auto Normalized() const
    {
        auto length = Length();
        assert(length > 0.000001);
        return *this / length;
    }

private:
    std::array<Element, ROWS> m_data;
};
