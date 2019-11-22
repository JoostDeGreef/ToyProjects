//
// Clear
//
template<typename ELEMENT, typename MATRIX>
auto& TMatrixFunctions<ELEMENT, MATRIX>::Clear()
{
    return Fill(Element());
}
