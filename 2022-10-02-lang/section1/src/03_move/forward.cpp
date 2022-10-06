#include<stdio.h>
#include <utility>

class shape{};
class circle: public shape {};

void foo(const shape&)
{
    puts("foo(const shape&)");
}

void foo(shape&&)
{
    puts("foo(shape&&)");
}

void bar(const shape& s)
{
    puts("bar(const shape&)");
    foo(s);
}

template <typename T>
void bar(T&& s)
{
    foo(std::forward<T>(s));
}

int main()
{
    circle temp;
    bar(temp);
    bar(circle());
}