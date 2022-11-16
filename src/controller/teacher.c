#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "teacher.h"
#include "../model/db.h"
#include "../view/teacher.h"
#include "../utils/io.h"

static bool report(bool type)
{
    cf_t cf;
    get_input("CF: ", CF_LEN, cf, false);

	report_t *report = do_view_report(cf, type);
	if(report != NULL) {
		print_report(report);
		report_dispose(report);
	}
	return false;
}


static bool report_weekly(void)
{
	return report(WEEKLY_REPORT_TYPE);
}


static bool report_monthly(void)
{
	return report(MONTHLY_REPORT_TYPE);
}


static bool quit(void) {
	return true;
}


static struct {
	enum actions action;
	bool (*control)(void);
} controls[END_OF_ACTIONS] = {
	{.action = WEEKLY_REPORT, .control = report_weekly},
	{.action = MONTHLY_REPORT, .control = report_monthly},
	{.action = QUIT, .control = quit}
};


void teacher_controller(void)
{
	db_switch_to_teacher();

	while(true) {
		int action = get_teacher_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}
