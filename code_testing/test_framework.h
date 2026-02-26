// test_framework.h
// Simple, lightweight testing framework with no external dependencies
// Provides macros for unit testing with minimal overhead

#ifndef BLEC_TEST_FRAMEWORK_H
#define BLEC_TEST_FRAMEWORK_H

#include <cstdio>
#include <cstring>

namespace blec {
namespace test {

// Test statistics
struct TestStats {
    int total = 0;
    int passed = 0;
    int failed = 0;
};

static TestStats g_test_stats;

// Test result reporting
inline void ReportTest(const char* test_name, bool passed, const char* file, int line) {
    g_test_stats.total++;
    if (passed) {
        g_test_stats.passed++;
        std::printf("  [PASS] %s\n", test_name);
    } else {
        g_test_stats.failed++;
        std::printf("  [FAIL] %s at %s:%d\n", test_name, file, line);
    }
}

// Print final test summary
inline void PrintSummary() {
    std::printf("\n==============================\n");
    std::printf("Test Summary:\n");
    std::printf("  Total:  %d\n", g_test_stats.total);
    std::printf("  Passed: %d\n", g_test_stats.passed);
    std::printf("  Failed: %d\n", g_test_stats.failed);
    std::printf("==============================\n");

    if (g_test_stats.failed == 0) {
        std::printf("All tests passed!\n\n");
    } else {
        std::printf("Some tests failed.\n\n");
    }
}

// Reset test statistics
inline void ResetStats() {
    g_test_stats.total = 0;
    g_test_stats.passed = 0;
    g_test_stats.failed = 0;
}

// Get test result
inline bool AllTestsPassed() {
    return g_test_stats.failed == 0;
}

} // namespace test
} // namespace blec

// Test macros
#define TEST_CASE(name) \
    void name(); \
    namespace { \
        struct name##_runner { \
            name##_runner() { std::printf("\nRunning: %s\n", #name); name(); } \
        } name##_instance; \
    } \
    void name()

#define ASSERT_TRUE(expr) \
    blec::test::ReportTest(#expr, (expr), __FILE__, __LINE__)

#define ASSERT_FALSE(expr) \
    blec::test::ReportTest("!" #expr, !(expr), __FILE__, __LINE__)

#define ASSERT_EQ(a, b) \
    blec::test::ReportTest(#a " == " #b, ((a) == (b)), __FILE__, __LINE__)

#define ASSERT_NE(a, b) \
    blec::test::ReportTest(#a " != " #b, ((a) != (b)), __FILE__, __LINE__)

#define ASSERT_LT(a, b) \
    blec::test::ReportTest(#a " < " #b, ((a) < (b)), __FILE__, __LINE__)

#define ASSERT_GT(a, b) \
    blec::test::ReportTest(#a " > " #b, ((a) > (b)), __FILE__, __LINE__)

#define ASSERT_LE(a, b) \
    blec::test::ReportTest(#a " <= " #b, ((a) <= (b)), __FILE__, __LINE__)

#define ASSERT_GE(a, b) \
    blec::test::ReportTest(#a " >= " #b, ((a) >= (b)), __FILE__, __LINE__)

#define ASSERT_STREQ(a, b) \
    blec::test::ReportTest(#a " == " #b, (std::strcmp((a), (b)) == 0), __FILE__, __LINE__)

#define ASSERT_NULL(ptr) \
    blec::test::ReportTest(#ptr " == nullptr", ((ptr) == nullptr), __FILE__, __LINE__)

#define ASSERT_NOT_NULL(ptr) \
    blec::test::ReportTest(#ptr " != nullptr", ((ptr) != nullptr), __FILE__, __LINE__)

#define TEST_MAIN() \
    int main() { \
        std::printf("============================\n"); \
        std::printf("Running B-Lec Unit Tests\n"); \
        std::printf("============================\n"); \
        blec::test::PrintSummary(); \
        return blec::test::AllTestsPassed() ? 0 : 1; \
    }

#endif // BLEC_TEST_FRAMEWORK_H
