#include "../test_runner.hpp"
#include "../../src/parser.hpp"

void addTest1(TestRunner &tr)
{
    Test t1("test1", "code length should equal 4", []() {
        Parser p;
        AnsiCode code;
        std::string text = "\x1b[0mABC";
        const auto b = text.cbegin();
        const auto e = text.cend();
        p.parseCode(code, b, e);
        shouldBeTrue(code.length == 4);
    });

    tr.add_test(t1);
}

void addTest2(TestRunner &tr)
{
    Test t2("test2", "Foreground color should be green", []() {
        Parser p;
        AnsiCode code;
        std::string text = "\x1b[32mABC";
        const auto b = text.cbegin();
        const auto e = text.cend();
        p.parseCode(code, b, e);
        shouldBeTrue(code.fgColor == GREEN);
    });

    tr.add_test(t2);
}

void addTest3(TestRunner &tr)
{
    Test t3("test3", "Background color should be red", []() {
        Parser p;
        AnsiCode code;
        std::string text = "\x1b[41mABC";
        const auto b = text.cbegin();
        const auto e = text.cend();
        p.parseCode(code, b, e);
        shouldBeTrue(code.bgColor == RED);

    });

    tr.add_test(t3);
}

void addTest4(TestRunner &tr)
{
    Test t4("test4", "Background color should be red and foreground color should be green", []() {
        Parser p;
        AnsiCode code;
        std::string text = "\x1b[41;32mABC";
        const auto b = text.cbegin();
        const auto e = text.cend();
        p.parseCode(code, b, e);
        shouldBeTrue(code.bgColor == RED && code.fgColor == GREEN);
    });

    tr.add_test(t4);
}

void addTest5(TestRunner &tr)
{
    Test t5("test5", "Parsed private mode code should be the correct length", []() {
        Parser p;
        AnsiCode code;
        std::string text = "\x1b[?2004hABCD";
        const auto b = text.cbegin();
        const auto e = text.cend();
        p.parseCode(code, b, e);
        shouldBeTrue(code.length == 8);
    });

    tr.add_test(t5);
}

void addTest6(TestRunner &tr)
{
    Test t6("test6", "The length of sequential escape codes should be correct", []() {
        Parser p;
        AnsiCode code;
        std::string text = "\x1b[41;32m\x1b[0mABCDEFG";
        const auto b = text.cbegin();
        const auto e = text.cend();
        p.parseCode(code, b, e);
        shouldBeTrue(code.length == 12);
    });
    tr.add_test(t6);
}

int main()
{
    TestRunner tr;
    addTest1(tr);
    addTest2(tr);    
    addTest3(tr);    
    addTest4(tr);    
    addTest5(tr);    
    addTest6(tr);    
    tr.run_tests();
}
