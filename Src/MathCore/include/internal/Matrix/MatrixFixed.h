template<typename ELEMENT, unsigned int ROWS, unsigned int COLS>
class TMatrixFixed : public TMatrixFunctions<TMatrixDataFixed<ELEMENT, ROWS, COLS>>
{
public:
    using Functions = TMatrixFunctions<TMatrixDataFixed<ELEMENT, ROWS, COLS>>;
    using Data = typename Functions::Data;
    using Element = typename Data::Element;
    using ThisType = TMatrixFixed<Element, ROWS, COLS>;

protected:
    friend Functions;
    TMatrixFixed(const Functions& other) : Functions(other) {}
    TMatrixFixed(Functions&& other) : Functions(other) {}

protected:
    friend Data;
    using Functions::Instance;

public:
    TMatrixFixed() : Functions() {}
    TMatrixFixed(const ThisType& other) : Functions(other) {}
    TMatrixFixed(ThisType&& other) : Functions(other) {}

    using Functions::Columns;
    using Functions::Rows;
    using Functions::Elements;
    using Functions::operator ();
    using Functions::operator +;
    using Functions::operator +=;
    using Functions::operator -;
    using Functions::operator -=;
    using Functions::operator *;
    using Functions::operator *=;
private:
};

