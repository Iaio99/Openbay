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
	validate_cf(user->cf);
	get_input("Name: ", NOME_LEN, user->name, false);
	get_input("Surname: ", NOME_LEN, user->surname, false);
	get_input("Address: ", ADDRESS_LEN, user->address, false);
	get_input("Birthday: ", DATE_LEN, user->birthday, false);
	validate_date(user->birthday);
	get_input("Birthcity: ", CITY_NAME_LEN, user->birthcity, false);
	get_input("Credit Card Number: ", CREDIT_CARD_NUMBER_LEN, credit_card->number, false);
	get_input("Credit Card CVV: ", sizeof(cvv)/sizeof(cvv[0]), cvv, false);
	validate_cvv(cvv);
	credit_card->cvv = atoi(cvv);
	get_input("Credit Card Expiration Date: ", DATE_LEN, credit_card->expiration_date, false);
	validate_date(credit_card->expiration_date);
}

bool ask_for_relogin(void)
{
	return yes_or_no("Do you want to log in as a different user?", 'y', 'n', false, true);
}
