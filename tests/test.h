#define DYNAMICO_LOGGING
#include "../dynamico.h"

#define DYNAMICO_TEST(name)                                                    \
    const char *TEST_NAME = name;                                              \
    fprintf(stderr,                                                            \
            DYNAMICO_TERMINAL_YELLOW                                           \
            "TEST " DYNAMICO_TERMINAL_GREEN "%s" DYNAMICO_TERMINAL_RESET       \
            " (" DYNAMICO_TERMINAL_BLUE "%s" DYNAMICO_TERMINAL_RESET           \
            "): %s\n",                                                         \
            TEST_NAME, __func__, __FILE__)

#define DYNAMICO_TEST_PASS(name)                                               \
    fprintf(stderr,                                                            \
            DYNAMICO_TERMINAL_YELLOW                                           \
            "TEST " DYNAMICO_TERMINAL_GREEN "%s" DYNAMICO_TERMINAL_RESET       \
            " (" DYNAMICO_TERMINAL_BLUE "%s" DYNAMICO_TERMINAL_RESET           \
            "): %s " DYNAMICO_TERMINAL_GREEN "PASSED!" DYNAMICO_TERMINAL_RESET \
            "\n",                                                              \
            TEST_NAME, __func__, __FILE__)

#define DYNAMICO_TEST_FAIL()                                                   \
    fprintf(stderr,                                                            \
            DYNAMICO_TERMINAL_YELLOW                                           \
            "TEST " DYNAMICO_TERMINAL_GREEN "%s" DYNAMICO_TERMINAL_RESET       \
            " (" DYNAMICO_TERMINAL_BLUE "%s" DYNAMICO_TERMINAL_RESET           \
            "): %s " DYNAMICO_TERMINAL_RED "FAILED!" DYNAMICO_TERMINAL_RESET   \
            "\n",                                                              \
            TEST_NAME, __func__, __FILE__)

#define DYNAMICO_EVALUATE_TEST()                                               \
    do                                                                         \
    {                                                                          \
        if (result == DYNAMICO_FAIL)                                           \
        {                                                                      \
            DYNAMICO_TEST_FAIL();                                              \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            DYNAMICO_TEST_PASS();                                              \
        }                                                                      \
    } while (0)
