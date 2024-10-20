#define DYNAMICO_GENERIC_IMPLEMENTATION
#define DYNAMICO_GENERATE_ALL
#include "test.h"
#include <stdio.h>
#include <stdlib.h>

#define defer() defer : DYNAMICO_EVALUATE_TEST();

#define init(name, vec, type)                                                  \
    int result = 0;                                                            \
    DYNAMICO_TEST(name);                                                       \
    dynamico_Generic vec = {0};                                                \
    WDEFER(dynamico_Generic_init(&vec, sizeof(type)));

int testInit()
{
    int result = 0;
    DYNAMICO_TEST("Generic Init");

    dynamico_Generic gen = {0};
    WDEFER(dynamico_Generic_init(&gen, sizeof(int)));
defer:
    DYNAMICO_EVALUATE_TEST();
    return result;
}

newDynamicoType(int, Int);

int testAppend()
{
    init("Generic Append", vec, int);

    dynamico_Int newIntVec = {0};
    WDEFER(dynamico_Int_init(&newIntVec));
    for (int i = 0; i < 1000000; i++)
    {
        WDEFER(dynamico_Int_append(&newIntVec, i));
    }

    for (int i = 0; i < newIntVec.size; i++)
    {
        WDEFER(dynamico_Generic_append(&vec, dynamico_Int_at(&newIntVec, i)));
    }

    for (int i = 0; i < newIntVec.size; i++)
    {
        int *data = dynamico_Int_at(&newIntVec, i);
        int *vecData = dynamico_Generic_at(&vec, i);
        if (*data != *vecData)
        {
            DYNAMICO_LOG_ERROR("Vec does not same value as newIntVec");
            return_defer(1);
        }
    }

defer:
    dynamico_Int_free(&newIntVec);
    dynamico_Generic_free(&vec);
    DYNAMICO_EVALUATE_TEST();
    return result;
}

int testShift()
{
  int result = 0;
  DYNAMICO_TEST("Generic Shift");

  dynamico_Generic vec = {0};
  dynamico_Generic_init(&vec, sizeof(int));

  const int TEST_VEC_SIZE = 10;
  int arr[TEST_VEC_SIZE];

  for (int i = 0; i < TEST_VEC_SIZE; i++) {
    arr[i] = i;
  }

  int* pArr = arr;
  for (int i = 0; i < TEST_VEC_SIZE; i++, pArr++) {
    WDEFER(dynamico_Generic_append(&vec, pArr));
  }

  DYNAMICO_LOG_INFO("Shifting...");
  dynamico_Generic_shift(&vec);
  DYNAMICO_LOG_INFO("Done shifting...");

  for (int i = 0; i < TEST_VEC_SIZE; i++) {
    int* at = dynamico_Generic_at(&vec, i);
    if (at != NULL && *at != i + 1) {
      DYNAMICO_LOG_ERROR("Vec did not shift");
      return_defer(1);
    }
  }


defer:
    DYNAMICO_EVALUATE_TEST();
    return result;
}

int main()
{
    int result = 1;
    result &= testInit();
    result &= testAppend();
    result &= testShift();
    return result;
}
