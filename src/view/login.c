#include <stdio.h>

#include "login.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_login_action()
{
	char options[3] = {'1', '2', '3'};
	char op;

	clear_screen();
	puts("\033[0;33m**********************************");
	puts("*              HOME              *");
	puts("**********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Login");
	puts("2) Sign In");
	puts("3) Quit\033[0;0m");

	op = multi_choice("\033[0;33mSelect an option\033[0;0m", options, 3);
	return op - '1';
}

void view_login(credentials_t *cred)
{
	clear_screen();
	puts("\033[0;33m*************************************");
	puts("*               LOGIN               *");
	puts("*************************************\033[0;0m\n");
	get_input("\033[0;33mUsername:\033[0;0m ", USERNAME_LEN, cred->username, false);
	get_input("\033[0;33mPassword:\033[0;0m ", PASSWORD_LEN, cred->password, true);
}

void view_registration(credentials_t *cred, user_t *user, credit_card_t *credit_card)
{
	char cvv[sizeof(unsigned short int)];

	clear_screen();
	puts("\033[0;33m**************************************");
	puts("*            REGISTRATION            *");
	puts("**************************************\033[0;0m\n");

	get_input("\033[0;33mUsername:\033[0;0m ", USERNAME_LEN, cred->username, false);
	get_input("\033[0;33mPassword:\033[0;0m ", PASSWORD_LEN, cred->password, true);

	get_input("\033[0;33mCF:\033[0;0m ", CF_LEN, user->cf, false);
	get_input("\033[0;33mName:\033[0;0m ", NOME_LEN, user->name, false);
	get_input("\033[0;33mSurname:\033[0;0m ", NOME_LEN, user->surname, false);
	get_input("\033[0;33mAddress:\033[0;0m ", ADDRESS_LEN, user->address, false);

	while(true) {
		if(validate_date(get_input("\033[0;33mBirthday [YYYY-MM-DD]:\033[0;0m ", DATE_LEN, user->birthday, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}

	get_input("\033[0;33mBirthcity:\033[0;0m ", CITY_NAME_LEN, user->birthcity, false);
	get_input("\033[0;33mCredit Card Number [XXXX-XXXX-XXXX-XXXX]:\033[0;0m ", CREDIT_CARD_NUMBER_LEN, credit_card->number, false);
	get_input("\033[0;33mCredit Card CVV [XXX]:\033[0;0m ", sizeof(cvv)/sizeof(cvv[0]), cvv, false);
	credit_card->cvv = atoi(cvv);

	while(true) {
		if(validate_date(get_input("\033[0;33mCredit Card Expiration Date [YYYY-MM-DD]:\033[0;0m ", DATE_LEN, credit_card->expiration_date, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}
}

bool ask_for_relogin(void)
{
	return yes_or_no("\033[0;33mDo you want to log in as a different user?\033[0;0m", 'y', 'n', false, true);
}
