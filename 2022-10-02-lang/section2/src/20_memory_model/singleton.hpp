#include<mutex>
#include<atomic>

class singleton
{
private:
    static std::mutex lock_;
    static std::atomic<singleton*> inst_ptr_;
public:
    singleton* instance(/* args */);
    ~singleton();
};
