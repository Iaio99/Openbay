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
	check_env_failing("ADMIN_USER");
	check_env_failing("ADMIN_PASS");
	check_env_failing("PISCINA_USER");
	check_env_failing("PISCINA_PASS");
	check_env_failing("INSEGNANTE_USER");
	check_env_failing("INSEGNANTE_PASS");
	check_env_failing("ANONYMOUS_USER");
	check_env_failing("ANONYMOUS_PASS");
	
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
		do {
			if(!login())
				fprintf(stderr, "Login unsuccessful\n");
			db_switch_to_login();
		} while(ask_for_relogin());
	}
	fini_db();
	fini_validation();

	puts("Bye!");
	return 0;
}
