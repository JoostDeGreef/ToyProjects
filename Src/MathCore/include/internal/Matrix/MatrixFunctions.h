template <typename T>
struct TMatrixCRTPHelper
{
    T& Base() 
    { 
        return static_cast<T&>(*this); 
    }
    T const& Base() const 
    { 
        return static_cast<T const&>(*this); 
    }
};

template<typename ELEMENT,typename MATRIX>
class TMatrixFunctions : TMatrixCRTPHelper<MATRIX>
{
public:
    using Element = ELEMENT;
    using Matrix = MATRIX;
    using ThisType = TMatrixFunctions<Element,MATRIX>;

protected:
    Matrix& base;

public:
    TMatrixFunctions() : base(this->Base()) {}

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
        for (unsigned int i = 0; i < base.Elements(); ++i)
        {
            base(i) = value;
        }
    }

    //
    // Transpose / Transposed
    //
    auto& Transpose()
    {
        if (base.Rows() == base.Columns())
        {
            for (unsigned int row = 0; row < base.Rows(); ++row)
            {
                for (unsigned int column = row + 1; column < base.Columns(); ++column)
                {
                    std::swap(base(column, row), base(row, column));
                }
            }
        }
        else
        {
            base = Transposed();
        }
        return base;
    }
    auto Transposed() const
    {
        auto res = base.InstanceTransposed();
        unsigned int index = 0;
        for (unsigned int column = 0; column < base.Columns(); ++column)
        {
            for (unsigned int row = 0; row < base.Rows(); ++row, ++index)
            {
                res(index) = base(row, column);
            }
        }
        return res;
    }

    //
    // +
    //
    auto operator + () const
    {
        return base;
    }
    const auto& operator + ()
    {
        return base;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator + (const M& other) const
    {
        return Matrix(base) += other;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value,int>::type = 0>
    auto& operator += (const M& other)
    {
        for (unsigned int i = 0; i < base.Elements(); ++i)
        {
            base(i) += other.base(i);
        }
        return base;
    }

    //
    // -
    //
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator - (const M& other) const
    {
        return Matrix(base) -= other.base;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto& operator -= (const M& other)
    {
        for (unsigned int i = 0; i < base.Elements(); ++i)
        {
            base(i) -= other.base(i);
        }
        return *(Matrix*)(this);
    }
    auto operator - () const
    {
        auto res(base);
        for (unsigned int i = 0; i < base.Elements(); ++i)
        {
            res(i) = -res(i);
        }
        return res;
    }

    //
    // *
    //
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator * (const M& other) const
    {
        //auto res = InstanceOuter<>
        //return MatrixRowColumn(*this) -= other;
        return 0;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator *= (const M& other)
    {
        //auto res = Instance<>
        //return MatrixRowColumn(*this) -= other;
        return 0;
    }
};
