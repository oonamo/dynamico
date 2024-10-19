#define DYNAMICO_GENERATE_ALL
#define DYNAMICO_LOGGING

#include "../dynamico.h"
#include <stdio.h>
#include <stdlib.h>

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

newDynamicoType(int, Int);
newDynamicoType(char *, String);

int testInit()
{
    int result = 0;
    DYNAMICO_TEST("Dynamic Init");

    dynamico_Int newIntVec = {0};
    WDEFER(dynamico_Int_init(&newIntVec));

defer:
    dynamico_Int_free(&newIntVec);
    DYNAMICO_EVALUATE_TEST();
    return result;
}

int testAppend()
{
    int result = 0;
    DYNAMICO_TEST("Dynamic Append");

    dynamico_Int newIntVec = {0};
    WDEFER(dynamico_Int_init(&newIntVec));
    for (int i = 0; i < 1000000; i++)
    {
        WDEFER(dynamico_Int_append(&newIntVec, i));
    }
    for (int i = 0; i < newIntVec.size; i++)
    {
        if (newIntVec.data[i] != i)
        {
            return_defer(1);
        }
    }

defer:
    dynamico_Int_free(&newIntVec);
    DYNAMICO_EVALUATE_TEST();

    return result;
}

int testShift()
{
    int result = 0;
    const int TEST_VEC_SIZE = 10;
    DYNAMICO_TEST("Dynamic Shift");
    dynamico_Int intVec = {0};
    dynamico_Int_init(&intVec);
    for (int i = 0; i < TEST_VEC_SIZE; i++)
    {
        WDEFER(dynamico_Int_append(&intVec, i));
    }
    dynamico_Int_shift(&intVec);
    for (int i = 0; i < intVec.size; i++)
    {
        if (intVec.data[i] != i + 1)
        {
            return_defer(1);
        }
    }

defer:
    dynamico_Int_free(&intVec);
    DYNAMICO_EVALUATE_TEST();
    return result;
}

int testAt()
{
    int result = 0;
    const int TEST_VEC_SIZE = 10;
    DYNAMICO_TEST("Dynamic At");
    dynamico_Int intVec = {0};
    dynamico_Int_init(&intVec);
    for (int i = 0; i < TEST_VEC_SIZE; i++)
    {
        WDEFER(dynamico_Int_append(&intVec, i));
    }
    for (int i = 0; i < intVec.size; i++)
    {
        int *value = dynamico_Int_at(&intVec, i);
        if (*value != i)
        {
            DYNAMICO_LOG_ERROR("Option.value is not the same as the index");
            return_defer(1);
        }
        *value = i + 1;
        if (*(int *)(dynamico_Int_at(&intVec, i)) != i + 1)
        {
            DYNAMICO_LOG_ERROR("Returned value from at is not mutable!");
            return_defer(1);
        }
    }
    int *value = dynamico_Int_at(&intVec, -200);
    if (value != NULL)
    {
        DYNAMICO_LOG_ERROR("Option.ok return ok on negative index");
        return_defer(1);
    }
defer:
    dynamico_Int_free(&intVec);
    DYNAMICO_EVALUATE_TEST();
    return result;
}

typedef struct rand_struct
{
    int id;
    struct rand_struct *parent;
} rand_struct;

newDynamicoType(rand_struct, Rand);

int testArbituaryStruct()
{
    DYNAMICO_TEST("Dynamic Rand");
    int result = 0;
    dynamico_Rand rand = {0};
    dynamico_Rand_init(&rand);
    const int RAND_STRUCT_SIZE = 50;

    for (int i = 0; i < RAND_STRUCT_SIZE; i++)
    {
        rand_struct s = {
            .id = i,
        };
        s.parent = dynamico_Rand_at(&rand, i - 1);
        WDEFER(dynamico_Rand_append(&rand, s));
    }

    for (int i = 0; i < RAND_STRUCT_SIZE - 1; i++)
    {
        rand_struct *current = dynamico_Rand_at(&rand, i);
        rand_struct *parent = dynamico_Rand_at(&rand, i - 1);
        if (parent != NULL && current->id != parent->id + 1)
        {
            DYNAMICO_LOG_ERROR("Index: %d: Did not maintain order of "
                               "rand_struct.\nCurrent: %d, Parent %d",
                               i, current->id, parent->id + 1);
            return_defer(1);
        }
        if (current->parent != NULL && parent != NULL &&
            parent->parent != NULL && current->parent->id != parent->id &&
            current->parent != parent)
        {
            DYNAMICO_LOG_ERROR("Parent pointers do not point to same address: "
                               "values\nIndex: %d current: %d, parent %d",
                               i, current->parent->id, parent->id);
            return_defer(1);
        }
    }

    dynamico_Rand_shift(&rand);

    for (int i = 0; i < rand.size; i++)
    {
        rand_struct *current = dynamico_Rand_at(&rand, i);
        if (current->id != i + 1)
        {
            DYNAMICO_LOG_ERROR(
                "Shift did not keep order or values: current %d, index %d",
                current->id, i);
            return_defer(1);
        }
    }

defer:
    dynamico_Rand_free(&rand);
    DYNAMICO_EVALUATE_TEST();
    return result;
}

int main()
{
    int result = 1;
    result &= testInit();
    result &= testAppend();
    result &= testShift();
    result &= testAt();
    result &= testArbituaryStruct();

defer:
    return result;
}
