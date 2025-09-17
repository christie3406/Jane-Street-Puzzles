#pragma once
#include <cstdint>
#include <vector>
#include <functional>
#include <unordered_set>
#include <cmath>

bool square(int64_t n);
bool prime(int64_t n);
bool fibonacci(int64_t n);
bool product_of_digits_is_m(int64_t n, int64_t m);
bool divisible_by_every_digit(int64_t n);
bool multiple_of_m(int64_t n, int64_t m);
bool odd_palindrome(int64_t n);

extern std::vector<std::function<bool(int64_t)>> functions;