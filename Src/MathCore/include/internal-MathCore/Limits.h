#pragma once

template<typename T>
class Limits
{
public:
	static constexpr T CompareEpsilon = (T)1e-10;
	static constexpr T MinValue = std::numeric_limits<T>::lowest();
	static constexpr T MaxValue = std::numeric_limits<T>::max();
};

