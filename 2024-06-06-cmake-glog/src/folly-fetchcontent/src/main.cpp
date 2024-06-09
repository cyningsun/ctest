#include <iostream>
#include <folly/Format.h>

int main(int argc, char *argv[]) {
    std::string s1 = fmt::format("The answer to {} is {}", "life", 42);

    std::string s2 = folly::sformat("{0}{0}{0}{0} Batman!", "na");
    std::cout << "Hello, World!" << std::endl;
    return 0;
}