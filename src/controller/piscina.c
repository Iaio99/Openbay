#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "piscina.h"
#include "../model/db.h"
#include "../view/piscina.h"
#include "../utils/io.h"

char nome_piscina[NOME_LEN] = {0};

static bool add_user()
{
	int op;
	char options[2] = {'1', '2'};

	puts("*** What type of user are you adding? ***\n");
	puts("1) Nuotatore Libero");
	puts("2) Iscritto");

	op = multi_choice("Select an option", options, 2);

	user_t user;

	get_input("CF: ", CF_LEN, user.utente, false);
    get_input("Nome: ", NOME_LEN, user.nome, false);
	get_input("Indirizzo: ", INDIRIZZO_LEN, user.indirizzo, false);

	if (op == 2)
		get_input("Età: ", sizeof(unsigned char), &user.età, false);
		
	do_add_user(user, op - 1);

	add_contact(user.utente);

	if (op == 2)
		add_certificate();

	return false;
}

static bool add_contact(cf_t user)
{
	contact_t contact;
	strcpy(contact.utente, user);

	int op;
	char options[3] = {'1', '2', '3'};

	do {
		puts("*** What type of contact are you inserting? ***\n");
		puts("1) E-mail");
		puts("2) Telefono");
		puts("3) Cellulare");
		op = multi_choice("Select an option", options, 3);

		switch (op) {
			case 1:
				contact.tipo = "E-mail";
				break;
			case 2:
				contact.tipo = "Telefono";
				break;
			case 3:
				contact.tipo = "Cellulare";
				break;
			default:
				puts("Contact type not valid!\n");
		}
		get_input("add contact: ", RECAPITO_LEN, contact.recapito, false);
	}while(yes_or_no("Do you want to insert a new contact?", 'y', 'n', true, true));

	do_add_contact(contact);
	return false;
}


static bool add_certificate(void) {
	certificate_t certificate;

	get_input("CF Iscritto: ", CF_LEN, certificate.iscritto, false);
	get_input("Nome e cognome del medico: ", NOME_LEN, certificate.medico, false);

	do_add_certificate(certificate);

	return false;
}


static bool add_course(void) {
	struct course_entry course;
	strcpy(course.nome_piscina, nome_piscina);


	get_input("Nome Corso: ", NOME_LEN, course.nome_corso, false);
	get_input("Min_p: ", sizeof(char), &course.min_p, false);
	get_input("Nome Corso: ", sizeof(char), &course.max_p, false);
	printf("Nome Corso: ");
	scanf("%d", &course.costo);
	get_input("CF Insegnante: ", CF_LEN+1, course.insegnante, false);

	do_add_course(course, nome_piscina);
	add_lesson(course.nome_corso, nome_piscina);

	return false;
}

void add_lesson(char nome_corso[NOME_LEN], char nome_piscina[NOME_LEN])
{
	lesson_t lesson;
	strcpy(lesson.nome_corso, nome_corso);
	strcpy(lesson.nome_piscina, nome_piscina);

	do {
		get_input("Giorno: ", DATE_LEN, lesson.giorno, false);
		get_input("Ora: ", TIME_LEN	, lesson.ora, false);
	}while(yes_or_no("Do you want to insert a new lesson?", 'y', 'y', true, true));

	do_add_lesson(lesson);
}


bool add_subscription(void)
{
	char nome_corso[NOME_LEN];
	cf_t iscritto;
	get_input("Nome Corso: ", NOME_LEN, nome_corso, false);
	get_input("CF Iscritto: ", CF_LEN+1, iscritto, false);

	do_add_subscription(iscritto, nome_corso, nome_piscina);

	return false;
}


bool remove_course(void)
{
	char nome_corso[NOME_LEN];
	get_input("Nome Corso: ", NOME_LEN, nome_corso, false);

	do_remove_course(nome_corso, nome_piscina);

	return false;
}


bool remove_user(void)
{
	cf_t user;
	get_input("CF User: ", CF_LEN+1, user, false);

	do_remove_user(user);

	return false;
}


bool remove_subscription(void)
{
	char nome_corso[NOME_LEN];
	cf_t iscritto;

	get_input("Nome Corso: ", NOME_LEN, nome_corso, false);
	get_input("CF Iscritto: ", CF_LEN+1, iscritto, false);

	do_remove_subscription(iscritto, nome_corso, nome_piscina);
	
	return false;
}


bool update_last_visit(void)
{
	cf_t free_swimmer;
	get_input("CF Nuotatore Libero: ", CF_LEN+1, free_swimmer, false);

	do_update_last_visit(free_swimmer, nome_piscina);
	
	return false;
}


bool modify_lesson(void)
{
	lesson_t lesson;
	strcpy(lesson.nome_piscina, nome_piscina);

	get_input("Nome Corso: ", NOME_LEN, lesson.nome_corso, false);
	get_input("Giorno: ", DATE_LEN, lesson.giorno, false);
	get_input("Ora: ", TIME_LEN	, lesson.ora, false);

	do_modify_lesson(lesson);

	return false;
}


static bool quit(void) {
	return true;
}


static struct {
	enum actions action;
	bool (*control)(void);
} controls[END_OF_ACTIONS] = {
	{.action = ADD_USER, .control = add_user},
	{.action = ADD_CERTIFICATE, .control = add_certificate},
	{.action = ADD_COURSE, .control = add_course},
	{.action = ADD_SUBSCRIPTION, .control = add_subscription},
	{.action = REMOVE_COURSE, .control = remove_course},
	{.action = REMOVE_USER, .control = remove_user},
	{.action = REMOVE_SUBSCRIPTION, .control = remove_subscription},
	{.action = UPDATE_LAST_VISIT, .control = update_last_visit},
	{.action = MODIFY_LESSON, .control = modify_lesson},
	{.action = QUIT, .control = quit}
};


void pool_controller(char username_pool[USERNAME_LEN])
{
	strcpy(nome_piscina, username_pool);
	db_switch_to_pool();

	while(true) {
		int action = get_pool_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}
