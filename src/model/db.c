#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <assert.h>

#include "db.h"
#include "../utils/db.h"

static MYSQL *conn;

static MYSQL_STMT *login_procedure;
static MYSQL_STMT *fai_offerta;
static MYSQL_STMT *imposta_controfferta;
static MYSQL_STMT *indici_asta;
static MYSQL_STMT *inserisci_categoria;
static MYSQL_STMT *stato_aste_utente;
static MYSQL_STMT *user_registration;
static MYSQL_STMT *visualizza_aste_passate;
static MYSQL_STMT *visualizza_oggetti_asta;


static void close_prepared_stmts(void)
{
	if(login_procedure) {
		mysql_stmt_close(login_procedure);
		login_procedure = NULL;
	}
	if(fai_offerta) {
		mysql_stmt_close(fai_offerta);
		fai_offerta = NULL;
	}
	if(imposta_controfferta) {
		mysql_stmt_close(imposta_controfferta);
		imposta_controfferta = NULL;
	}
	if(indici_asta) {
		mysql_stmt_close(indici_asta);
		indici_asta = NULL;
	}
	if(inserisci_categoria) {
		mysql_stmt_close(inserisci_categoria);
		inserisci_categoria = NULL;
	}
	if(stato_aste_utente) {
		mysql_stmt_close(stato_aste_utente);
		stato_aste_utente = NULL;
	}
	if(user_registration) {
		mysql_stmt_close(user_registration);
		user_registration = NULL;
	}
	if(visualizza_aste_passate) {
		mysql_stmt_close(visualizza_aste_passate);
		visualizza_aste_passate = NULL;
	}
	if(visualizza_oggetti_asta) {
		mysql_stmt_close(visualizza_oggetti_asta);
		visualizza_oggetti_asta = NULL;
	}
}


