#ifndef CSL_H
#define CSL_H

#include <assert.h>
#include <float.h>
#include <pthread.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* #define CSL_IMPLEMENTATION */

/**
 * MACRO DEFINITIONS FOR CONSTANT
 */

 #define csl_null_ptr ((void *)0)

#define csl_maximum_capacity_byte_size 0x40000000
#define csl_max_file_path_length 256
#define csl_max_buffer_size 1024
#define csl_lowest_log_level csl_log_level_trace

#define csl_default_time_format     "%Y-%m-%d %H:%M:%S"
#define csl_default_time_format2    "%Y/%m/%d %H:%M:%S"
#define csl_default_time_format3    "%Y 年 %m 月 %d 日 %H:%M:%S"
#define csl_time_expand_all(t)      (t).year, (t).month, (t).mday, (t).hour, (t).minute, (t).second

#define csl_default_input_stream    stdin
#define csl_default_output_stream   stdout

/**
 * MACRO DEFINITIONS FOR BASIC UTIL
 */

#define csl_unused_expression(var)  (void)(var)
#define csl_unused_parameter(var)   csl_unused_expression(var)

#define csl_check_type_size(var1, var2)     (sizeof(var1) == sizeof(var2))
#define csl_check_type_strict(var1, var2)   (void)((&(var1)) == (&(var2)))
#define csl_check_type_soft(var1, var2)     (void)(sizeof((var1) == (var2)))
#define csl_check_type_ptr(var1, var2)      (void)((var1) == (var2))

#define csl_array_size(array) (sizeof(array) / sizeof(*array))

#define csl_ptr_dereference(type, var)              (*(type *)var)
#define csl_ptr_dereference_const(type, var)        (*(const type *)var)
#define csl_ptr_assign(type, lvar, rvar)            (*(type *)lvar = *(type *)rvar)
#define csl_ptr_assign_const(type, lvar, rvar)      (*(type *)lvar = *(const type *)rvar)

#define csl_malloc_safe(type, ...)  ((type *)csl_checked_malloc(sizeof(type), csl_default_output_stream, ##__VA_ARGS__))
#define csl_malloc(type)            ((type *)malloc(sizeof(type)))
#define csl_calloc(size, type)      ((type *)calloc(size, sizeof(type)))
#define csl_free(var)               free(var)

#define csl_macro_concat(x, y) x##y
#define csl_macro_str(x) #x

#define csl_ternary_expression(expression, v1, v2) ((expression) ? (v1) : (v2))

#define csl_two_way_cmp(v1, v2)     ((v1) > (v2))
#define csl_three_way_cmp(v1, v2)   (((v1) > (v2)) - ((v1) < (v2)))

/**
 * MACRO DEFINITIONS WITH RETURN
 */
#define csl_return_value_if(expression, value)      do { if (!!(expression)) { return (value); } } while (0)
#define csl_return_value_if_not(expression, value)  csl_return_value_if(!(expression), value)
#define csl_return_if(expression)                   do { if (!!(expression)) { return; } } while (0)
#define csl_return_if_not(expression)               csl_return_if(!(expression))
#define csl_return_null_if(expression)              csl_return_value_if(expression, csl_null_ptr)
#define csl_return_null_if_not(expression)          csl_return_value_if_not(!(expression), csl_null_ptr)

/**
 * MACRO DEFINITIONS FOR REPORTING INFO
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
 * MACRO DEFINITIONS FOR LOG
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
 * MACRO DEFINITIONS FOR OPERATING STRUCTURE
 */
#define csl_offset_of(type, member)         ((size_t)(((type *)0)->member))
#define csl_container_of(ptr, type, member) (type *)((size_t)ptr - (size_t)csl_offset_of(type, member))

/**
 * MACRO DEFINITIONS FOR DEFINING STRUCTURE
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
 * MACRO DEFINITIONS FOR INITIALIZING STRUCTURE
 */
#define csl_buffer_initializer(item_type) \
    {.size = 0, .capacity = 0, .esize = sizeof(item_type), .data = csl_null_ptr}
#define csl_list_object_initializer(item_type) \
    {.size = 0, .capacity = 0, .esize = sizeof(item_type), .data = csl_null_ptr, .index = csl_null_ptr}
#define csl_bag_object_initializer(item_type) csl_buffer_initializer

/**
 * MACRO DEFINITIONS FOR OPERATING OBJECT
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
 * type definition of basic data types
 */
typedef char        csl_char_t;
typedef short       csl_short_t;
typedef int         csl_int_t;
typedef long        csl_long_t;
typedef long long   csl_long_long_t;
typedef float       csl_float_t;
typedef float       csl_float32_t;
typedef double      csl_double_t;
typedef double      csl_float64_t;

typedef int8_t      csl_int8_t;
typedef int16_t     csl_int16_t;
typedef int32_t     csl_int32_t;
typedef int64_t     csl_int64_t;
typedef uint8_t     csl_uint8_t;
typedef uint16_t    csl_uint16_t;
typedef uint32_t    csl_uint32_t;
typedef uint64_t    csl_uint64_t;


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

