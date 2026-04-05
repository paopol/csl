#ifndef CSL_H
#define CSL_H

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* #define CSL_IMPLEMENTATION */

/**
 * MICRO DEFINITIONS FOR CONSTANT
 */
#define csl_maximum_capacity_byte_size 0x40000000
#define csl_max_file_path_length 256
#define csl_max_buffer_size 1024
#define csl_lowest_log_level csl_log_level_trace

#define csl_default_time_format     "%Y-%m-%d %H:%M:%S"
#define csl_default_time_format2    "%Y/%m/%d %H:%M:%S"
#define csl_default_time_format3    "%Y 年 %m 月 %d 日 %H:%M:%S"
#define csl_time_expand_all(t)      (t).year, (t).month, (t).mday, (t).hour, (t).minute, (t).second

#define csl_null_ptr ((void *)0)

/**
 * MICRO DEFINITIONS FOR BASIC UTIL
 */

#define csl_unused_expression(var)  (void)(var)
#define csl_unused_parameter(var)   csl_unused_expression(var)

#define csl_check_type_size(var1, var2)     (sizeof(var1) == sizeof(var2))
#define csl_check_type_strict(var1, var2)   (void)((&(var1)) == (&(var2)))
#define csl_check_type_soft(var1, var2)     (void)(sizeof((var1) == (var2)))
#define csl_check_type_ptr(var1, var2)      (void)((var1) == (var2))

#define csl_array_size(array) (sizeof(array) / sizeof(*array))

/**
 * MICRO DEFINITIONS WITH RETURN
 */
#define csl_return_value_if(expression, value)      do { if (!!(expression)) { return (value); } } while (0)
#define csl_return_value_if_not(expression, value)  csl_return_value_if(!(expression), value)
#define csl_return_if(expression)                   do { if (!!(expression)) { return; } } while (0)
#define csl_return_if_not(expression)               csl_return_if(!(expression))
#define csl_return_null_if(expression)              csl_return_value_if(expression, csl_null_ptr)
#define csl_return_null_if_not(expression)          csl_return_value_if_not(!(expression), csl_null_ptr)

/**
 * MICRO DEFINITIONS FOR REPORTING INFO
 */
#define csl_report(stream, expression, msg_type_str, msg) \
    do                                                                                  \
    {                                                                                   \
        if (!(!!(expression)))                                                          \
        {                                                                               \
            fprintf(stream, "%s:%d:[%s]:%s\n", __FILE__, __LINE__, msg_type_str, msg);  \
            abort();                                                                    \
        }                                                                               \
    } while (0)

#define csl_unreachable(msg)    csl_report(stderr, csl_null_ptr, "UNREACHABLE", msg)
#define csl_todo(msg)           csl_report(stderr, csl_null_ptr, "TODO", msg)
#define csl_assert(expression)  csl_report(stderr, expression, "ASSERT", #expression)

/**
 * MICRO DEFINITIONS FOR OPERATING STRUCTURE
 */
#define csl_offset_of(type, member)         ((size_t)(((type *)0)->member))
#define csl_container_of(ptr, type, member) (type *)((size_t)ptr - (size_t)csl_offset_of(type, member))

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

#define csl_append(object, value) \
    do                                                                                  \
    {                                                                                   \
        if ((object).size >= (object).capacity)                                         \
        {                                                                               \
            size_t new_capacity = 2 * (object).capacity + 1;                            \
            void *new_data = realloc((object).data, new_capacity * (object).esize);     \
            csl_assert(new_data);                                                       \
            (object).capacity = new_capacity;                                           \
            (object).data = new_data;                                                   \
        }                                                                               \
        (object).data[(object).size++] = (value);                                       \
    } while (0)

#define csl_pop(object, value) \
    do                                              \
    {                                               \
        csl_assert((object).size > 0);              \
        (value) = (object).data[--(object).size];   \
    } while (0)

/**
 * MICRO DEFINITIONS FOR LOG
 */
#define csl_log_none(format, ...)   csl_log_write(csl_log_level_none, format, ##__VA_ARGS__)
#define csl_log_fatal(format, ...)  csl_log_write(csl_log_level_fatal, format, ##__VA_ARGS__)
#define csl_log_error(format, ...)  csl_log_write(csl_log_level_error, format, ##__VA_ARGS__)
#define csl_log_warn(format, ...)   csl_log_write(csl_log_level_warn, format, ##__VA_ARGS__)
#define csl_log_info(format, ...)   csl_log_write(csl_log_level_info, format, ##__VA_ARGS__)
#define csl_log_debug(format, ...)  csl_log_write(csl_log_level_debug, format, ##__VA_ARGS__)
#define csl_log_trace(format, ...)  csl_log_write(csl_log_level_trace, format, ##__VA_ARGS__)
#define csl_log_cnts(format, ...)   csl_log_write(csl_log_level_cnts, format, ##__VA_ARGS__)



