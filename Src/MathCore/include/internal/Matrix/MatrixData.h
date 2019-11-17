template<typename ELEMENT>
class TMatrixData
{
public:
    using Element = ELEMENT;
    using ThisType = TMatrixData<Element>;
    using MatrixColumnColumn = TMatrix<Element>;
    using MatrixColumnRow = TMatrix<Element>;
    using MatrixRowColumn = TMatrix<Element>;
    using MatrixRowRow = TMatrix<Element>;

    TMatrixData() : TMatrixData(0, 0) {}
    TMatrixData(const unsigned int rows, const unsigned int columns, const Element def)
        : m_data(columns* rows, def)
        , m_columns(columns)
        , m_rows(rows)
    {}
    TMatrixData(const unsigned int rows, const unsigned int columns)
        : TMatrixData(rows,columns,Element())
    {}
    TMatrixData(const ThisType& other)
        : m_data(other.m_data)
        , m_columns(other.m_columns)
        , m_rows(other.m_rows)
    {}
    TMatrixData(ThisType&& other)
        : m_data(other.m_data)
        , m_columns(other.m_columns)
        , m_rows(other.m_rows)
    {}

    static auto Instance(const unsigned int rows, const unsigned int columns)
    {
        return TMatrix<Element>(rows, columns); 
    }

    inline unsigned int Columns() const { return m_columns; }
    inline unsigned int Rows() const { return m_rows; }
    inline unsigned int Elements() const { return m_rows * m_columns; }
    inline unsigned int Index(const unsigned int row, const unsigned int column) const { return row * m_columns + column; }

    inline Element& operator () (const unsigned int row, const unsigned int column) { return m_data[Index(row, column)]; }
    inline Element operator () (const unsigned int row, const unsigned int column) const { return m_data[Index(row, column)]; }
    inline Element& operator () (const unsigned int index) { return m_data[index]; }
    inline Element operator () (const unsigned int index) const { return m_data[index]; }

    auto& Transpose()
    {
        if (m_rows == m_columns)
        {
            for (unsigned int row = 0; row < m_rows; ++row)
            {
                for (unsigned int column = row+1; column < m_columns; ++column)
                {
                    std::swap(m_data[Index(column, row)],m_data[Index(row, column)]);
                }
            }
        }
        else
        {
            std::vector<Element> data(Elements());
            unsigned int index = 0;
            for (unsigned int column = 0; column < m_columns; ++column)
            {
                for (unsigned int row = 0; row < m_rows; ++row, ++index)
                {
                    data[index] = m_data[Index(row,column)];
                }
            }
            data.swap(m_data);
            std::swap(m_rows, m_columns);
        }
        return *this;
    }
protected:
    Element* Get() { return m_data.data(); }

private:
    std::vector<Element> m_data;
    unsigned int m_columns;
    unsigned int m_rows;
};