static const char *csl_log_level_names[csl_log_level_cnts] = {
    [csl_log_level_none]    = "NONE", 
    [csl_log_level_fatal]   = "FATAL", 
    [csl_log_level_error]   = "ERROR", 
    [csl_log_level_warn]    = "WARN", 
    [csl_log_level_info]    = "INFO", 
    [csl_log_level_debug]   = "DEBUG", 
    [csl_log_level_trace]   = "TRACE", 
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
 * structure of log configuration
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
 * structure of type operations
 */
typedef struct csl_type_ops
{
    void *(*constructor)(void *self, const void *other);
    void *(*copy)(const void *other);
    void *(*move)(void *self, const void *other);
    void (*destructor)(void *self);

    int (*compare)(const void *v1, const void *v2);
    csl_bool_t (*equal)(const void *v1, const void *v2);

    uint64_t (*hash)(const void *self);

    char *(*to_string)(const void *self);
    void *(*from_string)(const char *self_str);

    void (*dump)(const void *self, size_t indent);
} csl_type_ops_t;

/**
 * structure of type information
 */
typedef struct csl_type_info
{
    const char *name;
    size_t size;
    csl_type_ops_t *ops;
    struct csl_type_info *parent;

    void *(*cast)(const void *self, const struct csl_type_info *type);
} csl_type_info_t;

/**
 * structure of bag operations
 */
typedef struct csl_bag csl_bag_t;
typedef struct csl_bag_ops
{
    /* read */
    size_t (*size)(const csl_bag_t *bag);
    size_t (*capacity)(const csl_bag_t *bag);
    void *(*data)(const csl_bag_t *bag);

    csl_bool_t (*empty)(const csl_bag_t *bag);

    /* visit */
    void *(*get)(const csl_bag_t *bag, size_t index);
    void *(*find)(const csl_bag_t *bag, const void *item, csl_bool_t (*equal)(const void *item1, const void *item2));

    /* create */
    csl_bool_t (*append)(csl_bag_t *bag, const void *item);
    csl_bool_t (*insert)(csl_bag_t *bag, size_t index, const void *item);
    csl_bool_t (*append_all)(csl_bag_t *bag, const void *items, size_t size);
    
    /* delete */
    csl_bool_t (*remove)(csl_bag_t *bag, size_t index);
    void *(*pop)(csl_bag_t *bag);
    void *(*detach)(csl_bag_t *bag, size_t index);

    /* manage */
    csl_bool_t (*reserve)(csl_bag_t *bag, size_t new_capacity);
    void (*shrink)(csl_bag_t *bag);

    void (*clear)(csl_bag_t *bag);
    void (*destroy)(csl_bag_t *bag);

    /* other */
    void *(*clone)(const csl_bag_t *bag);
} csl_bag_ops_t;

/**
 * structure of bag
 */
typedef struct csl_bag
{
    size_t size;
    size_t capacity;
    void *data;
    csl_bool_t owns_data;
    csl_bag_ops_t *ops;
    csl_type_info_t *type;
} csl_bag_t;


/**
 * structure of string builder
 */
typedef struct csl_string_builder
{
    char *data;
    size_t length;
    size_t capacity;
} csl_string_builder_t;

/**
 * structure of string buffer
 */
typedef struct csl_string_buffer
{
    char *data;
    size_t length;
    size_t capacity;
    pthread_mutex_t mutex;
} csl_string_buffer_t;

/**
 * structure of string view
 */
typedef struct csl_string_view
{
    const char *data;
    size_t count;
} csl_string_view_t;


/**
 * function declaration
 */

void *csl_checked_malloc(size_t size, FILE *stream, const char *format, ...);

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


void *csl_memory_copy(void *_dst, const void *_src, size_t _size);
void *csl_memory_move(void *_dst, const void *_src, size_t _size);
void *csl_memory_set(void *_dst, char _value, size_t _size);
int csl_memory_compare(const void *_data1, const void *_data2, size_t _size);

size_t csl_string_length(const char *_str);
char *csl_string_copy(char *_dst, const char *_src);
int csl_string_compare(const char *_str1, const char *_str2);
char *csl_string_copy_n(char *_dst, const char *_src, size_t _n);
int csl_string_compare_n(const char *_str1, const char *_str2, size_t _n);


void *csl__expand_capacity(void *object);
void *csl__reduce_capacity(void *object);
void *csl_push_back(void *object, const void *data);
void *csl_pop_back(void *object, void *data);
void *csl_push_front(void *object, const void *data);
void *csl_pop_front(void *object, void *data);



#ifdef CSL_IMPLEMENTATION

void *csl_checked_malloc(size_t size, FILE *stream, const char *format, ...)
{
    void *p = malloc(size);
    
    if (!p)
    {
        va_list args;

        va_start(args, format);
        vfprintf(stream, format, args);
        va_end(args);

        abort();
    }

    return p;
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


void *csl_memory_copy(void *_dst, const void *_src, size_t _size)
{
    csl_return_value_if(!_dst || !_src || _dst == _src || !_size, _dst);

    char *__dst = _dst;
    const char *__src = _src;
    
    for (size_t i = 0; i < _size; ++i)
    {
        __dst[i] = __src[i];
    }

    return _dst;
}

void *csl_memory_move(void *_dst, const void *_src, size_t _size)
{
    csl_return_value_if(!_dst || !_src || _dst == _src || !_size, _dst);

    char *__dst = _dst;
    const char *__src = _src;

    if (__dst < __src || __src + _size <= __dst)
    {
        for (size_t i = 0; i < _size; ++i)
        {
            __dst[i] = __src[i];
        }
    }
    else if (__dst > __src)
    {
        for (size_t i = _size; i > 0; --i)
        {
            __dst[i - 1] = __src[i - 1];
        }
    }

    return _dst;
}

void *csl_memory_set(void *_buffer, char _value, size_t _size)
{
    csl_return_value_if(!_buffer || !_size, _buffer);

    char *p = _buffer;

    for (size_t i = 0; i < _size; ++i)
    {
        p[i] = _value;
    }

    return _buffer;
}

int csl_memory_compare(const void *_buffer1, const void *_buffer2, size_t _size)
{
    csl_return_value_if(_buffer1 == _buffer2 || !_size, 0);
    csl_return_value_if(!_buffer1, -1);
    csl_return_value_if(!_buffer2, 1);
   
    const unsigned char *p1 = _buffer1;
    const unsigned char *p2 = _buffer2;

    for (size_t i = 0; i < _size; ++i)
    {
        if (p1[i] != p2[i])
        {
            return (p1[i] > p2[i]) - (p1[i] < p2[i]);
        }
    }

    return 0;
}


size_t csl_string_length(const char *_str)
{
    csl_return_value_if(!_str, 0);

    const char *start = _str;

    while (*_str)
    {
        ++_str;
    }

    return (size_t)(_str - start);
}

char *csl_string_copy(char *_dst, const char *_src)
{
    csl_return_value_if(!_dst || !_src, _dst);

    while (*_src)
    {
        *_dst++ = *_src++;
    }

    return _dst;
}

int csl_string_compare(const char *_str1, const char *_str2)
{
    csl_return_value_if(_str1 == _str2, 0);
    csl_return_value_if(!_str1, -1);
    csl_return_value_if(!_str2, 1);

    while (*_str1 && *_str1 == *_str2)
    {
        ++_str1;
        ++_str2;
    }

    return (*_str1 > *_str2) - (*_str1 < *_str2);
}

char *csl_string_copy_n(char *_dst, const char *_src, size_t _n)
{
    csl_return_value_if(!_dst || !_src || !_n, _dst);

    while (*_src && _n-- > 0)
    {
        *_dst++ = *_src++;
    }

    return _dst;
}

int csl_string_compare_n(const char *_str1, const char *_str2, size_t _n)
{
    csl_return_value_if(_str1 == _str2 || !_n, 0);
    csl_return_value_if(!_str1, -1);
    csl_return_value_if(!_str2, 1);

    while (*_str1 && *_str1 == *_str2 && _n-- > 0)
    {
        ++_str1;
        ++_str2;
    }

    return (*_str1 > *_str2) - (*_str1 < *_str2);
}

/*
csl_string_builder_t *csl_string_builder_create(const char *str)
{
    csl_string_builder_t *sb = csl_calloc(1, csl_string_builder_t);

    csl_assert(sb);

    csl_return_value_if(!str, sb);

    sb->length = csl_string_length(str);
    sb->capacity = sb->length + 1;

    sb->data = csl_calloc(sb->capacity, char);
    csl_assert(sb->data);
    csl_memory_copy(sb->data, str, sb->length * sizeof(char));
    sb->data[sb->length] = '\0';

    return sb;
}

csl_string_builder_t *csl_string_builder_append_char(csl_string_builder_t *sb, char c)
{
    csl_return_value_if(!sb, sb);

    if (sb->length + 1 >= sb->capacity)
    {
        size_t new_capacity = sb->capacity * 2 + 1;
        char *new_data = realloc(sb->data, new_capacity * sizeof(char));

        csl_assert(new_data);

        sb->capacity = new_capacity;
        sb->data = new_data;
    }

    sb->data[sb->length++] = c;
    sb->data[sb->length] = '\0';

    return sb;
}

csl_string_builder_t *csl_string_builder_append_str(csl_string_builder_t *sb, const char *str)
{
    csl_return_value_if(!sb || !str, sb);

    size_t len = sb->length + csl_string_length(str);

    if (len >= sb->capacity)
    {
        size_t new_capacity = 2 * sb->capacity + 1;

        while (new_capacity > sb->capacity && len >= new_capacity)
        {
            new_capacity = 2 * new_capacity + 1;
        }

        char *new_data = realloc(sb->data, new_capacity * sizeof(char));

        csl_assert(new_data);

        sb->capacity = new_capacity;
        sb->data = new_data;
    }

    csl_string_copy(sb->data + sb->length, str);
    sb->length = len;
    sb->data[sb->length] = '\0';

    return sb;
}
*/

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
