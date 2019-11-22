#pragma once

template<typename ELEMENT, typename MATRIX>
class TVectorFunctions : public TMatrixFunctions<ELEMENT,MATRIX>
{
public:
    using Element = ELEMENT;
    using Matrix = MATRIX;
    using ThisType = TVectorFunctions<Element, Matrix>;
    using TMatrixFunctions<ELEMENT,MATRIX>::base;

    template<typename V>
    typename std::enable_if<is_vector<V>::value, Element>::type
    InnerProduct(const V& other) const
    {
        auto res = base.m_data[0] * other[0];
        for (unsigned int i = 1; i < base.Dimension(); ++i)
        {
            res += base.m_data[i] * other[i];
        }
        return res;
    }

    template<typename V>
    typename std::enable_if<is_vector<V>::value, Matrix>::type
    OuterProduct(const V& other) const
    {
        Matrix res(*this);
        for (unsigned int i = 0; i < base.Dimension(); ++i)
        {
            res[i] *= other[i];
        }
        return res;
    }

    template<typename V>
    typename std::enable_if<is_vector_3D<V>::value && is_vector_3D<Matrix>::value, Matrix>::type
    Cross(const V& other) const
    {
        return Matrix(
            base.m_data[1] * other[2] - base.m_data[2] * other[1],
            base.m_data[2] * other[0] - base.m_data[0] * other[2],
            base.m_data[0] * other[1] - base.m_data[1] * other[0]
        );
    }

    Element Length() const
    {
        return sqrt(LengthSquared());
    }
    Element LengthSquared() const
    {
        Element res = pow(base.m_data[0], 2);
        for (unsigned int i = 1; i < base.Dimension(); ++i)
        {
            res += pow(base.m_data[i], 2);
        }
        return res;
    }

    auto& Normalize()
    {
        return *this /= Length();
    }
    auto Normalized() const
    {
        auto length = Length();
        assert(length > 0.000001);
        return *this / length;
    }

};


