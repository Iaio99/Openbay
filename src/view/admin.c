#include <stdio.h>
#include <string.h>

#include "admin.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_administrator_action(void)
{
	char options[3] = {'1', '2', '3'};
	char op;

	clear_screen();
	puts("\033[0;31m");
	puts("*********************************");
	puts("*        ADMIN DASHBOARD        *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Add a new object and make an asta");
	puts("2) Add a new category");
	puts("3) Quit\033[0;0m");

	op = multi_choice("\033[0;31mSelect an option\033[0;0m", options, 3);
	return op - '1';
}