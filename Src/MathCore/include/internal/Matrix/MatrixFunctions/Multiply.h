template<typename ELEMENT, typename MATRIX>
template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type>
auto TMatrixFunctions<ELEMENT, MATRIX>::operator * (const M& other) const
{
    base.assert_inner(other);
    auto res = base.InstanceOuter(other);
    for (unsigned int row = 0; row < base.Rows(); ++row)
    {
        for (unsigned int col = 0; col < other.Columns(); ++col)
        {
            Element& element = res(row, col);
            for (unsigned int inner = 0; inner < base.Columns(); ++inner)
            {
                element += base(row, inner) * other.base(inner, col);
            }
        }
    }
    return res;
}
