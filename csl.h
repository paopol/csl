#ifndef CSL_H
#define CSL_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CSL_IMPLEMENTATION
#ifdef CSL_IMPLEMENTATION




#define csl_unused_parameter(var) (void)(var)


/**
 * MICRO DEFINITIONS WITH RETURN
 */
#define csl_return_value_if(expression, value) do { if (!!(expression)) { return (value); } } while (0)
#define csl_return_value_if_not(expression, value) csl_return_value_if(!(expression), value)
#define csl_return_if(expression) do { if (!!(expression)) { return; } } while (0)
#define csl_return_if_not(expression) csl_return_if(!(expression))
#define csl_return_null_if(expression) csl_return_value_if(expression, csl_null_ptr)
#define csl_return_null_if_not(expression) csl_return_value_if_not(!(expression), csl_null_ptr)

/**
 * MICRO DEFINITIONS FOR REPORTING INFO
 */
#define csl_report(stream, expression, info_type, info) \
    do                                                                                  \
    {                                                                                   \
        if (!(!!(expression)))                                                          \
        {                                                                               \
            fprintf(stream, "%s:%d:[%s]:%s\n", __FILE__, __LINE__, info_type, info);    \
            abort();                                                                    \
        }                                                                               \
    } while (0)
#define csl_unreachable(info) csl_report(stderr, csl_null_ptr, "UNREACHABLE", info)
#define csl_todo(info) csl_report(stderr, csl_null_ptr, "TODO", info)
#define csl_assert(expression) csl_report(stderr, expression, "ASSERT", #expression)

/**
 * MICRO DEFINITIONS FOR OPERATING STRUCTURE
 */
#define csl_offset_of(type, member) ((size_t)(((type *)0)->member))
#define csl_container_of(ptr, type, member) \
    (type *)((size_t)ptr - (size_t)csl_offset_of(type, member))

/**
 * MICRO DEFINITIONS FOR DEFINING STRUCTURE
 */
#define csl_buffer_object(item_type) \
    size_t size;                        \
    size_t capacity;                    \
    size_t esize;                       \
    item_type *data;
#define csl_iterable_object(item_type) \
    item_type *index;
#define csl_list_object(item_type) \
    csl_buffer_object(item_type)    \
    csl_iterable_object(item_type)
#define csl_bag_object(item_type) \
    csl_buffer_object(item_type)

/**
 * MICRO DEFINITIONS FOR INITIALIZING STRUCTURE
 */
#define csl_buffer_initializer(item_type) \
    {.size = 0, .capacity = 0, .esize = sizeof(item_type), .data = csl_null_ptr}
#define csl_list_object_initializer(item_type) \
    {.size = 0, .capacity = 0, .esize = sizeof(item_type), .data = csl_null_ptr, .index = csl_null_ptr}
#define csl_bag_object_initializer(item_type) csl_buffer_initializer

/**
 * MICRO DEFINITIONS FOR OPERATING OBJECT
 */
#define csl_pos(object, pos)    ((void *)((char *)(object).data + (pos) * (object).esize))
#define csl_begin(object)       csl_pos(object, 0)
#define csl_end(object)         csl_pos(object, (object).size)
#define csl_first(object)       csl_pos(object, 0)
#define csl_last(object)        csl_pos(object, (object).size - 1)

#define csl_move(object, pos)       ((object).index = csl_pos(object, pos))
#define csl_move_first(object)      csl_move(object, 0)
#define csl_move_last(object)       csl_move(object, (object).size - 1)
#define csl_move_begin(object)      csl_move(object, 0)
#define csl_move_end(object)        csl_move(object, (object).size)
#define csl_slide(object, offset)   ((object).index = (void *)((ptrdiff_t)(object).index + (offset) * (ptrdiff_t)(object).esize))
#define csl_slide_left(object)      ((object).index = (void *)((char *)(object).index - (object).esize))
#define csl_slide_right(object)     ((object).index = (void *)((char *)(object).index + (object).esize))


#define csl_null_ptr ((void *)0)


typedef enum
{
    csl_false,
    csl_true,
} csl_bool_t;


