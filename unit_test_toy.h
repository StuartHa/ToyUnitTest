#ifndef UNIT_TEST_TOY_H
#define UNIT_TEST_TOY_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "rang.hpp"

#define REQUIRE3(predicate_expr, file, line) \
    if (!(predicate_expr)) { \
        current_test_case->failed_assertion = std::make_shared<FailedAssertion>(); \
        current_test_case->failed_assertion->file_name = file; \
        current_test_case->failed_assertion->line_number = line; \
        current_test_case->failed_assertion->expression = #predicate_expr; \
        return; \
    } \
    current_test_case->assertions_run++
#define REQUIRE2(predicate_expr, file, line) REQUIRE3(predicate_expr, file, line)
#define REQUIRE(predicate_expr) REQUIRE2(predicate_expr, __FILE__, __LINE__)

#define UNIQUE_NAME3(base, suffix) base ## suffix
#define UNIQUE_NAME2(base, suffix) UNIQUE_NAME3(base, suffix)
#define UNIQUE_NAME(base) UNIQUE_NAME2(base, __LINE__)

#define TEST_CASE3(name, file, line) \
    void UNIQUE_NAME(stu_test_)(); AutoRegister UNIQUE_NAME(unused_)(name, UNIQUE_NAME(stu_test_), file, line); void UNIQUE_NAME(stu_test_)()
#define TEST_CASE2(name, file, line) TEST_CASE3(name, file, line)
#define TEST_CASE(name) TEST_CASE2(name, __FILE__, __LINE__)

using VoidFnPtr = void (*)();

// Contains information about an assertion that failed.
struct FailedAssertion {
    std::string file_name;
    int line_number;
    std::string expression;
};

// Represents a single test case, including the information needed to run it and its results.
struct TestCase {
    TestCase(std::string name, VoidFnPtr fn, std::string file, int line) : name(std::move(name)), run(fn),
                                                                           file_name(std::move(file)),
                                                                           line_number(line) {}

    std::string name;
    std::string file_name;
    int line_number = 0;
    VoidFnPtr run = nullptr;
    int assertions_run = 0;
    // Only a single failed assertion is stored since execution is halted at first failure.
    std::shared_ptr<FailedAssertion> failed_assertion;
};

static std::shared_ptr<TestCase> current_test_case;
static std::vector<std::shared_ptr<TestCase>> test_cases;

// Used to register test case functions.
class AutoRegister {
public:
    AutoRegister(std::string name, VoidFnPtr run, std::string file_name, int line_number) noexcept {
        test_cases.push_back(std::make_shared<TestCase>(std::move(name), run, std::move(file_name), line_number));
    }
};

// Reports test results to terminal console.
class ConsoleReporter {
public:
    explicit ConsoleReporter(std::ostream &out) : out(out) {
        // Required for colors to work in Clion context.
        rang::setControlMode(rang::control::Force);
    }

    void WriteTestCaseResults(const TestCase &test_case) {
        if (!test_case.failed_assertion) {
            return;
        }

        WriteSeparator('-');
        out << test_case.name << '\n';
        WriteSeparator('-');
        out << test_case.file_name << ":" << test_case.line_number << '\n';
        WriteSeparator('.');
        out << test_case.failed_assertion->file_name << ":" << test_case.failed_assertion->line_number << ": "
            << rang::fgB::red << rang::style::bold << "FAILED:" << rang::fg::reset << rang::style::reset << '\n';
        out << '\t' << rang::fg::cyan << "REQUIRE(" << test_case.failed_assertion->expression << ')' << rang::fg::reset
            << '\n';
        out << '\n';
    };

    template<typename InputIt>
    void WriteSummary(InputIt start, InputIt end) {
        int test_case_count = 0;
        int assertion_count = 0;
        int failed_assertion_count = 0;
        while (start != end) {
            test_case_count++;
            if ((*start)->failed_assertion) {
                failed_assertion_count++;
            }
            assertion_count += (*start)->assertions_run;
            start++;
        }

        if (failed_assertion_count == 0) {
            out << "All tests passed\n\n";
        }

        WriteSeparator('=');
        out << "test cases: " << test_case_count << " | " << (test_case_count - failed_assertion_count) << " passed | "
            << failed_assertion_count << " failed\n";
        out << "assertions: " << assertion_count << " | " << (assertion_count - failed_assertion_count) << " passed | "
            << failed_assertion_count << " failed\n";
    }

private:
    // Writes out a terminal-width separator using input character.
    void WriteSeparator(char ch) {
        static const int kSeparatorWidth = 79;
        out << std::string(kSeparatorWidth, ch) << '\n';
    }

    std::ostream &out;
};

int main() {
    ConsoleReporter reporter(std::cout);
    for (auto &test_case : test_cases) {
        current_test_case = test_case;
        test_case->run();
        reporter.WriteTestCaseResults(*test_case);
    }
    reporter.WriteSummary(test_cases.begin(), test_cases.end());
    return std::any_of(test_cases.begin(), test_cases.end(), [](auto in) { return in->failed_assertion; });
}

#endif //UNIT_TEST_TOY_H
