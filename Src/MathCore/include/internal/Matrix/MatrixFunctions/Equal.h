//
// Compare all elements for equality
//
template<typename ELEMENT, typename MATRIX>
template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type>
bool TMatrixFunctions<ELEMENT, MATRIX>::Equal(const M& other) const
{
    base.assert_size(other);
    for (unsigned int i = 0; i < base.Elements(); ++i)
    {
        if (!Numerics::Equal(base(i), other.base(i)))
        {
            return false;
        }
    }
    return true;
}


