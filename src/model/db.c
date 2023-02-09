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


role_t attempt_login(credentials_t *cred)
{
	MYSQL_BIND param[3]; // Used both for input and output
	int role = 0;

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cred->username, strlen(cred->username), 0);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, cred->password, strlen(cred->password), 0);
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &role, sizeof(role), 0);

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
	set_binding_param(&param[0], MYSQL_TYPE_LONG, &role, sizeof(role), 0);

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


void do_user_registration(user_t *user, credentials_t *credentials, credit_card_t *credit_card)
{
	MYSQL_BIND param[11];
	MYSQL_TIME expiration_date, birthday;
	date_to_mysql_time(user->birthday, &birthday);
	date_to_mysql_time(credit_card->expiration_date, &expiration_date);

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, credentials->username, strlen(credentials->username), 0);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, credentials->password, strlen(credentials->password), 0);
	set_binding_param(&param[2], MYSQL_TYPE_STRING, user->cf, strlen(user->cf), 0);
	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, user->name, strlen(user->name), 0);
	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, user->surname, strlen(user->surname), 0);
	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, user->address, strlen(user->address), 0);
	set_binding_param(&param[6], MYSQL_TYPE_DATE, &birthday, sizeof(birthday), 0);
	set_binding_param(&param[7], MYSQL_TYPE_VAR_STRING, user->birthcity, strlen(user->birthcity), 0);
	set_binding_param(&param[8], MYSQL_TYPE_STRING, credit_card->number, strlen(user->address), 0);
	set_binding_param(&param[9], MYSQL_TYPE_SHORT, &credit_card->CVV, sizeof(credit_card->CVV), 0);
	set_binding_param(&param[10], MYSQL_TYPE_DATE, &expiration_date, sizeof(expiration_date), 0);

	if(mysql_stmt_bind_param(user_registration, param) != 0) {
 		// Note _param
		print_stmt_error(user_registration, "Could not bind parameters for Add Lesson");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(user_registration) != 0) {
		print_stmt_error(user_registration, "Could not execute Add Lesson procedure");
		return;
	}

	mysql_stmt_free_result(user_registration);
	mysql_stmt_reset(user_registration);
	return;
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


void do_indici_asta(object_t *object, unsigned short int duration)
{
	MYSQL_BIND param[11];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, object->code, strlen(object->code), 0);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, object->state, strlen(object->state), 0);
	set_binding_param(&param[2], MYSQL_TYPE_TINY, &object->lenght, sizeof(object->lenght), 1);
	set_binding_param(&param[3], MYSQL_TYPE_TINY, &object->width, sizeof(object->width), 1);
	set_binding_param(&param[4], MYSQL_TYPE_TINY, &object->height, sizeof(object->height), 1);
	set_binding_param(&param[5], MYSQL_TYPE_BLOB, object->description, strlen(object->description), 0);
	set_binding_param(&param[6], MYSQL_TYPE_FLOAT, &object->start_price, sizeof(object->start_price), 1);
	set_binding_param(&param[7], MYSQL_TYPE_TINY, &duration, sizeof(duration), 1);
	set_binding_param(&param[8], MYSQL_TYPE_VAR_STRING, object->category->first_level, strlen(object->category->first_level), 0);
	set_binding_param(&param[9], MYSQL_TYPE_VAR_STRING, object->category->second_level, strlen(object->category->second_level), 0);
	set_binding_param(&param[10], MYSQL_TYPE_VAR_STRING, object->category->third_level, strlen(object->category->third_level), 0);

	if(mysql_stmt_bind_param(indici_asta, param) != 0) {
 		// Note _param
		print_stmt_error(indici_asta, "Could not bind parameters for Add Lesson");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(indici_asta) != 0) {
		print_stmt_error(indici_asta, "Could not execute Add Lesson procedure");
		return;
	}

	mysql_stmt_free_result(indici_asta);
	mysql_stmt_reset(indici_asta);
	return;
}


void do_inserisci_categoria(category_t *category)
{
	MYSQL_BIND param[3];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, category->first_level, strlen(category->first_level), 0);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, category->second_level, strlen(category->second_level), 0);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, category->third_level, strlen(category->third_level), 0);

	if(mysql_stmt_bind_param(inserisci_categoria, param) != 0) {
 		// Note _param
		print_stmt_error(inserisci_categoria, "Could not bind parameters for Add Lesson");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(inserisci_categoria) != 0) {
		print_stmt_error(inserisci_categoria, "Could not execute Add Lesson procedure");
		return;
	}

	mysql_stmt_free_result(inserisci_categoria);
	mysql_stmt_reset(inserisci_categoria);
	return;
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
*/