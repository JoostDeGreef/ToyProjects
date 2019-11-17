template<typename DATA>
class TMatrixFunctions : public DATA
{
public:
    using Data = DATA;
    using Element = typename Data::Element;
    using ThisType = TMatrixFunctions<Data>;
    using MatrixColumnColumn = typename Data::MatrixColumnColumn;
    using MatrixColumnRow = typename Data::MatrixColumnRow;
    using MatrixRowColumn = typename Data::MatrixRowColumn;
    using MatrixRowRow = typename Data::MatrixRowRow;

    TMatrixFunctions() : TMatrixFunctions(0, 0) {}
    TMatrixFunctions(const unsigned int rows, const unsigned int columns) : Data(rows, columns) {}
    TMatrixFunctions(const unsigned int rows, const unsigned int columns, const Element & def) : Data(rows, columns, def) {}
    TMatrixFunctions(const ThisType& other) : Data(other) {}
    TMatrixFunctions(ThisType&& other) : Data(other) {}

    using Data::operator ();
    using Data::Columns;
    using Data::Rows;
    using Data::Elements;

    //
    // Clear
    //
    void Clear()
    {
        Fill(Element());
    }

    //
    // Fill
    //
    void Fill(const Element& value)
    {
        for (unsigned int i = 0; i < Elements(); ++i)
        {
            Get()[i] = value;
        }
    }

    //
    // Transpose / Transposed
    //
    using Data::Transpose;
    auto Transposed() const
    {
        auto res = MatrixColumnRow::Instance(Columns(),Rows());
        unsigned int index = 0;
        for (unsigned int column = 0; column < Columns(); ++column)
        {
            for (unsigned int row = 0; row < Rows(); ++row, ++index)
            {
                res(index) = (*this)(row, column);
            }
        }
        return res;
    }

    //
    // +
    //
    auto operator + () const
    {
        return *(MatrixRowColumn*)(this);
    }
    const auto& operator + ()
    {
        return *(MatrixRowColumn*)(this);
    }
    auto operator + (const ThisType& other) const
    {
        return MatrixRowColumn(*this) += other;
    }
    auto& operator += (const ThisType& other)
    {
        other.TestSize(*this);
        for (unsigned int i = 0; i < Elements(); ++i)
        {
            Get()[i] += other(i);
        }
        return *(MatrixRowColumn*)(this);
    }

    //
    // -
    //
    auto operator - (const ThisType& other) const
    {
        return MatrixRowColumn(*this) -= other;
    }
    auto& operator -= (const ThisType& other)
    {
        other.TestSize(*this);
        for (unsigned int i = 0; i < Elements(); ++i)
        {
            Get()[i] -= other(i);
        }
        return *(MatrixRowColumn*)(this);
    }
    auto operator - () const
    {
        auto res(*this);
        for (unsigned int i = 0; i < Elements(); ++i)
        {
            res(i) = -res(i);
        }
        return (MatrixRowColumn)(res);
    }

    //
    // *
    //

protected:
    using Data::Get;
    using Data::Index;
    using Data::Instance;

private:
    inline void TestSize(const unsigned int rows, const unsigned int columns) const
    {
        assert(rows == Rows() && columns == Columns());
    }
    inline void TestSize(const ThisType& other) const
    {
        TestSize(other.Rows(), other.Columns());
    }
};
