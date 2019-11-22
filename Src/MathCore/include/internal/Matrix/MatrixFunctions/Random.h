    // 
    // Fill with random values [min,max)
    //
    auto& Random(const Element& minValue,const Element& maxValue)
    {
        const Element range = maxValue - minValue;
        return PerformOperator([&minValue,&range](const Element&) {return minValue + Numerics::NormalizedRandomNumber(range); });
    }
