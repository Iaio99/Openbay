#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "user.h"
#include "../model/db.h"
#include "../view/user.h"
#include "../utils/io.h"

static bool report_pool(void)
{
	pool_t *pool = do_view_pool();
	if (pool != NULL) {
		print_pool(pool);
		pool_dispose(pool);
	}
    return false;
}

static bool report_course(void)
{
    char course_name[NOME_LEN];
    get_input("Course name: ", NOME_LEN, course_name, false);
	course_t *course = do_view_course(course_name);
	if(course != NULL) {
		print_course(course);
		course_dispose(course);
	}
	return false;
}


static bool quit(void) {
	return true;
}


static struct {
	enum actions action;
	bool (*control)(void);
} controls[END_OF_ACTIONS] = {
	{.action = REPORT_POOL, .control = report_pool},
	{.action = REPORT_COURSE, .control = report_course},
	{.action = QUIT, .control = quit}
};


void user_controller(void)
{
	db_switch_to_user();

	while(true) {
		int action = get_user_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}
