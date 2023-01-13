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
	role_t role = attempt_login(&cred);

	switch(role) {
		case ADMIN:
			administrator_controller();
			break;
		case USER:
			user_controller();
			break;
		default:
			return false;
	}

	return true;
}
