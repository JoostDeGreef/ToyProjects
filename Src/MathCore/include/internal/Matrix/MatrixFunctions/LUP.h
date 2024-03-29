//
// Calculate a LUP decomposition
//
template<typename ELEMENT, typename MATRIX>
class TMatrixFunctions<ELEMENT, MATRIX>::LUPPacked
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
                for (unsigned int c = i + 1; c < m_LU.Columns(); ++c)
                {
                    m_LU(r, c) -= factor * m_LU(i, c);
                }
            }
        }
    }

    const Matrix& LU() const
    {
        return m_LU;
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
    
    const std::vector<unsigned int> & p() const
    {
        return m_P;
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

template<typename ELEMENT, typename MATRIX>
auto TMatrixFunctions<ELEMENT, MATRIX>::LUP()
{
    LUPPacked lup(base);
    lup.Calculate();
    return lup;
}

template<typename ELEMENT, typename MATRIX>
template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type>
auto TMatrixFunctions<ELEMENT, MATRIX>::Solve(const M& b) const
{
   return Matrix::Solve(base.LUP(),b);
}

template<typename ELEMENT, typename MATRIX>
template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type>
auto TMatrixFunctions<ELEMENT, MATRIX>::Solve(const LUPPacked & lup,const M& b)
{
    auto & lu = lup.LU();
    auto & p = lup.p();
    lu.assert_inner(b);
    auto y = b.Instance();
    for(unsigned int r=0;r<y.Rows();++r)
    {
        unsigned int pr = p[r];
        for(unsigned int c=0;c<y.Columns();++c)
        {
            y(r,c) = b(pr,c);
        }
    }
    for(unsigned int r=0;r<y.Rows();++r)
    {
        for(unsigned int c=0;c<y.Columns();++c)
        {
            Element & e = y(r,c);
            for(unsigned int i=0;i<r;++i)
            {
                e -= y(i,c)*lu(r,i);
            }
        }
    }
    auto x = y;
    for(unsigned int r=y.Rows()-1;r<y.Rows();--r)
    {
        for(unsigned int c=0;c<x.Columns();++c)
        {
            Element & e = x(r,c);
            for(unsigned int i=r+1;i<y.Rows();++i)
            {
                e -= x(i,c)*lu(r,i);
            }
            e /= lu(r,r);
        }
    }
    return x;
}

