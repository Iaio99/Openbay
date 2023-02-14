#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "admin.h"
#include "../model/db.h"
#include "../view/admin.h"
#include "../utils/io.h"


char randchar(int first_letter, int last_letter)
{
	char c;
	srand(time(NULL));
	c = rand() % (last_letter + 1 - first_letter) + first_letter;

	return c;
}


void randcode(code_t code)
{
	code[0] = randchar('a', 'z');
	code[1] = randchar('a', 'z');
	code[2] = randchar('a', 'z');
	code[3] = '-';
	code[4] = randchar('0', '9');
	code[5] = randchar('0', '9');
	code[6] = randchar('0', '9');
	printf(code);
}


static bool add_new_object(void)
{
	object_t object;
	unsigned char duration;

//	randcode(object.code);

	memset(&object, 0, sizeof(object));

	get_input("State: ", STATE_LEN, object.state, false);
	get_number("Lenght: ", "%hu", &object.lenght);
	get_number("Width: ", "%hu", &object.width);
	get_number("Height: ", "%hu", &object.height);
	get_input("Description: ", DESCRIPTION_LEN, object.description, false);
	get_number("Base d'asta: ", "%f", &object.start_price);
	get_input("First level of category: ", LEVEL_LEN, object.category.first_level, false);
	get_input("Second level of category: ", LEVEL_LEN, object.category.second_level, false);
	get_input("Third level of category: ", LEVEL_LEN, object.category.third_level, false);
	
	get_number("Asta Duration [1-7]: ", "%cu", &duration);

	if (duration > '7' || duration < '1')
		return false;

	duration -= 48;

	do_indici_asta(object, duration);

    return false;
}


static bool add_new_category(void)
{
	category_t category;

	get_input("First Level: ", LEVEL_LEN, category.first_level, false);
	get_input("Second Level: ", LEVEL_LEN, category.second_level, false);
	get_input("Third Level: ", LEVEL_LEN, category.third_level, false);
	
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