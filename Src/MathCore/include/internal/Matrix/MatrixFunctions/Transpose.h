//
// Transpose / Transposed
//
template<typename ELEMENT, typename MATRIX>
template<typename DUMMY, typename std::enable_if<is_dynamic_matrix<MATRIX>::value || is_square_static_matrix<MATRIX>::value, DUMMY>::type>
auto& TMatrixFunctions<ELEMENT, MATRIX>::Transpose()
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
template<typename ELEMENT, typename MATRIX>
auto TMatrixFunctions<ELEMENT, MATRIX>::Transposed() const
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
