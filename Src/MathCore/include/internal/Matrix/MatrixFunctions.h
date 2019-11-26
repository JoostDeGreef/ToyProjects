#pragma once

template<typename ELEMENT, typename MATRIX>
class TMatrixFunctions
{
public:
    using Element = ELEMENT;
    using Matrix = MATRIX;
    using ThisType = TMatrixFunctions<Element, MATRIX>;

    template<typename M> void assert_inner(const M& m) const { assert(base.Columns()==m.Rows()); }
    template<typename M> void assert_outer(const M& m) const { assert(base.Rows()==m.Columns()); }
    template<typename M> void assert_size(const M& m) const { assert(base.Rows()==m.Rows() && base.Columns()==m.Columns()); }
                         void assert_square() const { assert(base.Rows()==base.Columns()); }

protected:
    Matrix& base;

public:
    TMatrixFunctions() : base(static_cast<Matrix&>(*this)) {}

    //
    // Perform logic operator on each element of this
    //
    template<typename F>
    auto PerformLogicOperator(const F& f) const;
    //
    // Perform logic operator on each element of this and other
    //
    template<typename M, typename F, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto PerformLogicOperator(const M& other, const F& f) const;
    //
    // Perform operator on each element of this
    //
    template<typename F>
    auto& PerformOperator(const F& f);
    //
    // Perform operator on each element of this and other
    //
    template<typename M, typename F, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto& PerformOperator(const M& other, const F& f);
    //
    // Clear
    //
    auto& Clear();
    //
    // Extract diagonal as column vector
    //
    auto Diag() const;
    //
    // Fill
    //
    auto& Fill(const Element& value);
    // 
    // Fill with random values [min,max)
    //
    auto& Random(const Element& minValue, const Element& maxValue);
    //
    // Fill diagonal with one, rest with zero
    //
    auto& Eye();
    //
    // Multiply all the diagnal elements
    //
    auto Trace() const;
    //
    // Compare all elements for equality
    //
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    bool Equal(const M& other) const;
    //
    // equality operators between matrices
    //
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator == (const M& other) const { return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a == b; }); }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator != (const M& other) const { return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a != b; }); }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator >  (const M& other) const { return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a >  b; }); }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator >= (const M& other) const { return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a >= b; }); }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator <  (const M& other) const { return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a <  b; }); }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator <= (const M& other) const { return PerformLogicOperator(other, [](const Element& a, const Element& b) { return a <= b; }); }
    //
    // Equality operators between matrix elements and value
    //
    auto operator == (const Element& e) const { return PerformLogicOperator([&e](const Element& a) { return a == e; }); }
    auto operator != (const Element& e) const { return PerformLogicOperator([&e](const Element& a) { return a != e; }); }
    auto operator >  (const Element& e) const { return PerformLogicOperator([&e](const Element& a) { return a > e; }); }
    auto operator >= (const Element& e) const { return PerformLogicOperator([&e](const Element& a) { return a >= e; }); }
    auto operator <  (const Element& e) const { return PerformLogicOperator([&e](const Element& a) { return a < e; }); }
    auto operator <= (const Element& e) const { return PerformLogicOperator([&e](const Element& a) { return a <= e; }); }
    //
    // Transpose / Transposed
    //
    template<typename DUMMY = int, typename std::enable_if<is_dynamic_matrix<Matrix>::value || is_square_static_matrix<Matrix>::value, DUMMY>::type = 0>
    auto& Transpose();
    auto Transposed() const;
    //
    // +
    //
    auto operator + (const Element& shift) const { return Matrix(base) += shift; }
    auto& operator += (const Element& shift) { return PerformOperator([&shift](const Element& a) {return a + shift; }); }
    auto operator + () const { return base; }
    const auto& operator + () { return base; }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator + (const M& other) const { return Matrix(base) += other; }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto& operator += (const M& other) { return PerformOperator(other, [](const Element& a, const Element& b) {return a + b; }); }
    //
    // -
    //
    auto operator - (const Element& shift) const { return Matrix(base) -= shift; }
    auto& operator -= (const Element& shift) { return PerformOperator([&shift](const Element& a) {return a - shift; }); }
    auto operator - () const { return Matrix(base).PerformOperator([](const Element& a) {return -a; }); }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator - (const M& other) const { return Matrix(base) -= other.base; }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto& operator -= (const M& other) { return PerformOperator(other, [](const Element& a, const Element& b) {return a - b; }); }
    //
    // /
    //
    auto operator / (const Element& scale) const { return Matrix(base) /= scale; }
    auto& operator /= (const Element& scale) { return PerformOperator([&scale](const Element& a) {return a / scale; }); }
    //
    // *
    //
    auto operator * (const Element& scale) const { return Matrix(base) *= scale; }
    auto& operator *= (const Element& scale) { return PerformOperator([&scale](const Element& a) {return a * scale; }); }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator *= (const M& other) { return base = base * other; }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator * (const M& other) const;
    //
    // Bitwise operators (only valid when Element is integer type)
    //
    auto operator | (const Element& e) const { return Matrix(base) |= e; }
    auto operator & (const Element& e) const { return Matrix(base) &= e; }
    auto operator ^ (const Element& e) const { return Matrix(base) ^= e; }
    template<typename DUMMY = int, typename std::enable_if<std::is_integral<Element>::value, DUMMY>::type = 0> auto operator ~ () const { return Matrix(base).PerformOperator([](const Element& a) {return ~a; }); }
    template<typename DUMMY = int, typename std::enable_if<std::is_integral<Element>::value, DUMMY>::type = 0> auto& operator |= (const Element& e) { return PerformOperator([&e](const Element& a) {return a | e; }); }
    template<typename DUMMY = int, typename std::enable_if<std::is_integral<Element>::value, DUMMY>::type = 0> auto& operator &= (const Element& e) { return PerformOperator([&e](const Element& a) {return a & e; }); }
    template<typename DUMMY = int, typename std::enable_if<std::is_integral<Element>::value, DUMMY>::type = 0> auto& operator ^= (const Element& e) { return PerformOperator([&e](const Element& a) {return a ^ e; }); }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>  auto operator | (const M& other) const { return Matrix(base) |= other; }
    template<typename M, typename std::enable_if<is_matrix<M>::value && std::is_integral<Element>::value, int>::type = 0> auto& operator |= (const M& other) { return PerformOperator(other, [](const Element& a, const Element& b) {return a | b; }); }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator & (const M& other) const { return Matrix(base) &= other; }
    template<typename M, typename std::enable_if<is_matrix<M>::value && std::is_integral<Element>::value, int>::type = 0> auto& operator &= (const M& other) { return PerformOperator(other, [](const Element& a, const Element& b) {return a & b; }); }
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> auto operator ^ (const M& other) const { return Matrix(base) ^= other; }
    template<typename M, typename std::enable_if<is_matrix<M>::value && std::is_integral<Element>::value, int>::type = 0> auto& operator ^= (const M& other) { return PerformOperator(other, [](const Element& a, const Element& b) {return a ^ b; }); }
    //
    // Logic operators (only valid when Element is bool)
    //
    template<typename DUMMY = int, typename std::enable_if<std::is_same<Element, bool>::value, DUMMY>::type = 0> auto operator ! () const { return Matrix(base).PerformOperator([](const Element& a) {return !a; }); }
    template<typename DUMMY = int, typename std::enable_if<std::is_same<Element, bool>::value, DUMMY>::type = 0> auto operator || (const Element& e) const { return PerformOperator([&e](const Element& a) {return a || e; }); }
    template<typename M, typename std::enable_if<std::is_same<typename M::Element, bool>::value, int>::type = 0> auto operator || (const M& other) const { return PerformOperator([](const Element& a, const Element& b) {return a || b; }); }
    //
    // Calculate Matrix Minor for position
    //
    auto Minor(const unsigned int row, const unsigned int column) const;
    //
    // Calculate the Minor Value (determinant of the Minor Matrix)
    //
    auto MinorValue(const unsigned int row, const unsigned int column) const;
    //
    // Calculate the Cofactor matrix
    //
    auto Cofactor() const;
    //
    // Calculate the Adjugate matrix
    //
    auto Adjugate() const { return base.Cofactor().Transpose(); }
    //
    // Calculate the determinant    
    //
    auto Determinant() const;
    //
    // Calculate the inverse
    //
    auto Inverse() const;
    //
    // Swap the contents of two rows
    //
    void SwapRows(const unsigned int row0, const unsigned int row1);
    //
    // Calculate a LUP decomposition
    //
    class LUPPacked;
    auto LUP();
    //
    // Solve A*x = b
    //
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0> static auto Solve(const LUPPacked & lup,const M& b);
    template<typename M, typename std::enable_if<is_matrix<M>::value, int>::type = 0>        auto Solve(const M& b) const;
    //
    // Write data to stream
    //
    std::ostream& to_stream(std::ostream& s) const;
};

#include "MatrixFunctions/Clear.h"
#include "MatrixFunctions/Cofactor.h"
#include "MatrixFunctions/Determinant.h"
#include "MatrixFunctions/Diag.h"
#include "MatrixFunctions/Equal.h"
#include "MatrixFunctions/Eye.h"
#include "MatrixFunctions/Fill.h"
#include "MatrixFunctions/Inverse.h"
#include "MatrixFunctions/LUP.h"
#include "MatrixFunctions/Minor.h"
#include "MatrixFunctions/Multiply.h"
#include "MatrixFunctions/PerformLogicOperator.h"
#include "MatrixFunctions/PerformOperator.h"
#include "MatrixFunctions/Random.h"
#include "MatrixFunctions/SwapRows.h"
#include "MatrixFunctions/Trace.h"
#include "MatrixFunctions/Transpose.h"

#include "MatrixFunctions/to_stream.h"


