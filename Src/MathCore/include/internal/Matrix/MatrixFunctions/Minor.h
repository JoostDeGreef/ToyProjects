//
// Calculate Matrix Minor for position
//
template<typename ELEMENT, typename MATRIX>
auto TMatrixFunctions<ELEMENT, MATRIX>::Minor(const unsigned int row, const unsigned int column) const
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
template<typename ELEMENT, typename MATRIX>
auto TMatrixFunctions<ELEMENT, MATRIX>::MinorValue(const unsigned int row, const unsigned int column) const
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
        break;
    }
    class PartialDeterminant
    {
    public:
        PartialDeterminant(Matrix& base)
            : m_base(base)
            , m_rows()
            , m_columns()
        {
            m_columns.resize(m_base.Rows());
            for(unsigned int i=0;i<m_base.Rows();++i)
            {
                m_columns[i] = i;
            }
            m_rows = m_columns;
        }
    
        auto Det2(const Element& a,const Element& b,
                  const Element& c,const Element& d) const
        { 
            return a*d - b*c;
        };
        auto Det3(const Element& a,const Element& b,const Element& c,
                  const Element& d,const Element& e,const Element& f,
                  const Element& g,const Element& h,const Element& i) const
        {
            return a*Det2(e,f,h,i)
                 - b*Det2(d,f,g,i)
                 + c*Det2(d,e,g,h);
        };
        auto Det4(const Element& a,const Element& b,const Element& c, const Element& d,
                  const Element& e,const Element& f,const Element& g, const Element& h,
                  const Element& i,const Element& j,const Element& k, const Element& l,
                  const Element& m,const Element& n,const Element& o, const Element& p) const
        {
            return a*Det3(f,g,h,j,k,l,n,o,p)
                 - b*Det3(e,g,h,i,k,l,m,o,p)
                 + c*Det3(e,f,h,i,j,l,m,n,p)
                 - d*Det3(e,f,g,i,j,k,m,n,o);
        };

        Element Evaluate(const unsigned int row, const unsigned int column) const
        {
            return Evaluate(m_rows, m_columns, row, column);
        }
    protected:
        std::vector<unsigned int> PartialCopy(const std::vector<unsigned int>& v, const unsigned int skip) const
        {
            std::vector<unsigned int> res;
            res.resize(v.size()-1);
            for(unsigned int i=0;i<skip;++i)
            {
                res[i] = v[i];
            }
            for(unsigned int i=skip+1;i<v.size();++i)
            {
                res[i-1] = v[i];
            }
            return res;
        }
        Element Evaluate(const std::vector<unsigned int>& rowsIn, const std::vector<unsigned int>& columnsIn, const unsigned int row, const unsigned int column) const
        {
            const std::vector<unsigned int> rows = PartialCopy(rowsIn,row);
            const std::vector<unsigned int> columns = PartialCopy(columnsIn,column);
            const auto & elem = [&](const unsigned int r, const unsigned int c)
            {
                return m_base(rows[r],columns[c]);
            };
            switch (rows.size())
            {
            case 2: return Det2(elem(0,0), elem(0,1),
                                elem(1,0), elem(1,1));
            case 3: return Det3(elem(0,0), elem(0,1), elem(0,2), 
                                elem(1,0), elem(1,1), elem(1,2), 
                                elem(2,0), elem(2,1), elem(2,2));
            case 4: return Det4(elem(0,0), elem(0,1), elem(0,2), elem(0,3),
                                elem(1,0), elem(1,1), elem(1,2), elem(1,3),
                                elem(2,0), elem(2,1), elem(2,2), elem(2,3),
                                elem(3,0), elem(3,1), elem(3,2), elem(3,3));
            default:
               break;
            }
            Element res = elem(0,0)*Evaluate(rows,columns,0,0);
            for(unsigned int c=2;c<columns.size();c+=2)
            {
               res += elem(0,c)*Evaluate(rows,columns,0,c);
            }
            for(unsigned int c=1;c<columns.size();c+=2)
            {
               res -= elem(0,c)*Evaluate(rows,columns,0,c);               
            }
            return res;
        }
    private:
        Matrix& m_base;
        std::vector<unsigned int> m_rows;
        std::vector<unsigned int> m_columns;
    };
    PartialDeterminant partialDeterminant(base);
    return partialDeterminant.Evaluate(row,column);
}
