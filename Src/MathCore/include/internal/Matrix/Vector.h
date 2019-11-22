#pragma once

template<typename VALUE_TYPE, unsigned int ROWS>
using TColumnVector = TMatrix<VALUE_TYPE, ROWS, 1>;

template<typename VALUE_TYPE, unsigned int COLUMNS>
using TRowVector = TMatrix<VALUE_TYPE, 1, COLUMNS>;

template<typename VALUE_TYPE, unsigned int ROWS>
using TVector = TColumnVector<VALUE_TYPE, ROWS>;

template<typename VALUE_TYPE>
using TVector3 = TVector<VALUE_TYPE, 3>;
using Vector3d = TVector3<double>;
using Vector3f = TVector3<float>;

template<typename VALUE_TYPE>
using TVector2 = TVector<VALUE_TYPE, 2>;
using Vector2d = TVector2<double>;
using Vector2f = TVector2<float>;
using Vector2i = TVector2<int>;

template<typename VALUE_TYPE>
inline TVector3<VALUE_TYPE> CrossProduct(const TVector3<VALUE_TYPE>& a, const TVector3<VALUE_TYPE>& b)
{
    return  TVector3<VALUE_TYPE>(
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]);
}

template<typename VALUE_TYPE>
inline VALUE_TYPE ScalarTripleProduct(const TVector3<VALUE_TYPE>& a, const TVector3<VALUE_TYPE>& b, const TVector3<VALUE_TYPE>& c)
{
    return a.InnerProduct(CrossProduct(b, c));
}

template<typename VALUE_TYPE>
inline TVector3<VALUE_TYPE> VectorTripleProduct(const TVector3<VALUE_TYPE>& a, const TVector3<VALUE_TYPE>& b, const TVector3<VALUE_TYPE>& c)
{
    return CrossProduct(a, CrossProduct(b, c));
}

