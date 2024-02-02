#ifndef _HARNESS_HPP
#define _HARNESS_HPP

#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::string __err_msg_w_line_no(int line)
{
    std::stringstream ss;
    ss << "Failed assertion on line " << line;
    return ss.str();
}

#define INIT_HARNESS \
    static size_t __tested_ = 0; \
    static size_t __passed_ = 0; \
    static size_t __failed_ = 0; \
    static std::vector<std::function<void (std::string&)>> __tests_; \
    \
    void __run_test(std::function<void (std::string&)> test) \
    { \
        std::string name; \
        __tested_++; \
        try \
        { \
            test(name); \
            __passed_++; \
            std::cout << "[ PASSED ]......" << name << '\n'; \
        } \
        catch(const std::exception& e) \
        { \
            __failed_++; \
            std::cerr << "[ FAILED ]......" << name << "......" << e.what() << '\n'; \
        } \
    } \
    \
    int __add_test(std::function<void (std::string&)> test) \
    { \
        __tests_.push_back(test); \
        return 0; \
    }

/// @brief Creates a test function with the name given
#define TEST(test_name) \
    void __ ## test_name ## _internal(); \
    void test_name(std::string& name) \
    { \
        name = __func__; \
        __ ## test_name ## _internal(); \
    } \
    int __add_ ## test_name = __add_test(test_name); \
    void __ ## test_name ## _internal()

/// @brief Throws a `std::runtime_error` if the values aren't equal
#define ASSERT_EQ(expected, actual) \
    (expected) == (actual) ? 0 : throw std::runtime_error(__err_msg_w_line_no(__LINE__));

/// @brief Throws a `std::runtime_error` if the values are equal
#define ASSERT_NE(expected, actual) \
    (expected) != (actual) ? 0 : throw std::runtime_error(__err_msg_w_line_no(__LINE__));

/// @brief Throws a `std::runtime_error` if the given expression is false
#define ASSERT(expression) \
    (expression) ? 0 : throw std::runtime_error(__err_msg_w_line_no(__LINE__));

/// @brief Auto-generates `main` function to perform all unit tests.
#define RUN_TESTS \
int main() \
{ \
    for (auto test: __tests_) \
    { \
        __run_test(test); \
    } \
    std::cout << "[ PASSED: " << __passed_ << "/" << __tested_ << " ]" << ((__passed_ == __tested_) ? " :)" : "") << '\n'; \
    std::cout << "[ FAILED: " << __failed_ << "/" << __tested_ << " ]" << '\n'; \
    return 0; \
}

#endif