#include <stdbool.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "validation.h"

regex_t regex_date;
regex_t regex_time;
regex_t regex_cf;
regex_t regex_credit_card_number;
regex_t regex_cvv;


static void print_regerror(int errcode, size_t length, regex_t *compiled)
{
	char buffer[length];
	(void) regerror(errcode, compiled, buffer, length);
	fprintf(stderr, "Regex match failed: %s\n", buffer);
}


bool init_validation(void)
{
	int ret1;
	int ret2;
	int ret3;
	int ret4;
	int ret5;

	ret1 = regcomp(&regex_date, "^([12][0-9]{3}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01]))", REG_EXTENDED);
	if(ret1) {
		if(ret1 == REG_ESPACE) {
			fprintf(stderr, "%s\n", strerror(ENOMEM));
		} else {
			fprintf(stderr, "Fatal error while setting up date validation regex.\n");
		}
	}

	ret2 = regcomp(&regex_time, "^([0-1]?[0-9]|2?[0-3]|[0-9]):([0-5][0-9]|[0-9])$", REG_EXTENDED);
	if(ret2) {
		if(ret2 == REG_ESPACE) {
			fprintf(stderr, "%s\n", strerror(ENOMEM));
		} else {
			fprintf(stderr, "Fatal error while setting up time validation regex.\n");
		}
	}

	ret3 = regcomp(&regex_cf, "^(([A-Z]|[a-z]){6}[0-9]{2}([A-Z]|[a-z])[0-9]{2}([A-Z]|[a-z])[0-9]{3}([A-Z]|[a-z]))", REG_EXTENDED);
	if(ret3) {
		if(ret3 == REG_ESPACE) {
			fprintf(stderr, "%s\n", strerror(ENOMEM));
		} else {
			fprintf(stderr, "Fatal error while setting up time validation regex.\n");
		}
	}

	ret4 = regcomp(&regex_credit_card_number, "^([0-9]{4}-[0-9]{4}-[0-9]{4}-[0-9]{4})", REG_EXTENDED);
	if (ret4) {
		if(ret4 == REG_ESPACE) {
			fprintf(stderr, "%s\n", strerror(ENOMEM));
		} else {
			fprintf(stderr, "Fatal error while setting up time validation regex.\n");
		}
	}

	ret5 = regcomp(&regex_cvv, "^([0-9]{3})", REG_EXTENDED);
	if (ret5) {
		if(ret5 == REG_ESPACE) {
			fprintf(stderr, "%s\n", strerror(ENOMEM));
		} else {
			fprintf(stderr, "Fatal error while setting up time validation regex.\n");
		}
	}

	return ret1 == 0 && ret2 == 0 && ret3 == 0 && ret4 == 0 && ret5 == 0;
}


void fini_validation(void)
{
	regfree(&regex_time);
	regfree(&regex_date);
}



bool validate_cf (char *str)
{
	int ret = regexec(&regex_cf, str, 0, NULL, 0);

	if (ret != 0 && ret != REG_NOMATCH) {
		size_t lenght = regerror(ret, &regex_cf, NULL, 0);
		print_regerror(ret, lenght, &regex_cf);
		return false;
	}

	return ret == 0;
}


bool validate_cvv (char *str)
{
	int ret = regexec(&regex_cvv, str, 0, NULL, 0);

	if (ret != 0 && ret != REG_NOMATCH) {
		size_t lenght = regerror(ret, &regex_cvv, NULL, 0);
		print_regerror(ret, lenght, &regex_cvv);
		return false;
	}

	return ret == 0;
}


bool validate_credit_card_number (char *str)
{
	int ret = regexec(&regex_credit_card_number, str, 0, NULL, 0);

	if (ret != 0 && ret != REG_NOMATCH) {
		size_t lenght = regerror(ret, &regex_credit_card_number, NULL, 0);
		print_regerror(ret, lenght, &regex_credit_card_number);
		return false;
	}

	return ret == 0;
}


bool validate_date(char *str)
{
	int ret = regexec(&regex_date, str, 0, NULL, REG_NOTEOL);

	if(ret != 0 && ret != REG_NOMATCH) {
		size_t length = regerror(ret, &regex_date, NULL, 0);
		print_regerror(ret, length, &regex_date);
		return false;
	}
	return ret == 0;
}


bool validate_time(char *str)
{
	int ret = regexec(&regex_time, str, 0, NULL, 0);

	if(ret != 0 && ret != REG_NOMATCH) {
		size_t length = regerror(ret, &regex_time, NULL, 0);
		print_regerror(ret, length, &regex_time);
		return false;
	}
	return ret == 0;
}
