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
class TMatrixFunctions : public TMatrixCRTPHelper<MATRIX>
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
    // Perform logic operator on each element of this
    //
    template<typename F>
    auto PerformLogicOperator(const F& f) const
    {
        auto res = base.InstanceLogic();
        for (unsigned int index = 0; index < base.Elements(); ++index)
        {
            res(index) = f(base(index));
        }
        return res;
    }
    //
    // Perform logic operator on each element of this and other
    //
    template<typename M, typename F, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto PerformLogicOperator(const M& other, const F& f) const
    {
        base.assert_size(other);
        auto res = base.InstanceLogic();
        for (unsigned int index = 0; index < base.Elements(); ++index)
        {
            res(index) = f(base(index), other.base(index));
        }
        return res;
    }
    //
    // Perform operator on each element of this
    //
    template<typename F>
    auto& PerformOperator(const F& f)
    {
        for (unsigned int index = 0; index < base.Elements(); ++index)
        {
            base(index) = f(base(index));
        }
        return base;
    }
    //
    // Perform operator on each element of this and other
    //
    template<typename M, typename F, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto& PerformOperator(const M& other, const F& f)
    {
        base.assert_size(other);
        for (unsigned int index = 0; index < base.Elements(); ++index)
        {
            base(index) = f(base(index), other.base(index));
        }
        return base;
    }

    //
    // Clear
    //
    auto& Clear()
    {
        return Fill(Element());
    }

    //
    // Fill
    //
    auto& Fill(const Element& value)
    {
        return PerformOperator([&value](const Element&) {return value; });
    }

    //
    // Fill diagonal with one, rest with zero
    //
    auto& Eye()
    {
        Clear();
        for (unsigned int i = 0; i < base.Rows() && i < base.Columns(); ++i)
        {
            base(i, i) = Element(1);
        }
        return base;
    }

    //
    // Extract diagonal as column vector
    //
    auto Diag()
    {
        auto res = base.InstanceDiag();
        for (unsigned int i = 0; i < base.Rows() && i < base.Columns(); ++i)
        {
            res(i) = base(i, i);
        }
        return res;
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
    auto operator + (const Element& shift) const
    {
        return Matrix(base) += shift;
    }
    auto & operator += (const Element& shift)
    {
        return PerformOperator([&shift](const Element& a) {return a + shift; });
    }
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
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto& operator += (const M& other)
    {
        return PerformOperator(other, [](const Element& a, const Element& b) {return a + b; });
    }

    //
    // -
    //
    auto operator - (const Element& shift) const
    {
        return Matrix(base) -= shift;
    }
    auto & operator -= (const Element& shift)
    {
        return PerformOperator([&shift](const Element& a) {return a - shift; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator - (const M& other) const
    {
        return Matrix(base) -= other.base;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto& operator -= (const M& other)
    {
        return PerformOperator(other, [](const Element& a, const Element& b) {return a - b; });
    }
    auto operator - () const
    {
        return Matrix(base).PerformOperator([](const Element& a) {return -a; });
    }

    //
    // *
    //
    auto operator * (const Element& scale) const
    {
        return Matrix(base) *= scale;
    }
    auto & operator *= (const Element& scale)
    {
        return PerformOperator([&scale](const Element& a) {return a * scale; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator * (const M& other) const
    {
        base.assert_inner(other);
        auto res = base.InstanceOuter(other);
        for (unsigned int row = 0; row < base.Rows(); ++row)
        {
            for( unsigned int col = 0; col < other.Columns(); ++col)
            {
                Element & element = res(row,col);
                for( unsigned int inner = 0; inner < base.Columns(); ++inner)
                {
                    element += base(row,inner)*other.base(inner,col);
                }
            }
        }
        return res;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator *= (const M& other)
    {
        return base = base * other;
    }
    
    //
    // equality operators between matrices
    //
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator == (const M& other) const
    {
        return PerformLogicOperator(other,[](const Element & a, const Element & b){ return a == b; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator != (const M& other) const
    {
        return PerformLogicOperator(other,[](const Element & a, const Element & b){ return a != b; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator > (const M& other) const
    {
        return PerformLogicOperator(other,[](const Element & a, const Element & b){ return a > b; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator >= (const M& other) const
    {
        return PerformLogicOperator(other,[](const Element & a, const Element & b){ return a >= b; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator < (const M& other) const
    {
        return PerformLogicOperator(other,[](const Element & a, const Element & b){ return a < b; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator <= (const M& other) const
    {
        return PerformLogicOperator(other,[](const Element & a, const Element & b){ return a <= b; });
    }

    //
    // Equality operators between matrix elements and value
    //
    auto operator == (const Element & e) const
    {
        return PerformLogicOperator([&e](const Element& a) { return a == e; });
    }
    auto operator != (const Element& e) const
    {
        return PerformLogicOperator([&e](const Element& a) { return a != e; });
    }
    auto operator > (const Element& e) const
    {
        return PerformLogicOperator([&e](const Element& a) { return a > e; });
    }
    auto operator >= (const Element& e) const
    {
        return PerformLogicOperator([&e](const Element& a) { return a >= e; });
    }
    auto operator < (const Element& e) const
    {
        return PerformLogicOperator([&e](const Element& a) { return a < e; });
    }
    auto operator <= (const Element& e) const
    {
        return PerformLogicOperator([&e](const Element& a) { return a <= e; });
    }

    //
    // Bitwise operators (only valid when Element is integer type)
    //
    template<typename std::enable_if<std::is_integral<Element>::value, int>::type = 0>
    auto operator ~ () const
    {
        return Matrix(base).PerformOperator([](const Element& a) {return ~a; });
    }
    auto operator | (const Element& e) const
    {
        return Matrix(base) |= e;
    }
    template<typename std::enable_if<std::is_integral<Element>::value, int>::type = 0>
    auto& operator |= (const Element & e)
    {
        return PerformOperator([&e](const Element& a) {return a | e; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator | (const M& other) const
    {
        return Matrix(base) |= other;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value && std::is_integral<Element>::value, int>::type = 0>
    auto& operator |= (const M & other)
    {
        return PerformOperator(other,[](const Element& a, const Element& b) {return a | b; });
    }

    //
    // Logic operators (only valid when Element is bool)
    //
    //template<typename std::enable_if<std::is_same<Element, bool>::value, int>::type = 0>
    //auto operator ! () const
    //{
    //    return Matrix(base).PerformOperator([](const Element& a) {return !a; });
    //}
    //template<typename std::enable_if<std::is_same<Element, bool>::value, int>::type = 0>
    //auto operator || (const Element& e) const
    //{
    //    return PerformOperator([&e](const Element& a) {return a || e; });
    //}
    //template<typename M, typename std::enable_if<std::is_same<Element, bool>::value, int>::type = 0>
    //auto operator || (const M& other) const
    //{
    //    return PerformOperator([](const Element& a, const Element& b) {return a || b; });
    //}
};

