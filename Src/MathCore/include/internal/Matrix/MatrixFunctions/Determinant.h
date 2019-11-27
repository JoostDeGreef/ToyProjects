//
// Calculate the determinant    
//
template<typename ELEMENT, typename MATRIX>
auto TMatrixFunctions<ELEMENT, MATRIX>::Determinant() const
{
    base.assert_square();
    auto Det1 = [&](const Element& a) { return a; };
    auto Det2 = [&](const Element& a,const Element& b,
                    const Element& c,const Element& d) { return a*d - b*c; };
    auto Det3 = [&](const Element& a,const Element& b,const Element& c,
                    const Element& d,const Element& e,const Element& f,
                    const Element& g,const Element& h,const Element& i) { return a*Det2(e,f,h,i) - b*Det2(d,f,g,i) + c*Det2(d,e,g,h); };
    auto Det4 = [&](const Element& a,const Element& b,const Element& c, const Element& d,
                    const Element& e,const Element& f,const Element& g, const Element& h,
                    const Element& i,const Element& j,const Element& k, const Element& l,
                    const Element& m,const Element& n,const Element& o, const Element& p) { return a*Det3(f,g,h,j,k,l,n,o,p) - b*Det3(e,g,h,i,k,l,m,o,p) + c*Det3(e,f,h,i,j,l,m,n,p) - d*Det3(e,f,g,i,j,k,m,n,o); };
    switch (base.Rows())
    {
    case 0: return 0;
    case 1: return Det1(base(0));
    case 2: return Det2(base(0), base(1), base(2), base(3));
    case 3: return Det3(base(0), base(1), base(2), base(3), base(4), base(5), base(6), base(7), base(8));
    case 4: return Det4(base(0), base(1), base(2), base(3), base(4), base(5), base(6), base(7), base(8), base(9), base(10), base(11), base(12), base(13), base(14), base(15));
    default:
        break;
    }
    auto res = base(0) * base.MinorValue(0, 0);
    for (unsigned int c = 1; c < base.Columns(); c += 2)
    {
        res -= base(c) * base.MinorValue(0, c);
    }
    for (unsigned int c = 2; c < base.Columns(); c += 2)
    {
        res += base(c) * base.MinorValue(0, c);
    }
    return res;
}
