//
// Fill diagonal with one, rest with zero
//
template<typename ELEMENT, typename MATRIX>
auto& TMatrixFunctions<ELEMENT, MATRIX>::Eye()
{
    Clear();
    for (unsigned int i = 0; i < base.Rows() && i < base.Columns(); ++i)
    {
        base(i, i) = Element(1);
    }
    return base;
}
