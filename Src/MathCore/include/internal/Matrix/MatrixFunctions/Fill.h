    //
    // Fill
    //
    auto& Fill(const Element& value)
    {
        return PerformOperator([&value](const Element&) {return value; });
    }
