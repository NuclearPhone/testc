#include "test.h"

const char* passing_test_function() {
    return NULL;
}

const char* failing_test_function() {
    return "this is a fail";
}

static const test_t passing_test = {
    .ptr = passing_test_function,
    .name = "passing test",
    .desc = "",
};
static const test_t failing_test = {
    .ptr = failing_test_function,
    .name = "failing test",
    .desc = "",
};

extern int main() {
    const bool pass = execute_test(passing_test);
    const bool fail = execute_test(failing_test);
    if (!pass)
        printf("testc failed the test for the passing function\n");
    if (fail)
        printf("testc failed the test for the failing function\n");
}
