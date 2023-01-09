#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "../src/parser.hpp"

#define TESTGREEN "\x1b[32m"
#define TESTBLUE "\x1b[34m"
#define TESTRESET "\x1b[0m"

#define describe(testname, msg) do { \
	std::cout << TESTBLUE << testname << TESTRESET << ": " << msg << std::endl; \
} while (0);

#define success(testname) do { \
    std::cout << TESTBLUE << testname << TESTRESET << TESTGREEN << ": Success" << TESTRESET << std::endl; \
} while(0);

typedef void (*test_func)(void);

void test1() {
    describe("test1", "code length should equal 4");
    Parser p;
    AnsiCode code;
    std::string text = "\x1b[0mABC";
    const auto b = text.cbegin();
    const auto e = text.cend();
    p.parseCode(code, b, e);
    assert(code.length == 4);
    success("test1");
}

void test2() {
    describe("test2", "Foreground color should be green");
    Parser p;
    AnsiCode code;
    std::string text = "\x1b[32mABC";
    const auto b = text.cbegin();
    const auto e = text.cend();
    p.parseCode(code, b, e);
    assert(code.fgColor == GREEN);
    success("test2");
}

void test3() {
    describe("test3", "Background color should be red");
    Parser p;
    AnsiCode code;
    std::string text = "\x1b[41mABC";
    const auto b = text.cbegin();
    const auto e = text.cend();
    p.parseCode(code, b, e);
    assert(code.bgColor == RED);
    success("test3");
}

void test4() {
    describe("test4", "Background color should be red and foreground color should be green");
    Parser p;
    AnsiCode code;
    std::string text = "\x1b[41;32mABC";
    const auto b = text.cbegin();
    const auto e = text.cend();
    p.parseCode(code, b, e);
    assert(code.bgColor == RED && code.fgColor == GREEN);
    success("test4");
}

void test5() {
    describe("test5", "Parsed private mode code should be the correct length");
    Parser p;
    AnsiCode code;
    std::string text = "\x1b[?2004hABCD";
    const auto b = text.cbegin();
    const auto e = text.cend();
    p.parseCode(code, b, e);
    assert(code.length == 8);
    success("test5");
}

void add_tests(std::vector<test_func> &tests) {
    tests.push_back(*test1);
    tests.push_back(*test2);
    tests.push_back(*test3);
    tests.push_back(*test4);
    tests.push_back(*test5);
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
