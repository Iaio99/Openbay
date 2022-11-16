#include <stdio.h>
#include <string.h>

#include "user.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_user_action(void)
{
	char options[3] = {'1', '2', '3'};
	char op;

	clear_screen();
	puts("\033[0;33m*********************************");
	puts("*        USER DASHBOARD         *");
	puts("*********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Print pools informations");
	puts("2) Print courses informations");
	puts("3) Quit");
	puts("\033[0;0m");

	op = multi_choice("\033[;33mSelect an option\033[;0m", options, 3);
	return op - '1';
}

void print_pool(pool_t *pool)
{
	clear_screen();
	printf("** test **\n\n");

	for(size_t i = 0; i < pool->num_entries; i++) {
		printf("%s %s %s %s %s %s\n",
			pool->pool[i].nome_piscina,
			pool->pool[i].indirizzo,
			pool->pool[i].telefono,
			pool->pool[i].nome_corso,
			pool->pool[i].apertura,
			pool->pool[i].chiusura);
	}
}

void print_course(course_t *course)
{
	clear_screen();
	printf("** test **\n\n");

	for(size_t i = 0; i < course->num_entries; i++) {
		printf("%s %d %d %d\n",
			course->course[i].nome_piscina,
			course->course[i].min_p,
			course->course[i].max_p,
			course->course[i].costo);
	}
}
