#pragma once
#include "../model/db.h"

enum actions {
	WEEKLY_REPORT,
	MONTHLY_REPORT,
	QUIT,
	END_OF_ACTIONS
};

extern int get_teacher_action(void);
void print_report(report_t *);