/**
 * enumeration of boolean
 */
typedef enum
{
    csl_false   = 0,
    csl_true    = 1,
} csl_bool_t;

/**
 * enumeration of sign
 */
typedef enum
{
    csl_sign_negative = -1,
    csl_sign_positive = 1,
} csl_sign_type_t;



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


/**
 * structure of timer
 */
typedef struct
{
    clock_t start;
    clock_t end;
} csl_timer_t;

/**
 * structure of time
 */
typedef struct
{
    int second; /* 0~61 */
    int minute; /* 0~59 */
    int hour;   /* 0~23 */
    int mday;   /* 1~31, the day of the month */
    int month;  /* 1~12 (not 0~11) */
    int year;   /* (not the number of years since 1900) */
    int wday;   /* 1~7 (not 0~6), the day of the week */
    int yday;   /* 1~366 (not 0~365), the day of the year */
    int isdst;  /* is dst (daylight saving time) */
} csl_time_t;

/**
 * enumeration of log level
 */
typedef enum
{
    csl_log_level_none,
    csl_log_level_fatal,
    csl_log_level_error,
    csl_log_level_warn,
    csl_log_level_info,
    csl_log_level_debug,
    csl_log_level_trace,
    csl_log_level_cnts,
} csl_log_level_t;

static const char *csl_log_level_names[] = {
    "NONE", "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE", 
};

/**
 * enumeration of log ouput
 */
typedef enum
{
    csl_log_output_console  = 1 << 0,
    csl_log_output_file     = 1 << 1,
} csl_log_output_t;

/**
 * enumeration of log time field
 */
typedef enum
{
    csl_log_time_field_year         = 1 << 0,
    csl_log_time_field_month        = 1 << 1,
    csl_log_time_field_day          = 1 << 2,
    csl_log_time_field_hour         = 1 << 3,
    csl_log_time_field_minute       = 1 << 4,
    csl_log_time_field_second       = 1 << 5,
    csl_log_time_field_timestamp    = 1 << 6,
} csl_log_time_field_t;

/**
 * structure of log config
 */
typedef struct
{
    csl_log_level_t lowest_level;
    csl_log_output_t output_mask;
    csl_log_time_field_t time_field_mask;

    char dir_path[csl_max_file_path_length];
    char file_path[csl_max_file_path_length];
    FILE *fp;
} csl_log_config_t;

/**
 * structure of logger
 */
typedef struct
{
    csl_log_config_t config;
} csl_logger_t;


/**
 * function declaration
 */

void *csl_memory_copy(void *dst, const void *src, size_t size);
void *csl_memory_move(void *dst, const void *src, size_t size);
void *csl_memory_move2(void *dst, const void *src, size_t size);
void *csl__expand_capacity(void *object);
void *csl__reduce_capacity(void *object);
void *csl_push_back(void *object, const void *data);
void *csl_pop_back(void *object, void *data);
void *csl_push_front(void *object, const void *data);
void *csl_pop_front(void *object, void *data);


double csl_timer_difftime(const csl_timer_t *timer);
void csl_timer_start(csl_timer_t *timer);
double csl_timer_end(csl_timer_t *timer);

time_t csl_time(time_t *timestamp);
csl_time_t *csl_tm2time(const struct tm *_tm, csl_time_t *t);
struct tm *csl_time2tm(const csl_time_t *t, struct tm *_tm);
struct tm *csl_timestamp2tm(const time_t *timestamp, struct tm *_tm);
time_t *csl_tm2timestamp(const struct tm *_tm, time_t *timestamp);
csl_time_t *csl_timestamp2time(const time_t *timestamp, csl_time_t *t);
time_t *csl_time2timestamp(const csl_time_t *t, time_t *timestamp);
csl_time_t *csl_localtime(csl_time_t *t, const time_t *timestamp);
char *csl_asctime(char *buffer, size_t buffer_size, const csl_time_t *t);
size_t csl_strftime(char *buffer, size_t buffer_size, const char *format, const csl_time_t *t);
double csl_difftime(const csl_time_t *t_end, const csl_time_t *t_begin);
csl_time_t *csl_now(csl_time_t *t);


void csl_log_write(csl_log_level_t log_level, const char *format, ...);



#ifdef CSL_IMPLEMENTATION

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


double csl_timer_difftime(const csl_timer_t *timer)
{
    csl_return_value_if(!timer, 0.0);

    csl_assert(timer->end >= timer->start);

    return (double)(timer->end - timer->start) / CLOCKS_PER_SEC;
}

