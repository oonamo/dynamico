#ifndef _DYNAMICO_H
#define _DYNAMICO_H

#define DYNAMICO_FAIL 1

#ifdef DYNAMICO_LOGGING
#ifdef DYNAMICO_NO_TERMINAL_COLORS
#define DYNAMICO_TERMINAL_RED ""
#define DYNAMICO_TERMINAL_YELLOW ""
#define DYNAMICO_TERMINAL_BLUE ""
#define DYNAMICO_TERMINAL_RESET ""
#define DYNAMICO_TERMINAL_GREEN ""
#else
#define DYNAMICO_TERMINAL_RED "\033[1;31m"
#define DYNAMICO_TERMINAL_YELLOW "\033[1;33m"
#define DYNAMICO_TERMINAL_BLUE "\033[1;34m"
#define DYNAMICO_TERMINAL_GREEN "\033[1;32m"
#define DYNAMICO_TERMINAL_RESET "\033[0m"
#endif

#define DYNAMICO_LOG_ERROR(format, ...)                                        \
    fprintf(stderr,                                                            \
            DYNAMICO_TERMINAL_RED "ERROR" DYNAMICO_TERMINAL_RESET              \
                                  ": %s:%d: " format "\n",                     \
            __FILE__, __LINE__, ##__VA_ARGS__)

#define DYNAMICO_LOG_INFO(format, ...)                                         \
    fprintf(stderr,                                                            \
            DYNAMICO_TERMINAL_BLUE "INFO" DYNAMICO_TERMINAL_RESET              \
                                   ": %s:%d: " format "\n",                    \
            __FILE__, __LINE__, ##__VA_ARGS__)

#define DYNAMICO_LOG_WARN(format, ...)                                         \
    fprintf(stdout,                                                            \
            DYNAMICO_TERMINAL_YELLOW "INFO" DYNAMICO_TERMINAL_RESET            \
                                     ": %s:%d: " format "\n",                  \
            __FILE__, __LINE__, ##__VA_ARGS__)

#define DYNAMICO_LOG_DEBUG(format, ...)                                        \
    fprintf(stdout, "INFO: %s:%d in function " format "\n", __FILE__,          \
            __LINE__, __func__##__VA_ARGS__)
#endif

#ifndef return_defer
#define return_defer(code)                                                     \
    do                                                                         \
    {                                                                          \
        result = code;                                                         \
        goto defer;                                                            \
    } while (0);
#endif

#ifndef WDEFER
#define WDEFER(eval)                                                           \
    do                                                                         \
    {                                                                          \
        if ((eval) == DYNAMICO_FAIL)                                           \
        {                                                                      \
            goto defer;                                                        \
        }                                                                      \
    } while (0)

#endif

#ifdef DYNAMICO_GENERATE_ALL
#define DYNAMICO_GENERATE_ALLOCATOR
#define DYNAMICO_GENERATE_APPEND
#define DYNAMICO_GENERATE_FREE
#define DYNAMICO_GENERATE_SHIFT
#define DYNAMICO_GENERATE_AT
#endif

#ifndef DYNAMICO_INIT_CAPACITY
#define DYNAMICO_INIT_CAPACITY 15
#endif

#ifdef DYNAMICO_GENERATE_ALLOCATOR
#define __generate_dynamico_allocator(type, name)                              \
    int dynamico_##name##_init(dynamico_##name *dynamic_object)                \
    {                                                                          \
        int result = 0;                                                        \
        dynamic_object->data =                                                 \
            (type *)malloc(sizeof(type) * DYNAMICO_INIT_CAPACITY);             \
        if (dynamic_object->data == NULL)                                      \
        {                                                                      \
            return_defer(DYNAMICO_FAIL);                                       \
        }                                                                      \
        dynamic_object->capacity = DYNAMICO_INIT_CAPACITY;                     \
        dynamic_object->size = 0;                                              \
    defer:                                                                     \
        return result;                                                         \
    }
#else
#define __generate_dynamico_allocator(type, name)
#endif

#ifdef DYNAMICO_GENERATE_APPEND
#define __generate_dynamico_appender(type, name)                               \
    int dynamico_##name##_append(dynamico_##name *dynamic_object, type obj)    \
    {                                                                          \
        int result = 0;                                                        \
        if (dynamic_object->size >= dynamic_object->capacity)                  \
        {                                                                      \
            dynamic_object->capacity = dynamic_object->capacity == 0           \
                                           ? DYNAMICO_INIT_CAPACITY            \
                                           : dynamic_object->capacity * 2;     \
            dynamic_object->data =                                             \
                realloc(dynamic_object->data,                                  \
                        sizeof(type) * dynamic_object->capacity * 2);          \
            if (dynamic_object->data == NULL)                                  \
            {                                                                  \
                return_defer(DYNAMICO_FAIL);                                   \
            }                                                                  \
        }                                                                      \
        dynamic_object->data[dynamic_object->size++] = obj;                    \
    defer:                                                                     \
        return result;                                                         \
    }

#else
#define __generate_dynamico_appender(type, name)
#endif

#ifdef DYNAMICO_GENERATE_SHIFT
#define __generate_dynamico_shift(type, name)                                  \
    void dynamico_##name##_shift(dynamico_##name *dynamic_object)              \
    {                                                                          \
        if (dynamic_object->size > 1)                                          \
        {                                                                      \
            for (size_t i = 1; i < dynamic_object->size; i++)                  \
            {                                                                  \
                dynamic_object->data[i - 1] = dynamic_object->data[i];         \
            }                                                                  \
            dynamic_object->size--;                                            \
        }                                                                      \
    }

#else
#define __generate_dynamico_shift(type, name)
#endif

#ifdef DYNAMICO_GENERATE_AT
#define __generate_dynamico_at(type, name)                                     \
    void *dynamico_##name##_at(dynamico_##name *dynamic_object, size_t index)  \
    {                                                                          \
        if (index >= dynamic_object->size || index < 0)                        \
        {                                                                      \
            return NULL;                                                       \
        }                                                                      \
        type *data = &dynamic_object->data[index];                             \
        return data;                                                           \
    }

#endif

#ifdef DYNAMICO_GENERATE_FREE
#define __generate_dynamico_free(type, name)                                   \
    void dynamico_##name##_free(dynamico_##name *dynamic_object)               \
    {                                                                          \
        free(dynamic_object->data);                                            \
        dynamic_object->data = NULL;                                           \
    }
#else
#define __generate_dynamico_free(type, name)
#endif

#define newDynamicoType(type, name)                                            \
    typedef struct                                                             \
    {                                                                          \
        type *data;                                                            \
        size_t size;                                                           \
        size_t capacity;                                                       \
    } dynamico_##name;                                                         \
    __generate_dynamico_allocator(type, name)                                  \
        __generate_dynamico_appender(type, name)                               \
            __generate_dynamico_free(type, name)                               \
                __generate_dynamico_shift(type, name)                          \
                    __generate_dynamico_at(type, name)

#endif
