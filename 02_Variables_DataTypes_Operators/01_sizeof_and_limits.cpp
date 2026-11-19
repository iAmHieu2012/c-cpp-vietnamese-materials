#include <climits>
#include <iostream>

int main()
{
    std::cout << "size of datatype:" << std::endl;
    std::cout << "char: " << sizeof(char) << " (bytes)" << std::endl;
    std::cout << "bool: " << sizeof(bool) << " (bytes)" << std::endl;
    std::cout << "short: " << sizeof(short) << " (bytes)" << std::endl;
    std::cout << "int: " << sizeof(int) << " (bytes)" << std::endl;
    std::cout << "long: " << sizeof(long) << " (bytes)" << std::endl;
    std::cout << "long long: " << sizeof(long long) << " (bytes)" << std::endl;
    std::cout << "unsigned int: " << sizeof(unsigned int) << " (bytes)" << std::endl;
    std::cout << "float: " << sizeof(float) << " (bytes)" << std::endl;
    std::cout << "double: " << sizeof(double) << " (bytes)" << std::endl;
    std::cout << "size_t: " << sizeof(size_t) << " (bytes)" << std::endl;

    std::cout << "INT_MAX = " << INT_MAX << std::endl;
    // std::cout << "INT_MAX + 1 = " << INT_MAX + 1 << std::endl; #warning
    std::cout << "INT_MIN = " << INT_MIN << std::endl;
    // std::cout << "INT_MIN - 1 = " << INT_MIN - 1 << std::endl; #warning
    int max_int = INT_MAX, min_int = INT_MIN, overflow_int_max = max_int + 1, overflow_int_min = min_int - 1;
    std::cout << "INT_MAX + 1 = " << overflow_int_max << std::endl;
    std::cout << "INT_MIN - 1 = " << overflow_int_min << std::endl;

    unsigned int u_zero = 0;
    unsigned int overflow_uint_zero = u_zero - 1;
    std::cout << "uint 0 - 1 = " << overflow_uint_zero << std::endl;

    float f = 0.1f+ 0.2f;
    std::cout << "float: 0.1 + 0.2 == 0.3 ?" << (f == 0.3f ?  "True" : "False") << std::endl; 
    double g = 0.1 + 0.2;
    std::cout << "double: 0.1 + 0.2 == 0.3 ?" << (g == 0.3 ?  "True" : "False") << std::endl; 
    return 0;
}