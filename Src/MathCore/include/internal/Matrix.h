#pragma once

template<typename ELEMENT> class TMatrix;
template<typename ELEMENT,unsigned int ROWS,unsigned int COLS> class TMatrixFixed;

template<typename ELEMENT>
class TMatrixData
{
public:
  using Element = ELEMENT;
  using ThisType = TMatrixData<Element>;
  using MatrixColumnColumn = TMatrix<Element>;
  using MatrixColumnRow = TMatrix<Element>;
  using MatrixRowColumn = TMatrix<Element>;
  using MatrixRowRow = TMatrix<Element>;
  
  TMatrixData() : TMatrixData(0,0) {}
  TMatrixData(unsigned int columns,unsigned int rows)
    : m_data()
    , m_columns(columns)
    , m_rows(rows)
  {
    m_data.resize(columns*rows);
  }
  TMatrixData(const ThisType & other) 
    : m_data(other.m_data)
    , m_columns(other.m_columns)
    , m_rows(other.m_rows)
  {}
  TMatrixData(ThisType && other)
    : m_data(other.m_data)
    , m_columns(other.m_columns)
    , m_rows(other.m_rows)
  {}

  unsigned int Columns() const { return m_columns; }
  unsigned int Rows() const { return m_rows; }
  unsigned int Elements() const { return m_rows*m_columns; }
  unsigned int Index(const unsigned int row, const unsigned int column) const { return row*m_columns + column; }
  
  Element & operator () (const unsigned int row, const unsigned int column) { return m_data[Index(row,column)]; }
  Element operator () (const unsigned int row, const unsigned int column) const { return m_data[Index(row,column)]; }
  Element & operator () (const unsigned int index) { return m_data[index]; }
  Element operator () (const unsigned int index) const { return m_data[index]; }

protected:
  Element * Get() { return m_data.data(); }
  
private:
  std::vector<Element> m_data;
  unsigned int m_columns;
  unsigned int m_rows;
};




template<typename ELEMENT,unsigned int ROWS,unsigned int COLUMNS>
class TMatrixDataFixed
{
public:
  using Element = ELEMENT;
  using ThisType = TMatrixDataFixed<Element,ROWS,COLUMNS>;
  using MatrixColumnColumn = TMatrixFixed<Element,COLUMNS,COLUMNS>;
  using MatrixColumnRow = TMatrixFixed<Element,COLUMNS,ROWS>;
  using MatrixRowColumn = TMatrixFixed<Element,ROWS,COLUMNS>;
  using MatrixRowRow = TMatrixFixed<Element,ROWS,ROWS>;
  
  TMatrixDataFixed()
    : m_data()
  {}
  TMatrixDataFixed(const ThisType & other) 
    : m_data(other.m_data)
  {}
  TMatrixDataFixed(ThisType && other)
    : m_data(other.m_data)
  {}

  unsigned int Columns() const { return COLUMNS; }
  unsigned int Rows() const { return ROWS; }
  unsigned int Elements() const { return ROWS*COLUMNS; }
  unsigned int Index(const unsigned int row, const unsigned int column) const { return row*COLUMNS + column; }
  
  Element & operator () (const unsigned int row, const unsigned int column) { return m_data[Index(row,column)]; }
  Element operator () (const unsigned int row, const unsigned int column) const { return m_data[Index(row,column)]; }
  Element & operator () (const unsigned int index) { return m_data[index]; }
  Element operator () (const unsigned int index) const { return m_data[index]; }

protected:
  Element * Get() { return m_data.data(); }
  
private:
  std::array<Element,COLUMNS*ROWS> m_data;
};




template<typename DATA>
class TMatrixFunctions : public DATA
{
public:
  using Data = DATA;
  using Element = typename Data::Element;
  using ThisType = TMatrixFunctions<Data>;
  using MatrixColumnColumn = typename Data::MatrixColumnColumn;
  using MatrixColumnRow = typename Data::MatrixColumnRow;
  using MatrixRowColumn = typename Data::MatrixRowColumn;
  using MatrixRowRow = typename Data::MatrixRowRow;

  TMatrixFunctions() : TMatrixFunctions(0,0) {}
  TMatrixFunctions(const unsigned int rows, const unsigned int columns) : Data(rows, columns) {}
  TMatrixFunctions(const ThisType & other) : Data(other) {}
  TMatrixFunctions(ThisType && other) : Data(other) {}
  
  using Data::operator ();
  using Data::Columns;
  using Data::Rows;
  using Data::Elements;
  
  void Clear()
  {
    Fill(Element());
  }
  
  void Fill(const Element & value)
  {
    for(unsigned int i=0;i<Elements();++i)
    {
      Get()[i] = value;
    }
  }
 
  auto operator + () const { return *(MatrixRowColumn*)(this); }
  auto & operator + () { return *(MatrixRowColumn*)(this); }
  auto & operator += (const ThisType & other)
  {
    for(unsigned int i=0;i<Elements();++i)
    {
      Get()[i] += other(i);
    }
    return *(MatrixRowColumn*)(this);
  }
  auto operator + (const ThisType & other) const { return MatrixRowColumn(*this) += other; }

protected:  
  using Data::Get;  
  using Data::Index;
  
private:
};




template<typename ELEMENT>
class TMatrix : public TMatrixFunctions<TMatrixData<ELEMENT>>
{
public:
  using Functions = TMatrixFunctions<TMatrixData<ELEMENT>>;
  using Data = typename Functions::Data;
  using Element = typename Data::Element;
  using ThisType = TMatrix<Element>;
  
protected:
  friend Functions;
  TMatrix(const Functions & other) : Functions(other) {}
  TMatrix(Functions && other) : Functions(other) {}
  
public:
  TMatrix() : TMatrix(0,0) {}
  TMatrix(const unsigned int rows, const unsigned int columns) : Functions(rows, columns) {}
  TMatrix(const ThisType & other) : Functions(other) {}
  TMatrix(ThisType && other) : Functions(other) {}
  
  using Functions::operator ();
  using Functions::operator +;
  using Functions::operator +=;
private:
};




template<typename ELEMENT,unsigned int ROWS,unsigned int COLS>
class TMatrixFixed : public TMatrixFunctions<TMatrixDataFixed<ELEMENT,ROWS,COLS>>
{
public:
  using Functions = TMatrixFunctions<TMatrixDataFixed<ELEMENT,ROWS,COLS>>;
  using Data = typename Functions::Data;
  using Element = typename Data::Element;
  using ThisType = TMatrixFixed<Element,ROWS,COLS>;

protected:
  friend Functions;
  TMatrixFixed(const Functions & other) : Functions(other) {}
  TMatrixFixed(Functions && other) : Functions(other) {}
  
public:
  TMatrixFixed() : Functions() {}
  TMatrixFixed(const ThisType & other) : Functions(other) {}
  TMatrixFixed(ThisType && other) : Functions(other) {}
  
  using Functions::operator ();
  using Functions::operator +;
  using Functions::operator +=;
private:
};

