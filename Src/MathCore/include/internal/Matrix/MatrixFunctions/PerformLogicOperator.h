    //
    // Perform logic operator on each element of this
    //
    template<typename F>
    auto PerformLogicOperator(const F& f) const
    {
        auto res = base.InstanceLogic();
        for (unsigned int index = 0; index < base.Elements(); ++index)
        {
            res(index) = f(base(index));
        }
        return res;
    }
    //
    // Perform logic operator on each element of this and other
    //
    template<typename M, typename F, typename std::enable_if<is_matrix<M>::value, int>::type = 0>
    auto PerformLogicOperator(const M& other, const F& f) const
    {
        base.assert_size(other);
        auto res = base.InstanceLogic();
        for (unsigned int index = 0; index < base.Elements(); ++index)
        {
            res(index) = f(base(index), other.base(index));
        }
        return res;
    }
