#pragma once

template<typename ELEMENT, typename MATRIX>
class TMatrixFunctions
{
public:
    using Element = ELEMENT;
    using Matrix = MATRIX;
    using ThisType = TMatrixFunctions<Element, MATRIX>;

protected:
    Matrix& base;

public:
    TMatrixFunctions() : base(static_cast<Matrix&>(*this)) {}

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
    // Fill with random values [min,max)
    //
    auto& Random(const Element& minValue,const Element& maxValue)
    {
        const Element range = maxValue - minValue;
        return PerformOperator([&minValue,&range](const Element&) {return minValue + Numerics::NormalizedRandomNumber(range); });
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
    auto Diag() const
    {
        auto res = base.InstanceDiag();
        for (unsigned int i = 0; i < base.Rows() && i < base.Columns(); ++i)
        {
            res(i) = base(i, i);
        }
        return res;
    }

    //
    // Multiply all the diagnal elements
    //
    Element Trace() const
    {
        Element res = base(0, 0);
        for (unsigned int i = 1; i < base.Rows() && i < base.Columns(); ++i)
        {
            res *= base(i, i);
        }
        return res;
    }

    //
    // Compare elements
    //
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    bool Equal(const M& other) const
    {
        base.assert_size(other);
        for (unsigned int i = 0; i < base.Elements(); ++i)
        {
            if (!Numerics::Equal(base(i),other.base(i)))
            {
                return false;
            }
        }
        return true;
    }

    //
    // Transpose / Transposed
    //
    template<typename DUMMY = int, typename std::enable_if<is_dynamic_matrix<Matrix>::value || is_square_static_matrix<Matrix>::value, DUMMY>::type = 0>
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
    auto& operator += (const Element& shift)
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
    auto& operator -= (const Element& shift)
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
    // /
    //
    auto operator / (const Element& scale) const
    {
        return Matrix(base) /= scale;
    }
    auto& operator /= (const Element& scale)
    {
        return PerformOperator([&scale](const Element& a) {return a / scale; });
    }

    //
    // *
    //
    auto operator * (const Element& scale) const
    {
        return Matrix(base) *= scale;
    }
    auto& operator *= (const Element& scale)
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
            for (unsigned int col = 0; col < other.Columns(); ++col)
            {
                Element& element = res(row, col);
                for (unsigned int inner = 0; inner < base.Columns(); ++inner)
                {
                    element += base(row, inner) * other.base(inner, col);
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
        return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a == b; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator != (const M& other) const
    {
        return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a != b; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator > (const M& other) const
    {
        return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a > b; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator >= (const M& other) const
    {
        return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a >= b; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator < (const M& other) const
    {
        return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a < b; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator <= (const M& other) const
    {
        return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a <= b; });
    }

    //
    // Equality operators between matrix elements and value
    //
    auto operator == (const Element& e) const
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
    template<typename DUMMY = int, typename std::enable_if<std::is_integral<Element>::value, DUMMY>::type = 0>
    auto operator ~ () const
    {
        return Matrix(base).PerformOperator([](const Element& a) {return ~a; });
    }
    auto operator | (const Element& e) const
    {
        return Matrix(base) |= e;
    }
    template<typename DUMMY = int, typename std::enable_if<std::is_integral<Element>::value, DUMMY>::type = 0>
    auto& operator |= (const Element& e)
    {
        return PerformOperator([&e](const Element& a) {return a | e; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator | (const M& other) const
    {
        return Matrix(base) |= other;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value && std::is_integral<Element>::value, int>::type = 0>
    auto& operator |= (const M& other)
    {
        return PerformOperator(other, [](const Element& a, const Element& b) {return a | b; });
    }
    auto operator & (const Element& e) const
    {
        return Matrix(base) &= e;
    }
    template<typename DUMMY = int, typename std::enable_if<std::is_integral<Element>::value, DUMMY>::type = 0>
    auto& operator &= (const Element& e)
    {
        return PerformOperator([&e](const Element& a) {return a & e; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator & (const M& other) const
    {
        return Matrix(base) &= other;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value && std::is_integral<Element>::value, int>::type = 0>
    auto& operator &= (const M& other)
    {
        return PerformOperator(other, [](const Element& a, const Element& b) {return a & b; });
    }
    auto operator ^ (const Element& e) const
    {
        return Matrix(base) ^= e;
    }
    template<typename DUMMY = int, typename std::enable_if<std::is_integral<Element>::value, DUMMY>::type = 0>
    auto& operator ^= (const Element& e)
    {
        return PerformOperator([&e](const Element& a) {return a ^ e; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator ^ (const M& other) const
    {
        return Matrix(base) ^= other;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value && std::is_integral<Element>::value, int>::type = 0>
    auto& operator ^= (const M& other)
    {
        return PerformOperator(other, [](const Element& a, const Element& b) {return a ^ b; });
    }

    //
    // Logic operators (only valid when Element is bool)
    //
    template<typename DUMMY = int, typename std::enable_if<std::is_same<Element, bool>::value, DUMMY>::type = 0>
    auto operator ! () const
    {
        return Matrix(base).PerformOperator([](const Element& a) {return !a; });
    }
    template<typename DUMMY = int, typename std::enable_if<std::is_same<Element, bool>::value, DUMMY>::type = 0>
    auto operator || (const Element& e) const
    {
        return PerformOperator([&e](const Element& a) {return a || e; });
    }
    template<typename M, typename std::enable_if<std::is_same<typename M::Element, bool>::value, int>::type = 0>
    auto operator || (const M& other) const
    {
        return PerformOperator([](const Element& a, const Element& b) {return a || b; });
    }

    //
    // Calculate Matrix Minor for position
    //
    auto Minor(const unsigned int row, const unsigned int column) const
    {
        assert(row < base.Rows() && column < base.Columns());
        auto dst = base.InstanceMinor();
        auto CopyBlock = [&](const unsigned int r, const unsigned int c, const unsigned int rc, const unsigned int cc, const unsigned int ro, const unsigned int co)
        {
            for (unsigned int ri = r; ri < r + rc; ++ri)
            {
                for (unsigned int ci = c; ci < c + cc; ++ci)
                {
                    dst(ri - ro, ci - co) = base(ri, ci);
                }
            }
        };
        CopyBlock(0, 0, row, column, 0, 0);
        CopyBlock(0, column + 1, row, base.Columns() - column - 1, 0, 1);
        CopyBlock(row + 1, 0, base.Rows() - row - 1, column, 1, 0);
        CopyBlock(row + 1, column + 1, base.Rows() - row - 1, base.Columns() - column - 1, 1, 1);
        return dst;
    }

    //
    // Calculate the Minor Value (determinant of the Minor Matrix)
    //
    Element MinorValue(const unsigned int row, const unsigned int column) const
    {
        assert(row < base.Rows() && column < base.Columns());
        base.assert_square();
        switch (base.Rows())
        {
        case 0:
        case 1:
            return 0;
        case 2:
            return base((1 - row), (1 - column));
        case 3:
        {
            unsigned int r0 = (row == 0) ? 1 : 0;
            unsigned int r1 = (row == 2) ? 1 : 2;
            unsigned int c0 = (column == 0) ? 1 : 0;
            unsigned int c1 = (column == 2) ? 1 : 2;
            return base(r0, c0) * base(r1, c1) - base(r0, c1) * base(r1, c0);
        }
        default:
            return base.Minor(row, column).Determinant();
        }
    }

    //
    // Calculate the Cofactor matrix
    //
    auto Cofactor() const
    {
        auto res = base.Instance();
        for (unsigned int r = 0; r < base.Rows(); ++r)
        {
            for (unsigned int c = 0; c < base.Columns(); ++c)
            {
                const auto m = base.MinorValue(r, c);
                res(r, c) = (r & 1) == (c & 1) ? m : -m;
            }
        }
        return res;
    }

    //
    // Calculate the Adjugate matrix
    //
    auto Adjugate() const
    {
        return base.Cofactor().Transpose();
    }

    //
    // Calculate the determinant    
    //
    Element Determinant() const
    {
        base.assert_square();
        switch (base.Rows())
        {
        case 0:
            return 0;
        case 1:
            return base(0);
        case 2:
            return base(0) * base(3) - base(1) * base(2);
        case 3:
            return base(0) * (base(4) * base(8) - base(5) * base(7))
                 - base(1) * (base(3) * base(8) - base(5) * base(6))
                 + base(2) * (base(3) * base(7) - base(4) * base(6));
        default:
            break;
        }
        auto res = base(0) * base.MinorValue(0, 0);
        for (unsigned int c = 1; c < base.Columns(); c+=2)
        {
            res -= base(c) * base.MinorValue(0, c);
        }
        for (unsigned int c = 2; c < base.Columns(); c += 2)
        {
            res += base(c) * base.MinorValue(0, c);
        }
        return res;
    }

    //
    // Calculate the inverse
    //
    auto Inverse() const
    {
        base.assert_square();
        auto a = base.Adjugate();
        auto det = base(0) * a(0);
        for (unsigned int i = 1; i < base.Rows(); ++i)
        {
            det += base(i) * a(i, 0);
        }
        // det = zero? -> singular matrix exception
        return a / det;
    }
        
    //
    // Swap the contents of two rows
    //
    void SwapRows(const unsigned int row0,const unsigned int row1)
    {
        unsigned int i0=row0*base.Columns();
        unsigned int i1=row1*base.Columns();
        for(unsigned int i=0;i<base.Columns();++i,++i0,++i1)
        {
            std::swap(base(i0),base(i1));
        }
    }
    
    //
    // Calculate a LUP decomposition
    //
    class LUPPacked
    {
    public:
        LUPPacked(Matrix& base)
            : m_LU(base)
            , m_P(base.Rows())
        {
            base.assert_square();
            for (unsigned int i = 0; i < base.Rows(); ++i)
            {
                m_P[i] = i;
            }
        }

        void Calculate()
        {
            for (unsigned int i = 0; i < m_LU.Rows(); ++i)
            {
                auto pivotRow = FindPivotRow(i);
                if (i != pivotRow)
                {
                    std::swap(m_P[i], m_P[pivotRow]);
                    m_LU.SwapRows(i, pivotRow);
                }
                for (unsigned int r = i + 1; r < m_LU.Rows(); ++r)
                {
                    auto factor = m_LU(r, i) / m_LU(i, i);
                    m_LU(r, i) = factor;
                    for (unsigned int c = i+1; c < m_LU.Columns(); ++c)
                    {
                        m_LU(r, c) -= factor * m_LU(i, c);
                    }
                }
            }
        }

        Matrix L() const
        {
            auto res = m_LU.Instance();
            for (unsigned int i = 0; i < res.Rows(); ++i)
            {
                res(i, i) = Element(1);
            }
            for (unsigned int r = 1; r < res.Rows(); ++r)
            {
                for (unsigned int c = 0; c < r; ++c)
                {
                    res(r, c) = m_LU(r, c);
                }
            }
            return res;
        }

        Matrix U() const
        {
            auto res = m_LU.Instance();
            for (unsigned int r = 0; r < res.Rows(); ++r)
            {
                for (unsigned int c = r; c < res.Columns(); ++c)
                {
                    res(r, c) = m_LU(r, c);
                }
            }
            return res;
        }

        Matrix P() const
        {
            auto res = m_LU.Instance();
            for (unsigned int r = 0; r < res.Rows(); ++r)
            {
                 res(r, m_P[r]) = Element(1);
            }
            return res;
        }
    private:
        Matrix m_LU;
        std::vector<unsigned int> m_P;

        // todo: improve pivot selection by taking the value which is largest relative to the other elements in a row
        unsigned int FindPivotRow(const unsigned int col)
        {
            unsigned int pivotRow = col;
            for (unsigned int i = col + 1; i < m_LU.Rows(); ++i)
            {
                if (m_LU(pivotRow, col) < m_LU(i, col))
                {
                    pivotRow = i;
                }
            }
            return pivotRow;
        }
    };
    auto LUP()
    {
        LUPPacked lup(base);
        lup.Calculate();
        return lup;
    }
};

