#include "clues.hpp"


// CLUES FOR EACH ROW
bool square(int64_t n){
 int64_t root = int64_t(std::sqrt(n));
 return (root * root == n);
}

bool product_of_digits_is_m(int64_t n, int64_t m){
 int64_t product = 1;
 while (n != 0) {
  int digit = n % 10; 
  product *= digit; 
  n /= 10;
 }
 return (product == m);
}

bool multiple_of_m(int64_t n, int64_t m){
 return (n % m == 0);
}

bool divisible_by_every_digit(int64_t n){
 int64_t original = n;
 int64_t temp = original;
 while (temp != 0){
  int digit = temp % 10;
  if (digit == 0 || original % digit != 0) return false;
  temp /= 10;
 }
 return true;
}

bool odd_palindrome(int64_t n){
if (n % 2 == 0) return 0;
 int64_t original = n;
 int64_t reversed = 0;
 while (n > 0) {
  reversed = reversed * 10 + n % 10;
  n /= 10;
 }
 return (original == reversed);
}

bool fibonacci(int64_t n) {
    std::unordered_set<int64_t> fibonacci_numbers = { // up to 5 digits 
        13, 21, 34, 55, 89, 
        144, 233, 377, 610, 987, 
        1597, 2584, 4181, 6765,
        10946,  17711, 28657, 46368, 75025
    };
    if (fibonacci_numbers.find(n) != fibonacci_numbers.end()) return true; 
    else return false;
}

bool prime(int64_t n){
 if (n == 2) return true;
 if (n % 2 == 0) return false;
 for (int64_t i = 3; i <= int64_t(std::sqrt(n)); i += 2) if (n % i == 0) return false;
 return true;
}

// functions[r](n) returns whether the number n fulfills the clue of row r
std::vector<std::function<bool(int64_t)>> functions = {
    [](int64_t n) { return square(n); },
    [](int64_t n) { return product_of_digits_is_m(n, 20); },
    [](int64_t n) { return multiple_of_m(n, 13); },
    [](int64_t n) { return multiple_of_m(n, 32); },
    [](int64_t n) { return divisible_by_every_digit(n); },
    [](int64_t n) { return product_of_digits_is_m(n, 25); },
    [](int64_t n) { return divisible_by_every_digit(n); },
    [](int64_t n) { return odd_palindrome(n); },
    [](int64_t n) { return fibonacci(n); },
    [](int64_t n) { return product_of_digits_is_m(n, 2025); },
    [](int64_t n) { return prime(n); }
}; 