typedef struct
{
    csl_buffer_object(void)
} csl_buffer_object_header_t;

typedef struct
{
    csl_list_object(void)
} csl_list_object_header_t;

typedef struct
{
    csl_bag_object(void)
} csl_bag_object_header_t;



void *csl_memory_copy(void *dst, const void *src, size_t size)
{
    csl_return_value_if(!dst || !src || dst == src, dst);

    char *d = dst;
    const char *s = src;
    
    while (size-- > 0)
    {
        *d++ = *s++;
    }

    return dst;
}

void *csl_memory_move(void *dst, const void *src, size_t size)
{
    csl_return_value_if(!dst || !src || dst == src, dst);

    char *d = dst;
    const char *s = src;
    int direction = 1;

    if (s < d && d < s + size)
    {
        d += size - 1;
        s += size - 1;

        direction = -1;
    }
    
    for (size_t i = 0; i < size; i++)
    {
        *d = *s;

        d += direction;
        s += direction;
    }

    return dst;
}

void *csl_memory_move2(void *dst, const void *src, size_t size)
{
    csl_return_value_if(!dst || !src || dst == src, dst);

    char *d = dst;
    const char *s = src;

    if (s < d && d < s + size)
    {
        d += size - 1;
        s += size - 1;

        while (size-- > 0)
        {
            *d-- = *s--;
        }
    }
    else
    {
        while (size-- > 0)
        {
            *d++ = *s++;
        }
    }

    return dst;
}

void *csl__expand_capacity(void *object)
{
    csl_list_object_header_t *obj = (csl_list_object_header_t *)object;

    size_t new_capacity = 2 * obj->capacity + 1;
    void *new_data = realloc(obj->data, new_capacity * obj->esize);

    csl_assert(new_data);

    obj->capacity = new_capacity;
    obj->data = new_data;

    return object;
}

void *csl__reduce_capacity(void *object)
{
    csl_list_object_header_t *obj = (csl_list_object_header_t *)object;

    size_t new_capacity = obj->capacity / 2;
    void *new_data = realloc(obj->data, new_capacity * obj->esize);

    csl_assert(new_data);

    obj->capacity = new_capacity;
    obj->data = new_data;

    return object;
}

void *csl_push_back(void *object, const void *data)
{
    csl_return_null_if(!object || !data);

    csl_list_object_header_t *obj = (csl_list_object_header_t *)object;

    if (obj->size >= obj->capacity)
    {
        csl__expand_capacity(object);
    }

    csl_memory_copy(csl_end(*obj), data, obj->esize);
    ++obj->size;

    return object;
}

void *csl_pop_back(void *object, void *data)
{
    csl_return_null_if(!object);

    csl_list_object_header_t *obj = (csl_list_object_header_t *)object;

    csl_return_null_if(obj->size < 1);

    csl_memory_copy(data, csl_last(*obj), obj->esize);
    --obj->size;

    return object;
}

void *csl_push_front(void *object, const void *data)
{
    csl_return_null_if(!object || !data);

    csl_list_object_header_t *obj = (csl_list_object_header_t *)object;

    if (obj->size >= obj->capacity)
    {
        csl__expand_capacity(object);
    }

    csl_memory_move(csl_pos(*obj, 1), csl_pos(*obj, 0), obj->size * obj->esize);

    csl_memory_copy(csl_begin(*obj), data, obj->esize);
    ++obj->size;

    /* obj->index = (char *)obj->index + obj->esize; */

    return object;
}

void *csl_pop_front(void *object, void *data)
{
    csl_return_null_if(!object);

    csl_list_object_header_t *obj = (csl_list_object_header_t *)object;

    csl_return_null_if(obj->size < 1);

    csl_memory_copy(data, csl_begin(*obj), obj->esize);

    csl_memory_copy(csl_pos(*obj, 0), csl_pos(*obj, 1), (obj->size - 1) * obj->esize);
    --obj->size;

    /* if (obj->index >= csl_pos(*obj, 1)) { obj->index = (char *)obj->index - obj->esize; } */

    return object;
}




#endif /* CSL_IMPLEMENTATION */


#endif /* CSL_H */
