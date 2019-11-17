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
    TMatrix(const Functions& other) : Functions(other) {}
    TMatrix(Functions&& other) : Functions(other) {}

protected:
    friend Data;
    using Functions::Instance;

public:
    TMatrix() : TMatrix(0, 0) {}
    TMatrix(const unsigned int rows, const unsigned int columns) : Functions(rows, columns) {}
    TMatrix(const unsigned int rows, const unsigned int columns, const Element& def) : Functions(rows, columns, def) {}
    TMatrix(const ThisType& other) : Functions(other) {}
    TMatrix(ThisType&& other) : Functions(other) {}

    using Functions::Columns;
    using Functions::Rows;
    using Functions::Elements;
    using Functions::operator ();
    using Functions::operator +;
    using Functions::operator +=;
    using Functions::operator -;
    using Functions::operator -=;
private:
};

