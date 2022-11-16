#include <stdio.h>
#include <string.h>

#include "piscina.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_pool_action(void)
{
	char options[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'a'};
	char op;

	clear_screen();
	puts("*******************************************");
	puts("*        POOL SECRETARY DASHBOARD         *");
	puts("*******************************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Insert new user");
	puts("2) Insert new contact of a user");
	puts("3) Insert new certificate of a signed");
	puts("4) Insert new course");
	puts("5) Insert new subscription to a course of a signed ");
	puts("6) Remove course");
	puts("7) Remove user");
	puts("8) Update last visit of a free swimmer");
	puts("9) Modify lessons of a course");
	puts("a) Quit");

	op = multi_choice("Select an option", options, 10);
	return op - '1';
}