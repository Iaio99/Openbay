#include <stdio.h>

#include "view/login.h"
#include "controller/login.h"
#include "model/db.h"
#include "utils/dotenv.h"
#include "utils/io.h"
#include "utils/validation.h"


#define check_env_failing(varname)                                          \
if(getenv((varname)) == NULL) {                                             \
        fprintf(stderr, "[FATAL] env variable %s not set\n", (varname));    \
        ret = false;                                                        \
}
static bool validate_dotenv(void)
{
	bool ret = true;

	check_env_failing("HOST");
	check_env_failing("PORT");
	check_env_failing("DB");
	check_env_failing("LOGIN_USER");
	check_env_failing("LOGIN_PASS");
	check_env_failing("USER_USER");
	check_env_failing("USER_PASS");
	check_env_failing("ADMIN_USER");
	check_env_failing("ADMIN_PASS");

	return ret;
}
#undef set_env_failing

int main()
{

	if(env_load(".", false) != 0)
		return 1;

	if(!validate_dotenv())
		return 1; 
	
	if(!init_validation())
		return 1;

	if(!init_db())
		return 1;

	if(initialize_io()) {
		int op;
		op = get_login_action();
		switch (op) {
			case 0:
				do {
					if(!login())
						fprintf(stderr, "Login unsuccessful\n");
					db_switch_to_login();
				} while(ask_for_relogin());
				break;
			case 1:
				register_user();
				break;
			case 2:
			default:
				break;
		}
	}
	fini_db();
	fini_validation();

	puts("Bye!");
	return 0;
}
