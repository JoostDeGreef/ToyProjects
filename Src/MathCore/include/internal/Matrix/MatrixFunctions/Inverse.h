//
// Calculate the inverse
//
template<typename ELEMENT, typename MATRIX>
auto TMatrixFunctions<ELEMENT, MATRIX>::Inverse() const
{
    base.assert_square();
    auto a = base.Adjugate();
    auto det = base(0) * a(0);
    for (unsigned int i = 1; i < base.Rows(); ++i)
    {
        det += base(i) * a(i, 0);
    }
    // det = zero? -> singular matrix exception
    return a / det;
}
