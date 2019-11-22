    //
    // Perform operator on each element of this
    //
    template<typename F>
    auto& PerformOperator(const F& f)
    {
        for (unsigned int index = 0; index < base.Elements(); ++index)
        {
            base(index) = f(base(index));
        }
        return base;
    }
    //
    // Perform operator on each element of this and other
    //
    template<typename M, typename F, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto& PerformOperator(const M& other, const F& f)
    {
        base.assert_size(other);
        for (unsigned int index = 0; index < base.Elements(); ++index)
        {
            base(index) = f(base(index), other.base(index));
        }
        return base;
    }
