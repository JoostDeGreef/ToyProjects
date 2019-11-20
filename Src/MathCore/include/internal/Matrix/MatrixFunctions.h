template<typename ELEMENT,typename MATRIX>
class TMatrixFunctions 
{
public:
    using Element = ELEMENT;
    using Matrix = MATRIX;
    using ThisType = TMatrixFunctions<Element,MATRIX>;

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
    auto operator & (const Element& e) const
    {
        return Matrix(base) &= e;
    }
    template<typename DUMMY = int, typename std::enable_if<std::is_integral<Element>::value, DUMMY>::type = 0>
    auto& operator &= (const Element & e)
    {
        return PerformOperator([&e](const Element& a) {return a & e; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator & (const M& other) const
    {
        return Matrix(base) &= other;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value && std::is_integral<Element>::value, int>::type = 0>
    auto& operator &= (const M & other)
    {
        return PerformOperator(other,[](const Element& a, const Element& b) {return a & b; });
    }
    auto operator ^ (const Element& e) const
    {
        return Matrix(base) ^= e;
    }
    template<typename std::enable_if<std::is_integral<Element>::value, int>::type = 0>
    auto& operator ^= (const Element & e)
    {
        return PerformOperator([&e](const Element& a) {return a ^ e; });
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto operator ^ (const M& other) const
    {
        return Matrix(base) ^= other;
    }
    template<typename M, typename std::enable_if<is_matrix<M>::value && std::is_integral<Element>::value, int>::type = 0>
    auto& operator ^= (const M & other)
    {
        return PerformOperator(other,[](const Element& a, const Element& b) {return a ^ b; });
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
      assert(row<base.Rows() && column<base.Columns());
      auto dst = base.InstanceMinor();
      auto CopyBlock = [&](const unsigned int r,const unsigned int c,const unsigned int rc,const unsigned int cc,const unsigned int ro,const unsigned int co)
      {
        for(unsigned int ri=r;ri<r+rc;++ri)
        {
          for(unsigned int ci=c;ci<c+cc;++ci)
          {
            dst(ri-ro,ci-co) = base(ri,ci);
          }
        }
      };
      CopyBlock(0,    0,       row,              column,                 0,0);
      CopyBlock(0,    column+1,row,              base.Columns()-column-1,0,1);
      CopyBlock(row+1,0,       base.Rows()-row-1,column,                 1,0);
      CopyBlock(row+1,column+1,base.Rows()-row-1,base.Columns()-column-1,1,1);
      return dst;
    }
    
    //
    // Calculate the Minor Value (determinant of the Minor Matrix)
    //
    Element MinorValue(const unsigned int row, const unsigned int column) const
    {
      assert(row<base.Rows() && column<base.Columns());
      base.assert_square();
      switch(base.Rows())
      {
        case 0:
        case 1:
          return 0;
        case 2:
          return base((1-row),(1-column));
        case 3:
        {
          unsigned int r0 = (row==0) ? 1 : 0;
          unsigned int r1 = (row==1) ? 2 : 1;
          unsigned int c0 = (column==0) ? 1 : 0;
          unsigned int c1 = (column==1) ? 2 : 1;
          return base(r0,c0)*base(r1,c1) - base(r0,c1)*base(r1,c0);
        }
        default:
          return base.Minor(row,column).Determinant();
      }
    }

    //
    // Calculate the Cofactor matrix
    //
    auto Cofactor() const
    {
      auto res = base.Instance();
      for(unsigned int r=0;r<base.Rows();++r)
      {
        for(unsigned int c=0;c<base.Columns();++c)
        {
          const auto m = base.MinorValue(r,c);
          res(r,c) = (r&1)==(c&1)?m:-m;
        }
      }
      return res;
    }
    
    //
    // Calculate the determinant    
    //
    Element Determinant() const
    {
      base.assert_square();
      switch(base.Rows())
      {
        case 0:
          return 0;
        case 1:
          return base(0);
        case 2:
          return base(0)*base(3)-base(1)*base(2);
        case 3:
          return base(0)*(base(4)*base(8)-base(5)*base(7))
               - base(1)*(base(3)*base(8)-base(5)*base(6))
               + base(2)*(base(3)*base(7)-base(4)*base(6));
        default:
          break;
      }
      auto c = base.Cofactor();
      auto res = c(0)*base(0);
      for(unsigned int i=1;i<base.Elements();++i)
      {
        res += c(i)*base(i);
      }
      return res;
    }
    
};

