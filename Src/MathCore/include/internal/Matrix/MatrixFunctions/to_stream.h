#pragma once

template<typename ELEMENT, typename MATRIX>
std::ostream& TMatrixFunctions<ELEMENT, MATRIX>::to_stream(std::ostream& s) const
{
    s << "[";
    auto StreamRow = [&](const unsigned int row)
    {
        s << base(row,0);
        for (unsigned int column = 1;column<base.Columns();++column)
        {
            s << "," << base(row,column);
        }
    };
    if(base.Elements()>0)
    {
        StreamRow(0);
        for (unsigned int row = 1;row<base.Rows();++row)
        {
            s << ";";
            StreamRow(row);
        }
    }
    s << "]";
    return s;
}

template<typename ELEMENT, unsigned int ROWS, unsigned int COLUMNS>
std::ostream& operator << (std::ostream& s, const TMatrix<ELEMENT,ROWS,COLUMNS>& m)
{
    return m.to_stream(s);
}
