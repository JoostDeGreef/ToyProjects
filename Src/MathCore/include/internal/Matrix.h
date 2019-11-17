#pragma once

template<typename ELEMENT> class TMatrix;
template<typename ELEMENT,unsigned int ROWS,unsigned int COLS> class TMatrixFixed;

template<typename T> struct is_matrix { bool value = false; };
template<typename E> struct is_matrix<TMatrix<E>> { bool value = true; };
template<typename E,unsigned int R,unsigned int C> struct is_matrix<TMatrixFixed<E,R,C>> { bool value = true; };

#include "Matrix/MatrixFunctions.h"
#include "Matrix/Matrix.h"
#include "Matrix/MatrixFixed.h"

