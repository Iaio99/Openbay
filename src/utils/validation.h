#pragma once

#include <stdbool.h>

extern bool init_validation(void);
extern void fini_validation(void);
extern bool validate_time(char *str);
extern bool validate_date(char *str);
extern bool validate_cf(char *str);
extern bool validate_cvv(char *str);
extern bool validate_credit_card_number(char *str);