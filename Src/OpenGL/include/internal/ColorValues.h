#pragma once

#include <type_traits> 

namespace OpenGL
{
    namespace Details
    {
        template <typename T>
        struct AlwaysFalse : std::false_type
        {};
    }

    template <typename T>
    class TColorValue
    {
    public:
        static constexpr T Hi() { static_assert(Details::AlwaysFalse<T>::value, "Type not supported"); }
        static constexpr T Lo() { static_assert(Details::AlwaysFalse<T>::value, "Type not supported"); }
        static           T Rnd() { static_assert(Details::AlwaysFalse<T>::value, "Type not supported"); }
        static unsigned int ToByte(const T& value) { static_assert(Details::AlwaysFalse<T>::value, "Type not supported"); }
        static           T FromByte(const unsigned int value) { static_assert(Details::AlwaysFalse<T>::value, "Type not supported"); }
    };
    template <>
    class TColorValue<float>
    {
    public:
        static constexpr float Hi() { return 1.0f; }
        static constexpr float Lo() { return 0.0f; }
        static           float Rnd() { Numerics::NormalizedRandomNumber<float>(); }
        static unsigned int ToByte(const float& value) { return (unsigned int)(value * 255); }
        static           float FromByte(const unsigned int value)
        {
            return value / 255.0f;
        }
    };
    template <>
    class TColorValue<double>
    {
    public:
        static constexpr double Hi() { return 1.0; }
        static constexpr double Lo() { return 0.0; }
        static           double Rnd() { Numerics::NormalizedRandomNumber<double>(); }
        static unsigned int ToByte(const double& value) { return (unsigned int)(value * 255); }
        static           double FromByte(const unsigned int value) { return value / 255.0; }
    };
    template <>
    class TColorValue<unsigned char>
    {
    public:
        static constexpr unsigned char Hi() { return 255; }
        static constexpr unsigned char Lo() { return 0; }
        static           unsigned char Rnd() { (unsigned char)(255 * Numerics::NormalizedRandomNumber<double>()); }
        static unsigned int ToByte(const unsigned int& value) { return value; }
        static           unsigned char FromByte(const unsigned int value) { return value; }
    };
}

