#pragma once

#include "Numerics.h"

template<typename ELEMENT,unsigned int ROWS,unsigned int COLS> class TMatrix;

template<typename T> struct is_matrix { static const bool value = false; };
template<typename E,unsigned int R,unsigned int C> struct is_matrix<TMatrix<E,R,C>> { static const bool value = true; };

template<typename T> struct is_square_static_matrix { static const bool value = false; };
template<typename E,unsigned int R,unsigned int C> struct is_square_static_matrix<TMatrix<E,R,C>> { static const bool value = C!=0 && C==R; };

template<typename T> struct is_static_matrix { static const bool value = false; };
template<typename E,unsigned int R,unsigned int C> struct is_static_matrix<TMatrix<E,R,C>> { static const bool value = C!=0 || R!=0; };

template<typename T> struct is_dynamic_matrix { static const bool value = false; };
template<typename E,unsigned int R,unsigned int C> struct is_dynamic_matrix<TMatrix<E,R,C>> { static const bool value = C==0 && R==0; };

#include "Matrix/MatrixFunctions.h"
#include "Matrix/Matrix.h"

