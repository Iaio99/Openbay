#include <stdio.h>
#include <string.h>

#include "user.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_user_action(void)
{
	char options[3] = {'1', '2', '3'};
	char op;

	clear_screen();
	puts("\033[0;33m*********************************");
	puts("*        USER DASHBOARD         *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Print pools informations");
	puts("2) Print courses informations");
	puts("3) Quit");
	puts("4) Print pools informations");
	puts("5) Print courses informations");
	puts("6) Quit");
	puts("\033[0;0m");

	op = multi_choice("\033[;33mSelect an option\033[;0m", options, 3);
	return op - '1';
}

void print_aste(asta_t *aste)
{
	clear_screen();
	printf("** test **\n\n");

	for(size_t i = 0; i < aste->num_entries; i++) {
		printf("%s %s %hu %hu %hu %hu %s %f %s %s %s %hu %f\n",
			aste->aste[i].object.code,
			aste->aste[i].object.state,
			aste->aste[i].object.lenght,
			aste->aste[i].object.width,
			aste->aste[i].object.height,
			aste->aste[i].object.description,
			aste->aste[i].object.start_price
			aste->aste[i].object.category.first_level,
			aste->aste[i].object.category.second_level,
			aste->aste[i].object.category.third_level,
			aste->aste[i].number_offers;
			aste->aste[i].max_offer;
	}
}
