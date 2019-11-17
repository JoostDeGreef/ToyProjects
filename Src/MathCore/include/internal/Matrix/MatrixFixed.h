template<typename ELEMENT,unsigned int ROWS,unsigned int COLUMNS>
class TMatrixFixed : public TMatrixFunctions<ELEMENT, TMatrixFixed<ELEMENT,ROWS,COLUMNS>>
{
public:
    using Element = ELEMENT;
    using ThisType = TMatrixFixed<ELEMENT,ROWS,COLUMNS>;
    using Functions = TMatrixFunctions<Element, ThisType>;

protected:
    friend Functions;
    auto InstanceTransposed() { return TMatrixFixed<ELEMENT, COLUMNS, ROWS>(); }

public:
    TMatrixFixed() : TMatrixFixed(Element()) {}
    TMatrixFixed(const Element def)
    {
        m_data.fill(def);
    }
    TMatrixFixed(const ThisType& other)
        : m_data(other.m_data)
    {}
    TMatrixFixed(ThisType&& other)
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

private:
    std::array<Element,ROWS*COLUMNS> m_data;
};