void csl_timer_start(csl_timer_t *timer)
{
    csl_return_if(!timer);

    timer->start = clock();
    timer->end = timer->start;
}

double csl_timer_end(csl_timer_t *timer)
{
    csl_return_value_if(!timer, 0.0);
    
    timer->end = clock();

    return csl_timer_difftime(timer);
}


time_t csl_time(time_t *timestamp)
{
    return time(timestamp);
}

csl_time_t *csl_tm2time(const struct tm *_tm, csl_time_t *t)
{
    csl_return_null_if(!_tm || !t);

    t->second = _tm->tm_sec;
    t->minute = _tm->tm_min;
    t->hour = _tm->tm_hour;
    t->mday = _tm->tm_mday;
    t->month = _tm->tm_mon + 1;
    t->year = _tm->tm_year + 1900;
    t->wday = _tm->tm_wday + 1;
    t->yday = _tm->tm_yday + 1;
    t->isdst = _tm->tm_isdst;

    return t;
}

struct tm *csl_time2tm(const csl_time_t *t, struct tm *_tm)
{
    csl_return_null_if(!t || !_tm);

    _tm->tm_sec = t->second;
    _tm->tm_min = t->minute;
    _tm->tm_hour = t->hour;
    _tm->tm_mday = t->mday;
    _tm->tm_mon = t->month - 1;
    _tm->tm_year = t->year - 1900;
    _tm->tm_wday = t->wday - 1;
    _tm->tm_yday = t->yday - 1;
    _tm->tm_isdst = t->isdst;

    return _tm;
}

struct tm *csl_timestamp2tm(const time_t *timestamp, struct tm *_tm)
{
    csl_return_null_if(!timestamp || !_tm);

    *_tm = *(struct tm *)localtime(timestamp);

    return _tm;
}

time_t *csl_tm2timestamp(const struct tm *_tm, time_t *timestamp)
{
    csl_return_null_if(!_tm || !timestamp);

    *timestamp = mktime((struct tm *)_tm);

    return timestamp;
}

csl_time_t *csl_timestamp2time(const time_t *timestamp, csl_time_t *t)
{
    csl_return_null_if(!timestamp || !t);

    struct tm _tm = {0};
    
    csl_timestamp2tm(timestamp, &_tm);

    return csl_tm2time(&_tm, t);
}

time_t *csl_time2timestamp(const csl_time_t *t, time_t *timestamp)
{
    csl_return_null_if(!t || !timestamp);

    struct tm _tm = {0};

    csl_time2tm(t, &_tm);

    return csl_tm2timestamp(&_tm, timestamp);
}

csl_time_t *csl_localtime(csl_time_t *t, const time_t *timestamp)
{
    return csl_timestamp2time(timestamp, t);
}

char *csl_asctime(char *buffer, size_t buffer_size, const csl_time_t *t)
{
    csl_return_null_if(!buffer || !buffer_size || !t);

    struct tm _tm = {0};
    int str_size = 0;
    size_t end = 0;

    csl_time2tm(t, &_tm);

    str_size = snprintf(buffer, buffer_size, "%s", asctime(&_tm));

    if (str_size <= 0)
    {
        end = 0;
    }
    else if (str_size < (int)buffer_size)
    {
        end = (size_t)str_size;
    }
    else
    {
        end = buffer_size - 1; 
    }

    buffer[end] = '\0';

    return buffer;
}

size_t csl_strftime(char *buffer, size_t buffer_size, const char *format, const csl_time_t *t)
{
    csl_return_value_if(!buffer || !buffer_size || !format || !t, 0);

    struct tm _tm = {0};

    return strftime(buffer, buffer_size, format, csl_time2tm(t, &_tm));
}

double csl_difftime(const csl_time_t *t_end, const csl_time_t *t_begin)
{
    time_t begin = 0, end = 0;

    csl_time2timestamp(t_begin, &begin);
    csl_time2timestamp(t_end, &end);

    return difftime(end, begin);
}

csl_time_t *csl_now(csl_time_t *t)
{
    time_t now = csl_time(NULL);

    return csl_localtime(t, &now);
}


void csl_log_write(csl_log_level_t log_level, const char *format, ...)
{
    csl_return_if(log_level > csl_lowest_log_level);

    FILE *stream = stdout;
    char time_str_buffer[32] = "";
    csl_time_t now = {0};

    csl_now(&now);

    csl_strftime(time_str_buffer, csl_array_size(time_str_buffer), csl_default_time_format, &now);

    fprintf(stream, "[%s]:[%s]:", time_str_buffer, csl_log_level_names[log_level]);

    va_list args;

    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);

    fprintf(stream, "\n");
}


#endif /* CSL_IMPLEMENTATION */


#endif /* CSL_H */
