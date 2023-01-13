#include <stdio.h>
#include <string.h>

#include "admin.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_administrator_action(void)
{
	char options[5] = {'1', '2', '3', '4', '5'};
	char op;

	clear_screen();
	puts("*********************************");
	puts("*        ADMIN DASHBOARD        *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Set job end of a teacher");
	puts("2) Add a qualification to a teacher");
	puts("3) Add new job for a teacher");
	puts("4) Change manager of a pool");
	puts("5) Quit");

	op = multi_choice("Select an option", options, 5);
	return op - '1';
}