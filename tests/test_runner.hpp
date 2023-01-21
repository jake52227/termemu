#ifndef TEST_RUNNER_HPP
#define TEST_RUNNER_HPP

#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <functional>

#define TESTGREEN "\x1b[32m"
#define TESTBLUE "\x1b[34m"
#define TESTRED "\x1b[31m"
#define TESTRESET "\x1b[0m"

struct TestFail : public std::exception
{
    const char *what()
    {
        return "Test assertion failed";
    }
};

void shouldBeTrue(bool expression);

class Test
{
public:
    Test(const char *name, const char *description, std::function<void(void)> tf);
    void run();
    void beforeTest();
    void declareSuccess();
    void declareFailure(const char *msg);
private:
    std::function<void(void)> testFunction;
    const char *description;
    const char *name;
};

class TestRunner
{
public:
    void add_test(Test tf);
    void run_tests();
private:
    std::vector<Test> tests;
};

#endif