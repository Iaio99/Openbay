#include <stdio.h>

#include "user.h"
#include "../utils/io.h"
#include "../utils/validation.h"

int get_teacher_action(void)
{
	char options[3] = {'1','2', '3'};
	char op;

	clear_screen();
	puts("**********************************");
	puts("*       TEACHER DASHBOARD        *");
	puts("**********************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Print weekly report");
	puts("2) Print monthly report");
	puts("3) Quit");

	op = multi_choice("Select an option", options, 3);
	return op - '1';
}

void print_report(report_t *report)
{
	clear_screen();
	printf("** test **\n\n");

	for(size_t i = 0; i < report->num_entries; i++) {
		printf("%s %s %s\n",
			report->report[i].giorno,
			report->report[i].ora,
			report->report[i].nome_piscina);
	}
}
