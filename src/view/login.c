#include <stdio.h>

#include "login.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_login_action()
{
	char options[3] = {'1', '2', '3'};
	char op;

	clear_screen();
	puts("**********************************");
	puts("*              HOME              *");
	puts("**********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Login");
	puts("2) Sign In");
	puts("3) Quit");

	op = multi_choice("Select an option", options, 3);
	return op - '1';
}

void view_login(credentials_t *cred)
{
	clear_screen();
	puts("*************************************");
	puts("*               LOGIN               *");
	puts("*************************************\n");
	get_input("Username: ", USERNAME_LEN, cred->username, false);
	get_input("Password: ", PASSWORD_LEN, cred->password, true);
}

void view_registration(credentials_t *cred, user_t *user, credit_card_t *credit_card)
{
	char cvv[sizeof(unsigned short int)];

	clear_screen();
	puts("**************************************");
	puts("*            REGISTRATION            *");
	puts("**************************************\n");
	get_input("Username: ", USERNAME_LEN, cred->username, false);
	get_input("Password: ", PASSWORD_LEN, cred->password, true);


	get_input("CF: ", CF_LEN, user->cf, false);
	get_input("Name: ", NOME_LEN, user->name, false);
	get_input("Surname: ", NOME_LEN, user->surname, false);
	get_input("Address: ", ADDRESS_LEN, user->address, false);

	while(true) {
		if(validate_date(get_input("Birthday [YYYY-MM-DD]: ", DATE_LEN, user->birthday, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}

	get_input("Birthcity: ", CITY_NAME_LEN, user->birthcity, false);
	get_input("Credit Card Number [XXXX-XXXX-XXXX-XXXX]: ", CREDIT_CARD_NUMBER_LEN, credit_card->number, false);
	get_input("Credit Card CVV [XXX]: ", sizeof(cvv)/sizeof(cvv[0]), cvv, false);
	credit_card->cvv = atoi(cvv);

	while(true) {
		if(validate_date(get_input("Credit Card Expiration Date [YYYY-MM-DD]: ", DATE_LEN, credit_card->expiration_date, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}
}

bool ask_for_relogin(void)
{
	return yes_or_no("Do you want to log in as a different user?", 'y', 'n', false, true);
}
