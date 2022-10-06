#include<type_traits>

template<typename C, typename T>
void _append(C& container, T* ptr, std::true_type)
{
    container.reserve(container.size() + size);
    for (size_t i=0; i<size; ++i)
    {
        container.push_back(ptr[i]);
    }
}

template<typename C, typename T>
void _append(C& container, T* ptr, std::false_type)
{
    for (size_t i=0; i<size; ++i)
    {
        container.push_back(ptr[i]);
    }
}

template<typename C, typename T>
void append(C& container, T* ptr, std::size_t size)
{
    _append(container, ptr, size, integral_constant<bool, has_reserve<C>::value{});
}