template<typename ELEMENT>
class TMatrix : public TMatrixFunctions<ELEMENT,TMatrix<ELEMENT>>
{
public:
    using Element = ELEMENT;
    using ThisType = TMatrix<Element>;
    using Functions = TMatrixFunctions<Element,ThisType>;

protected:
    friend Functions;
    auto InstanceTransposed() { return ThisType(Columns(), Rows()); }

public:
    TMatrix() : TMatrix(0, 0) {}
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

    inline Element& operator () (const unsigned int row, const unsigned int column) { return m_data[Index(row, column)]; }
    inline Element operator () (const unsigned int row, const unsigned int column) const { return m_data[Index(row, column)]; }
    inline Element& operator () (const unsigned int index) { return m_data[index]; }
    inline Element operator () (const unsigned int index) const { return m_data[index]; }

private:
    std::vector<Element> m_data;
    unsigned int m_columns;
    unsigned int m_rows;
};

