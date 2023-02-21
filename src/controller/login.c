#include <stdbool.h>
#include <stdio.h>

#include "login.h"
#include "user.h"
#include "admin.h"
#include "../view/login.h"
#include "../model/db.h"


bool login(void)
{
	credentials_t cred;

	view_login(&cred);

	struct login_data data = attempt_login(&cred);

	switch(data.role) {
		case ADMIN:
			administrator_controller();
			break;
		case USER:
			user_controller(data.cf);
			break;
		default:
			return false;
	}

	return true;
}

bool register_user(void)
{
	credentials_t cred;
	user_t user;
	credit_card_t credit_card;

	view_registration(&cred, &user, &credit_card);
	do_user_registration(user, cred, credit_card);
}