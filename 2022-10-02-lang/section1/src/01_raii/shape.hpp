enum class shape_type {
    circle,
    triangle,
    rectangle,
};

class shape{};
class circle: public shape{};
class triangle: public shape{};
class rectangle: public shape{};

shape* create_shape(shape_type type)
{
    switch (type)
    {
    case shape_type::circle:
        return new circle();
    case shape_type::triangle:
        return new triangle();
    case shape_type::rectangle:
        return new rectangle();
    }
    return nullptr;
}

class shape_wrapper
{
public:
    explicit shape_wrapper(shape* ptr = nullptr ):ptr_(ptr){}
    ~shape_wrapper(){
        delete ptr_;
    }
    shape* get()const{ return ptr_; }
private:
    shape* ptr_;
};

