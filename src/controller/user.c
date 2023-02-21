#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "user.h"
#include "../model/db.h"
#include "../view/user.h"
#include "../utils/io.h"


cf_t user_cf;


static bool user_auctions(void)
{
	auction_t *auctions = view_user_auctions_informations(user_cf);
	
	if(auctions != NULL) {
		print_user_auctions(auctions);
		auctions_dispose(auctions);
	}
	
	return false;
}


static bool make_offer(void)
{
	float import;
	code_t object_code;
	
	get_input("\033[0;32mInsert object's code:\033[0;0m ", CODE_LEN, object_code, false);
	get_number("\033[0;32mInsert import:\033[0;m ", "%f", &import);

	if (do_make_offer(user_cf, import, object_code) && yes_or_no("\033[0;32mDo you want to set a controffer on this object?\033[0;0m]", 'y', 'n', true, true))
		set_counteroffer(object_code);

	return false;
}


static void set_counteroffer(code_t object_code)
{
	float import;
	
	get_number("\033[0;32mInsert import:\033[0;0m ", "%f", &import);
	do_set_counteroffer(user_cf, import, object_code);
}


static bool auctions_in(void)
{
	auction_t *auctions = view_auctions_in_progress_informations();
	
	if(auctions != NULL) {
		print_auctions(auctions, "AUCTIONS IN PROGRESS");
		auctions_dispose(auctions);
	}
	
	return false;
}


static bool auctions_done(void)
{
	auction_t *auctions = view_closed_auctions_informations();
	
	if(auctions != NULL) {
		print_auctions(auctions, "CLOSED AUCTIONS");
		auctions_dispose(auctions);
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
	{.action = OFFER, .control = make_offer},
	{.action = USER_AUCTIONS, .control = user_auctions},
	{.action = AUCTIONS_IN_PROGRESS, .control = auctions_in},
	{.action = CLOSED_AUCTIONS, .control = auctions_done},
	{.action = QUIT, .control = quit}
};


void user_controller(cf_t cf)
{
	db_switch_to_user();

	strcpy(user_cf, cf);

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
