//
// Calculate the determinant    
//
template<typename ELEMENT, typename MATRIX>
auto TMatrixFunctions<ELEMENT, MATRIX>::Determinant() const
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
