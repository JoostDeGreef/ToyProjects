//
// Calculate the Cofactor matrix
//
template<typename ELEMENT, typename MATRIX>
auto TMatrixFunctions<ELEMENT, MATRIX>::Cofactor() const
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
