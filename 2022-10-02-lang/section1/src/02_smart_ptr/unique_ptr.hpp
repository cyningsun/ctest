template <typename T>
class unique_ptr
{
public:
    explicit unique_ptr(T* ptr = nullptr ):ptr_(ptr){}

    template <typename U>
    unique_ptr(unique_ptr<U>&& other)
    {
        ptr_ = other.release();
    }

    unique_ptr& operator=(unique_ptr rhs)
    {
        unique_ptr(rhs).swap(*this);
        return *this;
    }

    T* release()
    {
        T* ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

    void swap(unique_ptr& rhs)
    {
        using std::swap;
        swap(ptr_, rhs.ptr_);
    }

    ~unique_ptr(){
        delete ptr_;
    }

    T* get()const{ return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
private:
    T* ptr_;
};