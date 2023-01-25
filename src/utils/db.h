#pragma once
#include <stdbool.h>
#include <mysql.h>

#include "../model/db.h"

#define type(x, y) (__builtin_types_compatible_p(__typeof__((x)), y))

#define ctype_to_mysql_type(x) (\
    type((x), signed char): MYSQL_TYPE_TINYINT, \
	type((x), short int): MYSQL_TYPE_SHORT, \
	type((x), float): MYSQL_TYPE_FLOAT, \
    type((x), MYSQL_DATE): MYSQL_TYPE_DATE, \
    type((x), MYSQL_TIME): MYSQL_TYPE_TIME, \
    type((x), MYSQL_DATETIME): MYSQL_TYPE_DATETIME, \
    type((x), MYSQL_TIMESTAMP): MYSQL_TYPE_TIMESTAMP, \
    type((x), cf_t): MYSQL_TYPE_STRING, \
    type((x), code_t): MYSQL_TYPE_STRING, \
    type((x), char[]): MYSQL_TYPE_VAR_STRING, \
    type((x), MYSQL_BLOB): MYSQL_TYPE_BLOB \
)

typedef char[] MYSQL_BLOB;
typedef MYSQL_TIME MYSQL_DATE;
typedef MYSQL_TIME MYSQL_DATETIME;
typedef MYSQL_TIME MYSQL_TIMESTAMP;

extern void print_stmt_error (MYSQL_STMT *stmt, char *message);
extern void print_error(MYSQL *conn, char *message);
extern bool setup_prepared_stmt(MYSQL_STMT **stmt, char *statement, MYSQL *conn);
extern void finish_with_error(MYSQL *conn, char *message);
extern void finish_with_stmt_error(MYSQL *conn, MYSQL_STMT *stmt, char *message, bool close_stmt);
extern void set_binding_param(MYSQL_BIND *param, enum enum_field_types type, void *buffer, unsigned long len);
extern void date_to_mysql_time(char *str, MYSQL_TIME *time);
extern void time_to_mysql_time(char *str, MYSQL_TIME *time);
extern void init_mysql_timestamp(MYSQL_TIME *time);
extern void mysql_timestamp_to_string(MYSQL_TIME *time, char *str);
extern void mysql_date_to_string(MYSQL_TIME *date, char *str);
extern void mysql_time_to_string(MYSQL_TIME *time, char *str);
