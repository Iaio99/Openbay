#include <stdio.h>
#include <string.h>

#include "user.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_user_action(void)
{
	char options[5] = {'1', '2', '3', '4', '5'};
	char op;

	clear_screen();
	puts("\033[0;33m*********************************");
	puts("*        USER DASHBOARD         *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Do an offer");
	puts("2) See status of my aste");
	puts("3) See aste in doing");
	puts("4) See objects aggiudicated");
	puts("5) Quit");
	puts("\033[0;0m");

	op = multi_choice("\033[;33mSelect an option\033[;0m", options, 5);
	return op - '1';
}

void print_aste(asta_t *aste, char *message)
{
	clear_screen();
	printf("*** %s  ***\n\n", message);

	for(size_t i = 0; i < aste->num_entries; i++) {
		printf("%s %s %hu %hu %hu %s %.2f %s %s %s %s %.2f %hu\n",
			aste->aste[i].object.code,
			aste->aste[i].object.state,
			aste->aste[i].object.lenght,
			aste->aste[i].object.width,
			aste->aste[i].object.height,
			aste->aste[i].object.description,
			aste->aste[i].object.start_price,
			aste->aste[i].end,
			aste->aste[i].object.category.first_level,
			aste->aste[i].object.category.second_level,
			aste->aste[i].object.category.third_level,
			aste->aste[i].max_offer,
			aste->aste[i].number_offers
		);
	}
}

void print_my_aste(asta_t *aste)
{
	clear_screen();
	puts("*** YOUR ASTE INFORMATIONS ***\n\n");

	for(size_t i = 0; i < aste->num_entries; i++) {
		printf("%s %s %s %.2f %hu\n",
			aste->aste[i].object.code,
			aste->aste[i].object.description,
			aste->aste[i].end,
			aste->aste[i].max_offer,
			aste->aste[i].number_offers
		);
	}
}
