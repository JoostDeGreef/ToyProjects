template<typename ELEMENT, unsigned int ROWS, unsigned int COLUMNS>
class TMatrixDataFixed
{
public:
    using Element = ELEMENT;
    using ThisType = TMatrixDataFixed<Element, ROWS, COLUMNS>;
    using MatrixColumnColumn = TMatrixFixed<Element, COLUMNS, COLUMNS>;
    using MatrixColumnRow = TMatrixFixed<Element, COLUMNS, ROWS>;
    using MatrixRowColumn = TMatrixFixed<Element, ROWS, COLUMNS>;
    using MatrixRowRow = TMatrixFixed<Element, ROWS, ROWS>;

    TMatrixDataFixed()
        : m_data()
    {}
    TMatrixDataFixed(const ThisType& other)
        : m_data(other.m_data)
    {}
    TMatrixDataFixed(ThisType&& other)
        : m_data(other.m_data)
    {}

    static auto Instance(unsigned int, unsigned int)
    {
        return TMatrixFixed<Element, ROWS, COLUMNS>();
    }

    inline unsigned int Columns() const { return COLUMNS; }
    inline unsigned int Rows() const { return ROWS; }
    inline unsigned int Elements() const { return ROWS * COLUMNS; }
    inline unsigned int Index(const unsigned int row, const unsigned int column) const { return row * COLUMNS + column; }

    inline Element& operator () (const unsigned int row, const unsigned int column) { return m_data[Index(row, column)]; }
    inline Element operator () (const unsigned int row, const unsigned int column) const { return m_data[Index(row, column)]; }
    inline Element& operator () (const unsigned int index) { return m_data[index]; }
    inline Element operator () (const unsigned int index) const { return m_data[index]; }

    auto& Transpose()
    {
        static_assert(ROWS == COLUMNS);
        for (unsigned int row = 0; row < ROWS; ++row)
        {
            for (unsigned int column = row + 1; column < COLUMNS; ++column)
            {
                std::swap(m_data[Index(column, row)], m_data[Index(row, column)]);
            }
        }
        return *(MatrixRowColumn*)this;
    }

protected:
    Element* Get() { return m_data.data(); }

private:
    std::array<Element, COLUMNS * ROWS> m_data;
};

