#pragma once
#include "../model/db.h"

enum actions {
	REPORT_POOL,
	REPORT_COURSE,
	QUIT,
	END_OF_ACTIONS
};

extern int get_user_action(void);
void print_pool(pool_t *);
void print_course(course_t *);