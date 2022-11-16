#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "addetto_comunale.h"
#include "../model/db.h"
#include "../view/addetto_comunale.h"
#include "../utils/io.h"

static bool end_job(void)
{
	cf_t insegnante;
	
	get_input("CF insegnante: ", CF_LEN, insegnante, false);
	do_end_job(insegnante);

    return false;
}

static bool add_new_job(void)
{
	job_t job;

	get_input("CF insegnante: ", CF_LEN, job.insegnante, false);
	get_input("Data Inizio: ", DATE_LEN, job.data_inizio, false);
	get_input("Nome piscina: ", NOME_LEN, job.nome_piscina, false);
	
	do_add_new_job(job);

    return false;
}

static bool add_title(void)
{
	cf_t insegnante;
	char title[TITLE_LEN];
	
	get_input("CF insegnante: ", CF_LEN, insegnante, false);
	get_input("New qualification got: ", TITLE_LEN, title, false);
	
	do_add_title(insegnante, title);

    return false;
}

static bool update_manager(void)
{
	char pool_name[NOME_LEN];
	char new_manager[NOME_LEN];

	get_input("Pool name: ", NOME_LEN, pool_name, false);
	get_input("New manager name: ", NOME_LEN, new_manager, false);

	do_update_manager(pool_name, new_manager);

    return false;
}

static bool quit(void) {
	return true;
}


static struct {
	enum actions action;
	bool (*control)(void);
} controls[END_OF_ACTIONS] = {
	{.action = JOB_END, .control = end_job},
	{.action = JOB_NEW, .control = add_new_job},
	{.action = TITLE_NEW, .control = add_title},
	{.action = UPDATE_MANAGER, .control = update_manager},
	{.action = QUIT, .control = quit}
};


void administrator_controller(void)
{
	db_switch_to_administrator();

	while(true) {
		int action = get_administrator_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}
