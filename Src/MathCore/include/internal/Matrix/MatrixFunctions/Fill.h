//
// Fill
//
template<typename ELEMENT, typename MATRIX>
auto& TMatrixFunctions<ELEMENT, MATRIX>::Fill(const Element& value)
{
    return PerformOperator([&value](const Element&) {return value; });
}
