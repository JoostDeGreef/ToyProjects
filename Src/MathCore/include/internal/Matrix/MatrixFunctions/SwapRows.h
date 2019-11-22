//
// Swap the contents of two rows
//
template<typename ELEMENT, typename MATRIX>
void TMatrixFunctions<ELEMENT, MATRIX>::SwapRows(const unsigned int row0, const unsigned int row1)
{
    unsigned int i0 = row0 * base.Columns();
    unsigned int i1 = row1 * base.Columns();
    for (unsigned int i = 0; i < base.Columns(); ++i, ++i0, ++i1)
    {
        std::swap(base(i0), base(i1));
    }
}
