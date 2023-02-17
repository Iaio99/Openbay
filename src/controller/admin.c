#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "admin.h"
#include "../model/db.h"
#include "../view/admin.h"
#include "../utils/io.h"


static bool add_new_object(void)
{
	object_t object;
	unsigned char duration;

	memset(&object, 0, sizeof(object));

	get_input("\033[0;31mState: \033[0;0m", STATE_LEN, object.state, false);
	get_number("\033[0;31mLenght: \033[0;0m", "%hu", &object.lenght);
	get_number("\033[0;31mWidth: \033[0;0m", "%hu", &object.width);
	get_number("\033[0;31mHeight: \033[0;0m", "%hu", &object.height);
	get_input("\033[0;31mDescription: \033[0;0m", DESCRIPTION_LEN, object.description, false);
	get_number("\033[0;31mBase d'asta: \033[0;0m", "%f", &object.start_price);
	get_input("\033[0;31mFirst level of category: \033[0;0m", LEVEL_LEN, object.category.first_level, false);
	get_input("\033[0;31mSecond level of category: \033[0;0m", LEVEL_LEN, object.category.second_level, false);
	get_input("\033[0;31mThird level of category: \033[0;0m;", LEVEL_LEN, object.category.third_level, false);
	get_number("\033[0;31mAsta Duration [1-7]\033:[0;0m ", "%cu", &duration);

	duration -= 48;

	do_indici_asta(object, duration);

    return false;
}


static bool add_new_category(void)
{
	category_t category;

	get_input("\033[0;31mFirst Level:\033[0;0m ", LEVEL_LEN, category.first_level, false);
	get_input("\033[0;31mSecond Level:\033[0;0m ", LEVEL_LEN, category.second_level, false);
	get_input("\033[0;31mThird Level:\033[0;0m ", LEVEL_LEN, category.third_level, false);
	
	do_inserisci_categoria(category);

    return false;
}


static bool quit(void) {
	return true;
}


static struct {
	enum actions action;
	bool (*control)(void);
} controls[END_OF_ACTIONS] = {
	{.action = OBJECT_NEW, .control = add_new_object},
	{.action = CATEGORY_NEW, .control = add_new_category},
	{.action = QUIT, .control = quit}
};


void administrator_controller(void)
{
	db_switch_to_administrator();

	while(true) {
		int action = get_administrator_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}