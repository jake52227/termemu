#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "../src/parser.hpp"

#define success(msg) do {std::cout << msg << ": \x1b[32mSuccess\x1b[0m" << std::endl;} while(0);

typedef void (*test_func)(void);

void test1() {
    Parser p;
    std::string text = "\x1b[0mABC";
    struct ParsedText ps = p.parse(text);
    assert(ps.text.compare("ABC") == 0);
    success("test1");
}

void test2() {
    Parser p;
    std::string text = "\x1b[0;34;5mABC";
    struct ParsedText ps = p.parse(text);
    assert(ps.text.compare("ABC") == 0);
    success("test2");   
}

void test3() {
    Parser p;
    std::string text = "\x1b[0;34;5mAB\x1b[0mC";
    struct ParsedText ps = p.parse(text);
    assert(ps.text.compare("ABC") == 0);
    success("test3");   
}

void add_tests(std::vector<test_func> &tests) {
    tests.push_back(*test1);
    tests.push_back(*test2);
    tests.push_back(*test3);
}


void run_tests(const std::vector<test_func> &tests) {
    for (int i = 0; i < tests.size(); ++i) {
        tests.at(i)();
    }
}


int main() {
    std::vector<test_func> tests;
    add_tests(tests);
    run_tests(tests); 
}
