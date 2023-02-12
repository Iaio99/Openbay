#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "user.h"
#include "../model/db.h"
#include "../view/user.h"
#include "../utils/io.h"


/*
static bool user_aste(void)
{
	asta_t *aste = do_stato_aste_utente(user);
	if(aste != NULL) {
		print_aste(aste);
		aste_dispose(aste);
	}
	return false;
}
*/


static bool aste_in(void)
{
	asta_t *aste = do_visualizza_oggetti_asta();
	if(aste != NULL) {
		print_aste(aste);
		aste_dispose(aste);
	}
	return false;
}


static bool aste_done(void)
{
	asta_t *aste = do_visualizza_aste_passate();
	if(aste != NULL) {
		print_aste(aste);
		aste_dispose(aste);
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
//	{.action = OFFER, .control = report_pool},
//	{.action = CONTROFFER, .control = report_course},
//	{.action = USER_ASTE, .control = report_pool},
	{.action = ASTE_IN_CORSO, .control = aste_in},
	{.action = ASTE_FINITE, .control = aste_done},
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
