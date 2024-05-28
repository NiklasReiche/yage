namespace utils
{
    /**
     * Inherits from multiple lambdas and exposes their operator(). Can be used to supply std::visit with in-place
     * lambdas for each variant.
     * @tparam Ts a lambda for each visit overload
     */
    template<class... Ts>
    struct overload : Ts ...
    {
        using Ts::operator()...;
    };
}
