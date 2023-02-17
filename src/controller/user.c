#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "user.h"
#include "../model/db.h"
#include "../view/user.h"
#include "../utils/io.h"


cf_t user_cf;


static bool user_aste(void)
{
	asta_t *aste = do_stato_aste_utente(user_cf);
	
	if(aste != NULL) {
		print_my_aste(aste);
		aste_dispose(aste);
	}
	
	return false;
}


static bool offer(void)
{
	float import;
	code_t object_code;
	
	get_input("\033[0;32mInsert object's code: \033[0;0m", CODE_LEN, object_code, false);
	get_number("\033[0;32mInsert import: [0;m", "%f", &import);

	if (do_fai_offerta(user_cf, import, object_code) && yes_or_no("\033[0;32mDo you want to set a controffer on this object?\033[0;0m]", 'y', 'n', true, true))
		set_controffer(object_code);

	return false;
}


static void set_controffer(code_t object_code)
{
	float import;
	
	get_number("\033[0;32mInsert import: \033[0;0m]", "%f", &import);
	do_imposta_controfferta(user_cf, import, object_code);
}


static bool aste_in(void)
{
	asta_t *aste = do_visualizza_oggetti_asta();
	
	if(aste != NULL) {
		print_aste(aste, "ASTE IN DOING");
		aste_dispose(aste);
	}
	
	return false;
}


static bool aste_done(void)
{
	asta_t *aste = do_visualizza_aste_passate();
	
	if(aste != NULL) {
		print_aste(aste, "ASTE FINISHED");
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
	{.action = OFFER, .control = offer},
	{.action = USER_ASTE, .control = user_aste},
	{.action = ASTE_IN_CORSO, .control = aste_in},
	{.action = ASTE_FINITE, .control = aste_done},
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
