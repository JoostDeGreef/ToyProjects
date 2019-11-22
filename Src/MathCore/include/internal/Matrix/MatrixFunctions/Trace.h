//
// Multiply all the diagnal elements
//
template<typename ELEMENT, typename MATRIX>
auto TMatrixFunctions<ELEMENT, MATRIX>::Trace() const
{
    Element res = base(0, 0);
    for (unsigned int i = 1; i < base.Rows() && i < base.Columns(); ++i)
    {
        res *= base(i, i);
    }
    return res;
}
