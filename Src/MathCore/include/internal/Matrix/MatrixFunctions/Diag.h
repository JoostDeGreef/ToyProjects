//
// Extract diagonal as column vector
//
template<typename ELEMENT, typename MATRIX>
auto TMatrixFunctions<ELEMENT, MATRIX>::Diag() const
{
    auto res = base.InstanceDiag();
    for (unsigned int i = 0; i < base.Rows() && i < base.Columns(); ++i)
    {
        res(i) = base(i, i);
    }
    return res;
}
