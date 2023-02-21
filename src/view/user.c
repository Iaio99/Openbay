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
	puts("\033[0;32m*********************************");
	puts("*        USER DASHBOARD         *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Make an offer");
	puts("2) View status of my auctions");
	puts("3) View auctions in progress");
	puts("4) View objects awarded");
	puts("5) Quit");
	puts("\033[0;0m");

	op = multi_choice("\033[0;32mSelect an option\033[0;0m", options, 5);
	return op - '1';
}

void print_auctions(auction_t *auctions, char *message)
{
	clear_screen();
	printf("\033[0;32m*** %s  ***\033[0;0m\n\n", message);

	for(size_t i = 0; i < auctions->num_entries; i++) {
		printf("%s %s %hu %hu %hu %s %.2f %s %s %s %s %.2f %hu\n",
			auctions->auctions[i].object.code,
			auctions->auctions[i].object.state,
			auctions->auctions[i].object.lenght,
			auctions->auctions[i].object.width,
			auctions->auctions[i].object.height,
			auctions->auctions[i].object.description,
			auctions->auctions[i].object.start_price,
			auctions->auctions[i].end,
			auctions->auctions[i].object.category.first_level,
			auctions->auctions[i].object.category.second_level,
			auctions->auctions[i].object.category.third_level,
			auctions->auctions[i].max_offer,
			auctions->auctions[i].number_offers
		);
	}
}

void print_user_auctions(auction_t *auctions)
{
	clear_screen();
	puts("\033[0;32m*** YOUR AUCTIONS INFORMATIONS ***\033[0;0m\n\n");

	for(size_t i = 0; i < auctions->num_entries; i++) {
		printf("%s %s %s %.2f %hu\n",
			auctions->auctions[i].object.code,
			auctions->auctions[i].object.description,
			auctions->auctions[i].end,
			auctions->auctions[i].max_offer,
			auctions->auctions[i].number_offers
		);
	}
}
