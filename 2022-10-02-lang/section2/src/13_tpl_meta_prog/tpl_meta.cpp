#include<stdio.h>

template<bool cond, typename Then, typename Else>
struct If;

template<typename Then, typename Else>
struct If<true, Then, Else> {
    typedef Then type;
};

template<typename Then, typename Else>
struct If<false, Then, Else> {
    typedef Else type;
};

template<bool condition, typename Body>
struct WhileLoop;

template<typename Body>
struct WhileLoop<true, Body>{
    typedef typename WhileLoop<Body::cond_value,  typename Body::next_type>::type type;
};

template<typename Body>
struct WhileLoop<false, Body>{
    typedef typename Body::res_type type;
};

template<typename Body>
struct While{
    typedef typename WhileLoop<Body::cond_value,  typename Body::next_type>::type type;
};

template <class T, T v>
struct integral_constant {
    static const T value = v;
    typedef T value_type;
    typedef integral_constant type;
};

template <int result, int n>
struct SumLoop
{
    static  const bool cond_value = n != 0;
    static const int res_value = result;
    typedef integral_constant<int, res_value> res_type;
    typedef SumLoop<result+n, n-1> next_type;
};

template <int n>
struct Sum
{
    typedef SumLoop<0, n> type;
};

int main() {
    printf("%d\n", While<Sum<10>::type>::type::value);
    // While<SumLoop<0, 10>>::type::value
    // WhileLoop<true,  SumLoop<10, 9>>::type::value
    // WhileLoop<true,  SumLoop<19, 8>>::type::value
    // WhileLoop<false,  SumLoop<55, 0>>::type::value
    // SumLoop<55, 0>::res_type::value
    // integral_constant<int, 55>::value
    // 55
}