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
        return base.Minor(row, column).Determinant();
    }
}
