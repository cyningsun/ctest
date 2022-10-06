#include <cln/cln.h>

int my_gcd(int a, int b)
{
    while (b !=0)
    {
        int r = a%b;
        a = b;
        b = r;
    }
    return a;
}

template <typename E>
E my_gcd(E a, E b)
{
    while (b !=0)
    {
        E r = my_mod(a,b);
        a = b;
        b = r;
    }
    return a;
}

template <typename E>
E my_mod(const E& lhs, const E& rhs)
{
    return lhs % rhs;
}


cln::cl_I my_mod(const cln::cl_I& lhs, const cln::cl_I& rhs)
{
    return mod(lhs, rhs);
}

template<>
cln::cl_I my_mod<cln::cl_I>(const cln::cl_I& lhs, const cln::cl_I& rhs)
{
    return mod(lhs, rhs);
}
