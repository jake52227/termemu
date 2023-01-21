#include "test_runner.hpp"

void shouldBeTrue(bool expression)
{
    if (!expression)
        throw TestFail();
}

Test::Test(const char *name, const char *description, std::function<void(void)> tf)
{
    this->description = description;
    this->name = name;
    this->testFunction = tf;
}

void Test::run()
{
    this->testFunction();
}

void Test::beforeTest()
{
    std::cout << TESTBLUE << this->name << TESTRESET << ": " << description << std::endl;
}

void Test::declareSuccess()
{
    std::cout << TESTBLUE << this->name << TESTRESET << TESTGREEN << ": Success\n" << TESTRESET << std::endl;
}

void Test::declareFailure(const char *msg)
{
    std::cout << TESTBLUE << this->name << TESTRESET << TESTRED << ": Fail: " << TESTRESET << msg << std::endl;
}


void TestRunner::add_test(Test tf)
{
    this->tests.push_back(tf);
}

void TestRunner::run_tests()
{
    int sz = this->tests.size();
    for (int i = 0; i < sz; ++i)
    {
        Test t = this->tests.at(i);
        try
        {
            t.beforeTest();
            t.run();
            t.declareSuccess();
        }
        catch (const std::exception &e)
        {
            t.declareFailure(e.what());
        }
    }
}