# Dynamico
Create dynamic objects with ease

## Example

```c
#define DYNAMICO_INIT_CAPACITY 20
#include "dynamico.h"
typedef struct
{
  int id;
  char* name;
} student_t;

newDynamicoType(student_t, Student);

int main()
{
  int result = 0;
  dynamico_Student studentVec = {0};
  dynamico_Student_init(&studentVec); // Allocates memory for studentVec

  for (int i = 0; i < 10; i++)
  {
    student_t student = { .id = i, .name = NULL };
    // WDEFER returns to defer if append returns DYNAMICO_FAIL
    WDEFER(dynamico_Student_append(&studentVec, student)); // Appends student to studentVec
  }

defer:
  dynamico_Student_free(&studentVec);
  return result;
}
```