static bool initialize_prepared_stmts(role_t for_role)
{
	switch(for_role) {

		case LOGIN_ROLE:
			if(!setup_prepared_stmt(&login_procedure, "call login(?, ?, ?)", conn)) {
				print_stmt_error(login_procedure, "Unable to initialize Login statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&user_registration, "call user_registration(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(user_registration, "Unable to initialize Info Corso statement\n");
				return false;
			}
			break;
		
		case ADMIN:
			if(!setup_prepared_stmt(&indici_asta, "call indici_asta(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(indici_asta, "Unable to initialize End Job statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&inserisci_categoria, "call inserisci_categoria(?, ?, ?)", conn)) {
				print_stmt_error(inserisci_categoria, "Unable to initialize Add Title statement\n");
				return false;
			}
			break;

		case USER:
			if(!setup_prepared_stmt(&fai_offerta, "call fai_offerta(?, ?, ?)", conn)) {
				print_stmt_error(fai_offerta, "Unable to initialize register Info Piscine statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&imposta_controfferta, "call imposta_controfferta(?, ?, ?)", conn)) {
				print_stmt_error(imposta_controfferta, "Unable to initialize Info Corso statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&stato_aste_utente, "call stato_aste_utente(?)", conn)) {
				print_stmt_error(stato_aste_utente, "Unable to initialize register Info Piscine statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&visualizza_aste_passate, "call visualizza_aste_passate()", conn)) {
				print_stmt_error(visualizza_aste_passate, "Unable to initialize register Info Piscine statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&visualizza_oggetti_asta, "call visualizza_oggetti_asta()", conn)) {
				print_stmt_error(visualizza_oggetti_asta, "Unable to initialize Info Corso statement\n");
				return false;
			}		
			break;

		default:
			fprintf(stderr, "[FATAL] Unexpected role to prepare statements.\n");
			exit(EXIT_FAILURE);
	}

	return true;
}


bool init_db(void)
{
	unsigned int timeout = 300;
	bool reconnect = true;

	conn = mysql_init(NULL);

	if(conn == NULL) {
		finish_with_error(conn, "mysql_init() failed (probably out of memory)\n");
	}

	if(mysql_real_connect(conn, getenv("HOST"), getenv("LOGIN_USER"), getenv("LOGIN_PASS"), getenv("DB"),
			      atoi(getenv("PORT")), NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}

	if (mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout)) {
		print_error(conn, "[mysql_options] failed.");
	}

	if(mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect)) {
		print_error(conn, "[mysql_options] failed.");
	}

#ifndef NDEBUG
	mysql_debug("d:t:O,/tmp/client.trace");

	if(mysql_dump_debug_info(conn)) {
		print_error(conn, "[debug_info] failed.");
	}
#endif

	return initialize_prepared_stmts(LOGIN_ROLE);
}


void fini_db(void)
{
	close_prepared_stmts();
	mysql_close(conn);
}


role_t attempt_login(struct credentials *cred)
{
	MYSQL_BIND param[3]; // Used both for input and output
	int role = 0;

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cred->username, strlen(cred->username));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, cred->password, strlen(cred->password));
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &role, sizeof(role));

	if(mysql_stmt_bind_param(login_procedure, param) != 0) {
 		// Note _param
		print_stmt_error(login_procedure, "Could not bind parameters for login");
		role = FAILED_LOGIN;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(login_procedure) != 0) {
		print_stmt_error(login_procedure, "Could not execute login procedure");
		role = FAILED_LOGIN;
		goto out;
	}

	// Prepare output parameters
	set_binding_param(&param[0], MYSQL_TYPE_LONG, &role, sizeof(role));

	if(mysql_stmt_bind_result(login_procedure, param)) {
		print_stmt_error(login_procedure, "Could not retrieve output parameter");
		role = FAILED_LOGIN;
		goto out;
	}

	// Retrieve output parameter
	if(mysql_stmt_fetch(login_procedure)) {
		print_stmt_error(login_procedure, "Could not buffer results");
		role = FAILED_LOGIN;
		goto out;
	}

    out:
	// Consume the possibly-returned table for the output parameter
	while(mysql_stmt_next_result(login_procedure) != -1) {}

	mysql_stmt_free_result(login_procedure);
	mysql_stmt_reset(login_procedure);
		return role;
}


void db_switch_to_login(void)
{
	close_prepared_stmts();
	if(mysql_change_user(conn, getenv("LOGIN_USER"), getenv("LOGIN_PASS"), getenv("DB"))) {
		fprintf(stderr, "mysql_change_user() failed: %s\n", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
	if(!initialize_prepared_stmts(LOGIN_ROLE)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
		exit(EXIT_FAILURE);
	}
}


void db_switch_to_administrator(void)
{
	close_prepared_stmts();
	if(mysql_change_user(conn, getenv("ADMINISTRATOR_USER"), getenv("ADMINISTRATOR_PASS"), getenv("DB"))) {
		fprintf(stderr, "mysql_change_user() failed: %s\n", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
	if(!initialize_prepared_stmts(ADMIN)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
		exit(EXIT_FAILURE);
	}
}


void db_switch_to_user(void)
{
	close_prepared_stmts();
	if(mysql_change_user(conn, getenv("ANONYMOUS_USER"), getenv("ANONYMOUS_PASS"), getenv("DB"))) {
		fprintf(stderr, "mysql_change_user() failed: %s\n", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
	if(!initialize_prepared_stmts(USER)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
		exit(EXIT_FAILURE);
	}
}

/*
course_t *do_view_course(char nome_corso[NOME_LEN])
{
	int status;
	size_t row = 0;
	MYSQL_BIND param[5];
	char nome_piscina[NOME_LEN];
	int min_p;
	int max_p;
	int costo;
	course_t *course = NULL;

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, nome_corso, strlen(nome_corso));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, nome_piscina, NOME_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &costo, sizeof(costo));
	set_binding_param(&param[3], MYSQL_TYPE_TINY, &max_p, sizeof(max_p));
	set_binding_param(&param[4], MYSQL_TYPE_TINY, &min_p, sizeof(min_p));
	
	if(mysql_stmt_bind_param(view_course, param)) {
		print_stmt_error(view_course, "Unable to bind output parameters for get course\n");
		free(course);
		course = NULL;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(view_course) != 0) {
		print_stmt_error(view_course, "Could not execute get course procedure");
		goto out;
	}

	mysql_stmt_store_result(view_course);

	course = malloc(sizeof(*course) + sizeof(struct course_entry) * mysql_stmt_num_rows(view_course));
	if(course == NULL)
		goto out;
	memset(course, 0, sizeof(*course) + sizeof(struct course_entry) * mysql_stmt_num_rows(view_course));
	course->num_entries = mysql_stmt_num_rows(view_course);

	// Get bound parameters
	mysql_stmt_store_result(view_course);
	
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, nome_piscina, NOME_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &costo, sizeof(costo));
	set_binding_param(&param[3], MYSQL_TYPE_TINY, &max_p, sizeof(max_p));
	set_binding_param(&param[4], MYSQL_TYPE_TINY, &min_p, sizeof(min_p));

	if(mysql_stmt_bind_result(view_course, param)) {
		print_stmt_error(view_course, "Unable to bind output parameters for get course\n");
		free(course);
		course = NULL;
		goto out;
	}

	while (true) {
		status = mysql_stmt_fetch(view_course);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy(course->course[row].nome_piscina, nome_piscina);
		course->course[row].costo = costo;
		course->course[row].max_p = max_p;
		course->course[row].min_p = min_p;

		row++;
	}
    out:
	mysql_stmt_free_result(view_course);
	mysql_stmt_reset(view_course);
	return course;
}


void course_dispose(course_t *course)
{
	free(course);
}


pool_t *do_view_pool()
{
	int status;
	size_t row = 0;
	MYSQL_BIND param[6];
	char nome_piscina[NOME_LEN];
	char telefono[TELEFONO_LEN];
	char indirizzo[INDIRIZZO_LEN];
	MYSQL_TIME apertura;
	MYSQL_TIME chiusura;
	char nome_corso[NOME_LEN];

	pool_t *pool = NULL;

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, nome_piscina, NOME_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, indirizzo, INDIRIZZO_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, telefono, TELEFONO_LEN);
	set_binding_param(&param[3], MYSQL_TYPE_DATE, &apertura, sizeof(chiusura));
	set_binding_param(&param[4], MYSQL_TYPE_DATE, &chiusura, sizeof(apertura));
	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, nome_corso, NOME_LEN);

	if(mysql_stmt_bind_param(view_pool, param)) {
		print_stmt_error(view_pool, "Unable to bind output parameters for get pool\n");
		free(pool);
		pool = NULL;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(view_pool) != 0) {
		print_stmt_error(view_pool, "Could not execute get pool procedure");
		goto out;
	}

	mysql_stmt_store_result(view_pool);

	pool = malloc(sizeof(*pool) + sizeof(struct pool_entry) * mysql_stmt_num_rows(view_pool));
	if(pool == NULL)
		goto out;
	memset(pool, 0, sizeof(*pool) + sizeof(struct pool_entry) * mysql_stmt_num_rows(view_pool));
	pool->num_entries = mysql_stmt_num_rows(view_pool);

	// Get bound parameters
	mysql_stmt_store_result(view_pool);

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, nome_piscina, NOME_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, indirizzo, INDIRIZZO_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, telefono, TELEFONO_LEN);
	set_binding_param(&param[3], MYSQL_TYPE_DATE, &apertura, sizeof(apertura));
	set_binding_param(&param[4], MYSQL_TYPE_DATE, &chiusura, sizeof(chiusura));
	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, nome_corso, NOME_LEN);

	if(mysql_stmt_bind_result(view_pool, param)) {
		print_stmt_error(view_pool, "Unable to bind output parameters for get pool\n");
		free(pool);
		pool = NULL;
		goto out;
	}


	while (true) {
		status = mysql_stmt_fetch(view_pool);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy(pool->pool[row].nome_piscina, nome_piscina);
		strcpy(pool->pool[row].indirizzo, indirizzo);
		strcpy(pool->pool[row].telefono, telefono);
		if ((apertura.month > 12 || apertura.month < 1) || (apertura.day > 31 || apertura.day < 1)) {
			memset(pool->pool[row].apertura, 0, DATE_LEN);
			memset(pool->pool[row].chiusura, 0, DATE_LEN);
		} else {
			mysql_date_to_string(&apertura, pool->pool[row].apertura);
			mysql_date_to_string(&chiusura, pool->pool[row].chiusura);
		}
		strcpy(pool->pool[row].nome_corso, nome_corso);

		row++;
	}
    out:
	mysql_stmt_free_result(view_pool);
	mysql_stmt_reset(view_pool);
	return pool;
}


void pool_dispose(pool_t *pool)
{
	free(pool);
}


report_t *do_view_report(cf_t insegnante, bool type)
{
	int status;
	size_t row = 0;
	MYSQL_BIND param[5];
	MYSQL_TIME giorno;
	MYSQL_TIME ora;
	char nome_piscina[NOME_LEN];
	report_t *report = NULL;

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, insegnante, CF_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_TINY, &type, sizeof(type));
	set_binding_param(&param[2], MYSQL_TYPE_DATE, &giorno, sizeof(giorno));
	set_binding_param(&param[3], MYSQL_TYPE_TIME, &ora, sizeof(ora));
	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, nome_piscina, NOME_LEN);
	
	if(mysql_stmt_bind_param(view_report, param)) {
		print_stmt_error(view_report, "Unable to bind output parameters for get report\n");
		free(report);
		report = NULL;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(view_report) != 0) {
		print_stmt_error(view_report, "Could not execute get report procedure");
		goto out;
	}

	mysql_stmt_store_result(view_report);

	report = malloc(sizeof(*report) + sizeof(struct report_entry) * mysql_stmt_num_rows(view_report));
	if(report == NULL)
		goto out;
	memset(report, 0, sizeof(*report) + sizeof(struct report_entry) * mysql_stmt_num_rows(view_report));
	report->num_entries = mysql_stmt_num_rows(view_report);

	// Get bound parameters
	mysql_stmt_store_result(view_report);
	
	set_binding_param(&param[2], MYSQL_TYPE_DATE, &giorno, sizeof(giorno));
	set_binding_param(&param[3], MYSQL_TYPE_TIME, &ora, sizeof(ora));
	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, nome_piscina, NOME_LEN);

	if(mysql_stmt_bind_result(view_report, param)) {
		print_stmt_error(view_report, "Unable to bind output parameters for get report\n");
		free(report);
		report = NULL;
		goto out;
	}

	while (true) {
		status = mysql_stmt_fetch(view_report);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy(report->report[row].nome_piscina, nome_piscina);
		mysql_date_to_string(&giorno, report->report[row].giorno);
		mysql_time_to_string(&ora, report->report[row].ora);

		row++;
	}
    out:
	mysql_stmt_free_result(view_report);
	mysql_stmt_reset(view_report);
	return report;
}


void report_dispose(report_t *report)
{
	free(report);
}


void do_add_user(user_t user, bool type)
{
	MYSQL_BIND param[5];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, user.utente, CF_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, user.nome, strlen(user.nome));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, user.indirizzo, strlen(user.indirizzo));
	set_binding_param(&param[3], MYSQL_TYPE_TINY, &user.età, sizeof(unsigned char));
	set_binding_param(&param[4], MYSQL_TYPE_TINY, &type, sizeof(bool));

	if(mysql_stmt_bind_param(add_user, param) != 0) {
 		// Note _param
		print_stmt_error(add_user, "Could not bind parameters for Add User");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(add_user) != 0) {
		print_stmt_error(add_user, "Could not execute Add User procedure");
		return;
	}

	mysql_stmt_free_result(add_user);
	mysql_stmt_reset(add_user);
	return;
}


void do_add_contact(contact_t contact)
{
	MYSQL_BIND param[3];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, contact.utente, CF_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, contact.tipo, strlen(contact.tipo));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, contact.recapito, strlen(contact.recapito));

	if(mysql_stmt_bind_param(add_contact, param) != 0) {
 		// Note _param
		print_stmt_error(add_contact, "Could not bind parameters for Add Contact");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(add_contact) != 0) {
		print_stmt_error(add_contact, "Could not execute Add Contact procedure");
		return;
	}

	mysql_stmt_free_result(add_contact);
	mysql_stmt_reset(add_contact);
	return;
}


void do_indici_asta(asta_t asta)
{
	MYSQL_BIND param[11];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, &certificate.iscritto, CF_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, certificate.medico, strlen(certificate.medico));
	set_binding_param(&param[2], MYSQL_TYPE_STRING, &certificate.iscritto, CF_LEN);
	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, certificate.medico, strlen(certificate.medico));
	set_binding_param(&param[4], MYSQL_TYPE_STRING, &certificate.iscritto, CF_LEN);
	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, certificate.medico, strlen(certificate.medico));
	set_binding_param(&param[6], MYSQL_TYPE_STRING, &certificate.iscritto, CF_LEN);
	set_binding_param(&param[7], MYSQL_TYPE_VAR_STRING, certificate.medico, strlen(certificate.medico));
	set_binding_param(&param[8], MYSQL_TYPE_STRING, &certificate.iscritto, CF_LEN);
	set_binding_param(&param[9], MYSQL_TYPE_VAR_STRING, certificate.medico, strlen(certificate.medico));
	set_binding_param(&param[10], MYSQL_TYPE_STRING, &certificate.iscritto, CF_LEN);


	if(mysql_stmt_bind_param(indici_asta, param) != 0) {
 		// Note _param
		print_stmt_error(indici_asta, "Could not bind parameters for Add Certificate");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(indici_asta) != 0) {
		print_stmt_error(indici_asta, "Could not execute Add Certificate procedure");
		return;
	}

	mysql_stmt_free_result(indici_asta);
	mysql_stmt_reset(indici_asta);
	return;
}


void do_add_course(struct course_entry course, char nome_piscina[NOME_LEN])
{
	MYSQL_BIND param[6];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, course.nome_corso, strlen(course.nome_corso));
	set_binding_param(&param[1], MYSQL_TYPE_STRING, course.nome_piscina, strlen(course.nome_piscina));
	set_binding_param(&param[2], MYSQL_TYPE_TINY, &course.min_p, sizeof(char ));
	set_binding_param(&param[3], MYSQL_TYPE_TINY, &course.max_p, sizeof(char));
	set_binding_param(&param[4], MYSQL_TYPE_LONG, &course.costo, sizeof(int));
	set_binding_param(&param[5], MYSQL_TYPE_STRING, &course.insegnante, CF_LEN);

	if(mysql_stmt_bind_param(add_course, param) != 0) {
 		// Note _param
		print_stmt_error(add_course, "Could not bind parameters for Add Course");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(add_course) != 0) {
		print_stmt_error(add_course, "Could not execute Add Course procedure");
		return;
	}

	mysql_stmt_free_result(add_course);
	mysql_stmt_reset(add_course);
	return;
}


void do_add_lesson(lesson_t lesson)
{
	MYSQL_BIND param[4];
	MYSQL_TIME giorno;
	MYSQL_TIME ora;

	date_to_mysql_time(lesson.giorno, &giorno);
	time_to_mysql_time(lesson.ora, &ora);

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_TINY, &giorno, sizeof(giorno));
	set_binding_param(&param[1], MYSQL_TYPE_TINY, &ora, sizeof(ora));
	set_binding_param(&param[2], MYSQL_TYPE_STRING, lesson.nome_corso, strlen(lesson.nome_corso));
	set_binding_param(&param[3], MYSQL_TYPE_STRING, lesson.nome_piscina, strlen(lesson.nome_piscina));

	if(mysql_stmt_bind_param(add_lesson, param) != 0) {
 		// Note _param
		print_stmt_error(add_lesson, "Could not bind parameters for Add Lesson");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(add_lesson) != 0) {
		print_stmt_error(add_lesson, "Could not execute Add Lesson procedure");
		return;
	}

	mysql_stmt_free_result(add_lesson);
	mysql_stmt_reset(add_lesson);
	return;
}


void do_add_subscription(cf_t iscritto, char course_name[NOME_LEN], char nome_piscina[NOME_LEN])
{
	MYSQL_BIND param[2];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, iscritto, CF_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, course_name, strlen(course_name));

	if(mysql_stmt_bind_param(add_subscription, param) != 0) {
 		// Note _param
		print_stmt_error(add_subscription, "Could not bind parameters for Add Certificate");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(add_subscription) != 0) {
		print_stmt_error(add_subscription, "Could not execute Add Certificate procedure");
		return;
	}

	mysql_stmt_free_result(add_subscription);
	mysql_stmt_reset(add_subscription);
	return;
}


void do_remove_course(char course_name[NOME_LEN], char nome_piscina[NOME_LEN])
{
	MYSQL_BIND param[1];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, course_name, strlen(course_name));

	if(mysql_stmt_bind_param(remove_course, param) != 0) {
 		// Note _param
		print_stmt_error(remove_course, "Could not bind parameters for Remove Course");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(remove_course) != 0) {
		print_stmt_error(remove_course, "Could not execute Remove Course procedure");
		return;
	}

	mysql_stmt_free_result(remove_course);
	mysql_stmt_reset(remove_course);
	return;
}


void do_remove_subscription(cf_t user, char course_name[NOME_LEN], char nome_piscina[NOME_LEN])
{
	MYSQL_BIND param[3];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, user, CF_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, course_name, strlen(course_name));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, nome_piscina, NOME_LEN);

	if(mysql_stmt_bind_param(remove_subscription, param) != 0) {
 		// Note _param
		print_stmt_error(remove_subscription, "Could not bind parameters for Remove Subscription");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(remove_subscription) != 0) {
		print_stmt_error(remove_subscription, "Could not execute Remove Subscription procedure");
		return;
	}

	mysql_stmt_free_result(remove_subscription);
	mysql_stmt_reset(remove_subscription);
	return;
}


void do_remove_user(cf_t user)
{
	MYSQL_BIND param[1];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, user, CF_LEN);

	if(mysql_stmt_bind_param(remove_user, param) != 0) {
 		// Note _param
		print_stmt_error(remove_user, "Could not bind parameters for Remove User");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(remove_user) != 0) {
		print_stmt_error(remove_user, "Could not execute Remove User procedure");
		return;
	}

	mysql_stmt_free_result(remove_user);
	mysql_stmt_reset(remove_user);
	return;
}


void do_modify_lesson(lesson_t lesson)
{
	MYSQL_BIND param[4];
	set_binding_param(&param[0], MYSQL_TYPE_DATE, lesson.giorno, DATE_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_DATE, lesson.ora, DATE_LEN);
	set_binding_param(&param[2], MYSQL_TYPE_DATE, lesson.nome_corso, strlen(lesson.nome_corso));
	set_binding_param(&param[3], MYSQL_TYPE_DATE, lesson.nome_piscina, strlen(lesson.nome_piscina));

	if(mysql_stmt_bind_param(modify_lesson, param) != 0) {
 		// Note _param
		print_stmt_error(modify_lesson, "Could not bind parameters for Modify Lesson");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(modify_lesson) != 0) {
		print_stmt_error(modify_lesson, "Could not execute Modify Lesson procedure");
		return;
	}

	mysql_stmt_free_result(modify_lesson);
	mysql_stmt_reset(modify_lesson);
	return;
}


void do_update_last_visit(cf_t free_swimmer, char nome_piscina[NOME_LEN])
{
	MYSQL_BIND param[2];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, free_swimmer, CF_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, nome_piscina, strlen(nome_piscina));

	if(mysql_stmt_bind_param(update_last_visit, param) != 0) {
 		// Note _param
		print_stmt_error(update_last_visit, "Could not bind parameters for Update Last Visit");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(update_last_visit) != 0) {
		print_stmt_error(update_last_visit, "Could not execute Update Last Visit procedure");
		return;
	}

	mysql_stmt_free_result(update_last_visit);
	mysql_stmt_reset(update_last_visit);
	return;
}



void do_end_job(cf_t insegnante)
{
	MYSQL_BIND param[1];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, insegnante, CF_LEN);

	if(mysql_stmt_bind_param(end_job, param) != 0) {
 		// Note _param
		print_stmt_error(end_job, "Could not bind parameters for End Job");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(end_job) != 0) {
		print_stmt_error(end_job, "Could not execute End Job procedure");
		return;
	}

	mysql_stmt_free_result(end_job);
	mysql_stmt_reset(end_job);
	return;
}


void do_add_title(cf_t insegnante, char title[TITLE_LEN])
{
	MYSQL_BIND param[2]; // Used both for input and output

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, insegnante, CF_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, title, strlen(title));

	if(mysql_stmt_bind_param(add_title, param) != 0) {
 		// Note _param
		print_stmt_error(add_title, "Could not bind parameters for End Job");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(add_title) != 0) {
		print_stmt_error(add_title, "Could not execute End Job procedure");
		return;
	}

	mysql_stmt_free_result(add_title);
	mysql_stmt_reset(add_title);
	return;
}


void do_add_new_job(job_t job)
{
	MYSQL_BIND param[3];
	MYSQL_TIME data_inizio;

	date_to_mysql_time(job.data_inizio, &data_inizio);

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, job.insegnante, CF_LEN);
	set_binding_param(&param[1], MYSQL_TYPE_DATE, &data_inizio, sizeof(data_inizio));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, job.nome_piscina, strlen(job.nome_piscina));

	if(mysql_stmt_bind_param(add_new_job, param) != 0) {
 		// Note _param
		print_stmt_error(add_new_job, "Could not bind parameters for Add New Job");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(add_new_job) != 0) {
		print_stmt_error(add_new_job, "Could not execute Add New Job procedure");
		return;
	}

	mysql_stmt_free_result(add_new_job);
	mysql_stmt_reset(add_new_job);
	return;
}


void do_update_manager(char nome_piscina[NOME_LEN], char nuovo_responsabile[NOME_LEN])
{
	MYSQL_BIND param[2]; // Used both for input and output

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, nome_piscina, strlen(nome_piscina));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, nuovo_responsabile, strlen(nuovo_responsabile));

	if(mysql_stmt_bind_param(update_manager, param) != 0) {
 		// Note _param
		print_stmt_error(update_manager, "Could not bind parameters for Update Manager");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(update_manager) != 0) {
		print_stmt_error(update_manager, "Could not execute Update Manager procedure");
		return;
	}

	mysql_stmt_free_result(update_manager);
	mysql_stmt_reset(update_manager);
	return;
}
*/