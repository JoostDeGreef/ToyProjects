#pragma once

#include <array>

template<typename VALUE_TYPE, unsigned int DIMENSION>
class TVector
{
public:
	typedef TVector<VALUE_TYPE, DIMENSION> this_type;
	typedef VALUE_TYPE value_type;
	typedef unsigned int index_type;
	typedef unsigned int size_type;
	
	static const size_type dimension;

private:
	std::array<value_type, DIMENSION> m_data;

public:
	TVector()
	{}
	TVector(const this_type& other)
	{
		Copy(other);
	}
	template<typename ...Args>
	TVector(const value_type& value, Args&& ...values)
	{
		Set(value, std::forward<Args>(values)...);
	}
	explicit TVector(const value_type* data)
	{
		Set(data);
	}

	this_type& operator = (const this_type& other)
	{
		Copy(other);
		return *this;
	}

	void Copy(const this_type& other)
	{
		m_data = other.m_data;
	}

private:
	template<index_type index>
	void SetIndex(const value_type& value)
	{
		m_data[index] = value;
	}
	template<index_type index, typename ...Args>
	void SetIndex(const value_type& value, const Args&... values)
	{
		m_data[index] = value;
		SetIndex<index + 1>(values...);
	}
public:
	template<typename ...Args>
	void Set(const value_type& value, const Args&... values)
	{
		static_assert(sizeof...(values) + 1 == DIMENSION, "Incorrect number of values");
		SetIndex<0>(value, values...);
	}
	void Set(const value_type* data)
	{
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			m_data[i] = data[i];
		}
	}

	void Fill(const value_type& value)
	{
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			m_data[i] = value;
		}
	}

	const value_type* GetData() const
	{
		return m_data.data();
	}
	value_type* GetData()
	{
		return m_data.data();
	}

public:
	bool operator == (const this_type& other) const
	{
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			if( !Numerics::Equal(m_data[i], other[i]) )
			{
			  return false;
			}
		}
		return true;
	}

	const value_type& operator [] (const index_type& index) const
	{
		return m_data[index];
	}
	value_type& operator [] (const index_type& index)
	{
		return m_data[index];
	}

	value_type InnerProduct(const this_type& other) const
	{
		value_type res = m_data[0] * other[0];
		for (index_type i = 1; i < DIMENSION; ++i)
		{
			res += m_data[i] * other[i];
		}
		return res;
	}

	this_type OuterProduct(const this_type& other) const
	{
		this_type res;
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			res[i] = m_data[i] * other[i];
		}
		return res;
	}

	template<typename VECTOR_TYPE = this_type>
	typename std::enable_if<3 == DIMENSION, VECTOR_TYPE>::type
		Cross(const this_type& other) const
	{
		return this_type(
			m_data[1] * other[2] - m_data[2] * other[1],
			m_data[2] * other[0] - m_data[0] * other[2],
			m_data[0] * other[1] - m_data[1] * other[0]
		);
	}

	value_type Length() const
	{
		return sqrt(LengthSquared());
	}
	value_type LengthSquared() const
	{
		value_type res = pow(m_data[0], 2);
		for (index_type i = 1; i < DIMENSION; ++i)
		{
			res += pow(m_data[i], 2);
		}
		return res;
	}

	this_type& Normalize()
	{
		return *this /= Length();
	}
	this_type Normalized() const
	{
		value_type length = Length();
		assert(length > 0.000001);
		return *this / length;
	}

	this_type operator + (const this_type& other) const
	{
		this_type res;
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			res[i] = m_data[i] + other[i];
		}
		return res;
	}
	this_type& operator += (const this_type& other)
	{
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			m_data[i] += other[i];
		}
		return *this;
	}

	this_type operator - (const this_type& other) const
	{
		this_type res;
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			res[i] = m_data[i] - other[i];
		}
		return res;
	}
	this_type& operator -= (const this_type& other)
	{
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			m_data[i] -= other[i];
		}
		return *this;
	}

	this_type operator * (const value_type& value) const
	{
		this_type res;
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			res[i] = m_data[i] * value;
		}
		return res;
	}
	this_type& operator *= (const value_type& value)
	{
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			m_data[i] *= value;
		}
		return *this;
	}

	this_type operator / (const value_type& value) const
	{
		this_type res;
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			res[i] = m_data[i] / value;
		}
		return res;
	}
	this_type& operator /= (const value_type& value)
	{
		for (index_type i = 0; i < DIMENSION; ++i)
		{
			m_data[i] /= value;
		}
		return *this;
	}
};

template<typename VALUE_TYPE, unsigned int DIMENSION>
const typename TVector<VALUE_TYPE, DIMENSION>::size_type TVector<VALUE_TYPE, DIMENSION>::dimension = DIMENSION;

template<typename VALUE_TYPE>
using TVector3 = TVector<VALUE_TYPE, 3>;
using Vector3d = TVector3<double>;
using Vector3f = TVector3<float>;

template<typename VALUE_TYPE>
using TVector2 = TVector<VALUE_TYPE, 2>;
using Vector2d = TVector2<double>;
using Vector2f = TVector2<float>;

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

