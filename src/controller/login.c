#include <stdbool.h>
#include <stdio.h>

#include "login.h"
#include "user.h"
#include "teacher.h"
#include "addetto_comunale.h"
#include "piscina.h"
#include "../view/login.h"
#include "../model/db.h"


bool login(void)
{
	credentials_t cred;

	view_login(&cred);
	role_t role = attempt_login(&cred);

	switch(role) {
		case ADDETTO_COMUNALE:
			administrator_controller();
			break;
		case PISCINA:
			pool_controller(cred.username);
			break;
		case INSEGNANTE:
			teacher_controller();
			break;
		case USER:
			user_controller();
			break;
		default:
			return false;
	}

	return true;
}
