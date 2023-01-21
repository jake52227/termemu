#include "../test_runner.hpp"
#include "../../src/text_container.hpp"

void addTest1(TestRunner &tr)
{
    Test t1("test1", "the text container should not try to remove anything from an empty container", []() {
        TextContainer tc(5);
        tc.removeLast();
        shouldBeTrue(tc.getCount() == 0);
    });

    tr.add_test(t1);
}

void addTest2(TestRunner &tr)
{
    Test t2("test2", "the text container's counter should update correctly", []() {
        TextContainer tc(5);
        shouldBeTrue(tc.getCount() == 0);
        tc.store(std::string("hello"));
        shouldBeTrue(tc.getCount() == 1);
        tc.store(std::string("world"));
        shouldBeTrue(tc.getCount() == 2);
    });

    tr.add_test(t2);
}

int main(void)
{
    TestRunner tr;
    addTest1(tr);
    addTest2(tr);
    tr.run_tests();
}