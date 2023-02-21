#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <assert.h>

#include "db.h"
#include "../utils/db.h"


static MYSQL *conn;

static MYSQL_STMT *login_procedure;
static MYSQL_STMT *make_offer_procedure;
static MYSQL_STMT *set_counteroffer_procedure;
static MYSQL_STMT *call_auction_procedure;
static MYSQL_STMT *insert_category_procedure;
static MYSQL_STMT *user_auctions_informations_procedure;
static MYSQL_STMT *user_registration_procedure;
static MYSQL_STMT *closed_auctions_informations;
static MYSQL_STMT *auctions_in_progress_informations_procedure;


static void close_prepared_stmts(void)
{
	if(login_procedure) {
		mysql_stmt_close(login_procedure);
		login_procedure = NULL;
	}
	if(make_offer_procedure) {
		mysql_stmt_close(make_offer_procedure);
		make_offer_procedure = NULL;
	}
	if(set_counteroffer_procedure) {
		mysql_stmt_close(set_counteroffer_procedure);
		set_counteroffer_procedure = NULL;
	}
	if(call_auction_procedure) {
		mysql_stmt_close(call_auction_procedure);
		call_auction_procedure = NULL;
	}
	if(insert_category_procedure) {
		mysql_stmt_close(insert_category_procedure);
		insert_category_procedure = NULL;
	}
	if(user_auctions_informations_procedure) {
		mysql_stmt_close(user_auctions_informations_procedure);
		user_auctions_informations_procedure = NULL;
	}
	if(user_registration_procedure) {
		mysql_stmt_close(user_registration_procedure);
		user_registration_procedure = NULL;
	}
	if(closed_auctions_informations) {
		mysql_stmt_close(closed_auctions_informations);
		closed_auctions_informations = NULL;
	}
	if(auctions_in_progress_informations_procedure) {
		mysql_stmt_close(auctions_in_progress_informations_procedure);
		auctions_in_progress_informations_procedure = NULL;
	}
}


static bool initialize_prepared_stmts(role_t for_role)
{
	switch(for_role) {

		case LOGIN_ROLE:
			if(!setup_prepared_stmt(&login_procedure, "call login(?, ?, ?, ?)", conn)) {
				print_stmt_error(login_procedure, "Unable to initialize login statement");
				return false;
			}
			if(!setup_prepared_stmt(&user_registration_procedure, "call user_registration(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(user_registration_procedure, "Unable to initialize user_registration statement");
				return false;
			}
			break;
		
		case ADMIN:
			if(!setup_prepared_stmt(&call_auction_procedure, "call call_auction(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
				print_stmt_error(call_auction_procedure, "Unable to initialize call_auction_procedure statement");
				return false;
			}
			if(!setup_prepared_stmt(&insert_category_procedure, "call insert_category(?, ?, ?)", conn)) {
				print_stmt_error(insert_category_procedure, "Unable to initialize insert_category_procedure statement");
				return false;
			}
			break;

		case USER:
			if(!setup_prepared_stmt(&make_offer_procedure, "call make_offer(?, ?, ?)", conn)) {
				print_stmt_error(make_offer_procedure, "Unable to initialize register make_offer_procedure statement");
				return false;
			}
			if(!setup_prepared_stmt(&set_counteroffer_procedure, "call set_counteroffer(?, ?, ?)", conn)) {
				print_stmt_error(set_counteroffer_procedure, "Unable to initialize set_counteroffer_procedure statement");
				return false;
			}
			if(!setup_prepared_stmt(&user_auctions_informations_procedure, "call user_auctions_informations(?)", conn)) {
				print_stmt_error(user_auctions_informations_procedure, "Unable to initialize register user_auctions_informations_procedure statement");
				return false;
			}
			if(!setup_prepared_stmt(&closed_auctions_informations, "call closed_auctions_informations()", conn)) {
				print_stmt_error(closed_auctions_informations, "Unable to initialize register isualizza_aste_passate statement");
				return false;
			}
			if(!setup_prepared_stmt(&auctions_in_progress_informations_procedure, "call auctions_in_progress_informations()", conn)) {
				print_stmt_error(auctions_in_progress_informations_procedure, "Unable to initialize auctions_in_progress_informations_procedure statement");
				return false;
			}		
			break;

		default:
			fprintf(stderr, "[FATAL] Unexpected role to prepare statements.");
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
		finish_with_error(conn, "mysql_init() failed (probably out of memory)");
	}

	if(mysql_real_connect(conn, getenv("HOST"), getenv("LOGIN_USER"), getenv("LOGIN_PASS"), getenv("DB"),
			      atoi(getenv("PORT")), NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed");
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


struct login_data attempt_login(credentials_t *cred)
{
	MYSQL_BIND param[4]; // Used both for input and output
	struct login_data data;
	data.role = 0;
	memset(data.cf, 0, CF_LEN);

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cred->username, strlen(cred->username), 0);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, cred->password, strlen(cred->password), 0);
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &data.role, sizeof(data.role), 0);
	set_binding_param(&param[3], MYSQL_TYPE_STRING, data.cf, CF_LEN, 0);


	if(mysql_stmt_bind_param(login_procedure, param) != 0) {
 		// Note _param
		print_stmt_error(login_procedure, "Could not bind parameters for login");
		data.role = FAILED_LOGIN;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(login_procedure) != 0) {
		print_stmt_error(login_procedure, "Could not execute login procedure");
		data.role = FAILED_LOGIN;
		goto out;
	}

	// Prepare output parameters
	set_binding_param(&param[0], MYSQL_TYPE_LONG, &data.role, sizeof(data.role), 0);
	set_binding_param(&param[1], MYSQL_TYPE_STRING, data.cf, CF_LEN, 0);

	if(mysql_stmt_bind_result(login_procedure, param)) {
		print_stmt_error(login_procedure, "Could not retrieve output parameter");
		data.role = FAILED_LOGIN;
		goto out;
	}

	// Retrieve output parameter
	if(mysql_stmt_fetch(login_procedure)) {
		print_stmt_error(login_procedure, "Could not buffer results");
		data.role = FAILED_LOGIN;
		goto out;
	}


    out:
	// Consume the possibly-returned table for the output parameter
	while(mysql_stmt_next_result(login_procedure) != -1) {}

	mysql_stmt_free_result(login_procedure);
	mysql_stmt_reset(login_procedure);
	return data;
}


void do_user_registration(user_t user, credentials_t credentials, credit_card_t credit_card)
{
	MYSQL_BIND param[11];
	MYSQL_TIME expiration_date, birthday;
	date_to_mysql_time(user.birthday, &birthday);
	date_to_mysql_time(credit_card.expiration_date, &expiration_date);

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, credentials.username, strlen(credentials.username), 0);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, credentials.password, strlen(credentials.password), 0);
	set_binding_param(&param[2], MYSQL_TYPE_STRING, user.cf, strlen(user.cf), 0);
	set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, user.name, strlen(user.name), 0);
	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, user.surname, strlen(user.surname), 0);
	set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, user.address, strlen(user.address), 0);
	set_binding_param(&param[6], MYSQL_TYPE_DATE, &birthday, sizeof(birthday), 0);
	set_binding_param(&param[7], MYSQL_TYPE_VAR_STRING, user.birthcity, strlen(user.birthcity), 0);
	set_binding_param(&param[8], MYSQL_TYPE_STRING, credit_card.number, strlen(credit_card.number), 0);
	set_binding_param(&param[9], MYSQL_TYPE_SHORT, &credit_card.cvv, sizeof(credit_card.cvv), 1);
	set_binding_param(&param[10], MYSQL_TYPE_DATE, &expiration_date, sizeof(expiration_date), 0);

	if(mysql_stmt_bind_param(user_registration_procedure, param) != 0) {
 		// Note _param
		print_stmt_error(user_registration_procedure, "Could not bind parameters for user_registration");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(user_registration_procedure) != 0) {
		print_stmt_error(user_registration_procedure, "Could not execute user_registration procedure");
		return;
	}

	mysql_stmt_free_result(user_registration_procedure);
	mysql_stmt_reset(user_registration_procedure);
	return;
}


void db_switch_to_login(void)
{
	close_prepared_stmts();
	if(mysql_change_user(conn, getenv("LOGIN_USER"), getenv("LOGIN_PASS"), getenv("DB"))) {
		fprintf(stderr, "mysql_change_user() failed: %s", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
	if(!initialize_prepared_stmts(LOGIN_ROLE)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.");
		exit(EXIT_FAILURE);
	}
}


void db_switch_to_administrator(void)
{
	close_prepared_stmts();
	if(mysql_change_user(conn, getenv("ADMIN_USER"), getenv("ADMIN_PASS"), getenv("DB"))) {
		fprintf(stderr, "mysql_change_user() failed: %s", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
	if(!initialize_prepared_stmts(ADMIN)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.");
		exit(EXIT_FAILURE);
	}
}


void db_switch_to_user(void)
{
	close_prepared_stmts();
	if(mysql_change_user(conn, getenv("USER_USER"), getenv("USER_PASS"), getenv("DB"))) {
		fprintf(stderr, "mysql_change_user() failed: %s", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
	if(!initialize_prepared_stmts(USER)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.");
		exit(EXIT_FAILURE);
	}
}


void do_call_auction(object_t object, unsigned char duration)
{
	MYSQL_BIND param[10];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, object.state, strlen(object.state), 0);
	set_binding_param(&param[1], MYSQL_TYPE_TINY, &object.lenght, sizeof(object.lenght), 1);
	set_binding_param(&param[2], MYSQL_TYPE_TINY, &object.width, sizeof(object.width), 1);
	set_binding_param(&param[3], MYSQL_TYPE_TINY, &object.height, sizeof(object.height), 1);
	set_binding_param(&param[4], MYSQL_TYPE_BLOB, object.description, strlen(object.description), 0);
	set_binding_param(&param[5], MYSQL_TYPE_FLOAT, &object.start_price, sizeof(object.start_price), 1);
	set_binding_param(&param[6], MYSQL_TYPE_TINY, &duration, sizeof(duration), 1);
	set_binding_param(&param[7], MYSQL_TYPE_VAR_STRING, object.category.first_level, strlen(object.category.first_level), 0);
	set_binding_param(&param[8], MYSQL_TYPE_VAR_STRING, object.category.second_level, strlen(object.category.second_level), 0);
	set_binding_param(&param[9], MYSQL_TYPE_VAR_STRING, object.category.third_level, strlen(object.category.third_level), 0);

	if(mysql_stmt_bind_param(call_auction_procedure, param) != 0) {
 		// Note _param
		print_stmt_error(call_auction_procedure, "Could not bind parameters for call_auction");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(call_auction_procedure) != 0) {
		print_stmt_error(call_auction_procedure, "Could not execute call_auction procedure");
		return;
	}

	mysql_stmt_free_result(call_auction_procedure);
	mysql_stmt_reset(call_auction_procedure);
	return;
}


void do_insert_category(category_t category)
{
	MYSQL_BIND param[3];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, category.first_level, strlen(category.first_level), 0);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, category.second_level, strlen(category.second_level), 0);
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, category.third_level, strlen(category.third_level), 0);

	if(mysql_stmt_bind_param(insert_category_procedure, param) != 0) {
 		// Note _param
		print_stmt_error(insert_category_procedure, "Could not bind parameters for insert_category");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(insert_category_procedure) != 0) {
		print_stmt_error(insert_category_procedure, "Could not execute insert_category procedure");
		return;
	}

	mysql_stmt_free_result(insert_category_procedure);
	mysql_stmt_reset(insert_category_procedure);
	return;
}


extern void do_set_counteroffer(cf_t user, float import, code_t object)
{
	MYSQL_BIND param[3];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, user, strlen(user), 0);
	set_binding_param(&param[1], MYSQL_TYPE_FLOAT, &import, sizeof(import), 1);
	set_binding_param(&param[2], MYSQL_TYPE_STRING, object, strlen(object), 0);

	if(mysql_stmt_bind_param(set_counteroffer_procedure, param) != 0) {
 		// Note _param
		print_stmt_error(set_counteroffer_procedure, "Could not bind parameters for set_counteroffer");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(set_counteroffer_procedure) != 0) {
		print_stmt_error(set_counteroffer_procedure, "Could not execute set_counteroffer procedure");
		return;
	}

	mysql_stmt_free_result(set_counteroffer_procedure);
	mysql_stmt_reset(set_counteroffer_procedure);
	return;
}


extern bool do_make_offer(cf_t user, float import, code_t object)
{
	MYSQL_BIND param[3];

	// Prepareparam parameters
	set_binding_param(&param[0], MYSQL_TYPE_STRING, user, strlen(user), 0);
	set_binding_param(&param[1], MYSQL_TYPE_FLOAT, &import, sizeof(import), 1);
	set_binding_param(&param[2], MYSQL_TYPE_STRING, object, strlen(object), 0);

	if(mysql_stmt_bind_param(make_offer_procedure, param) != 0) {
 		// Note _param
		print_stmt_error(make_offer_procedure, "Could not bind parameters for make_offer");
		return false;
	}

	// Run procedure
	if(mysql_stmt_execute(make_offer_procedure) != 0) {
		print_stmt_error(make_offer_procedure, "Could not execute make_offer procedure");
		return false;
	}

	mysql_stmt_free_result(make_offer_procedure);
	mysql_stmt_reset(make_offer_procedure);
	return true;
}


auction_t *view_user_auctions_informations(cf_t user)
{
	int status;
	size_t row = 0;
	MYSQL_BIND param[6];

	code_t code;
	char description[DESCRIPTION_LEN];
	unsigned short int number_offers;
	float max_offer;
	MYSQL_TIME end_auction;

	auction_t *auctions = NULL;

	set_binding_param(&param[0], MYSQL_TYPE_STRING, user, strlen(user), 0);
//	set_binding_param(&param[1], MYSQL_TYPE_STRING, code, CODE_LEN, 0);
//	set_binding_param(&param[2], MYSQL_TYPE_BLOB, description, DESCRIPTION_LEN, 0);
//	set_binding_param(&param[3], MYSQL_TYPE_TIMESTAMP, &end_auction, sizeof(end_auction), 0);
//	set_binding_param(&param[4], MYSQL_TYPE_SHORT, &number_offers, sizeof(number_offers), 0);
//	set_binding_param(&param[5], MYSQL_TYPE_FLOAT, &max_offer, sizeof(max_offer), 0);

	if(mysql_stmt_bind_param(user_auctions_informations_procedure, param)) {
		print_stmt_error(user_auctions_informations_procedure, "Unable to bind output parameters for user_auctions_informations");
		free(auctions);
		auctions = NULL;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(user_auctions_informations_procedure) != 0) {
		print_stmt_error(user_auctions_informations_procedure, "Could not execute user_auctions_informations procedure");
		goto out;
	}

	mysql_stmt_store_result(user_auctions_informations_procedure);

	auctions = malloc(sizeof(*auctions) + sizeof(struct auction_entry) * mysql_stmt_num_rows(user_auctions_informations_procedure));
	if(auctions == NULL)
		goto out;
	memset(auctions, 0, sizeof(*auctions) + sizeof(struct auction_entry) * mysql_stmt_num_rows(user_auctions_informations_procedure));
	auctions->num_entries = mysql_stmt_num_rows(user_auctions_informations_procedure);

	// Get bound parameters
	mysql_stmt_store_result(user_auctions_informations_procedure);
	
	set_binding_param(&param[0], MYSQL_TYPE_STRING, code, CODE_LEN, 0);
	set_binding_param(&param[1], MYSQL_TYPE_BLOB, description, DESCRIPTION_LEN, 0);
	set_binding_param(&param[2], MYSQL_TYPE_DATETIME, &end_auction, sizeof(end_auction), 0);
	set_binding_param(&param[3], MYSQL_TYPE_SHORT, &number_offers, sizeof(number_offers), 0);
	set_binding_param(&param[4], MYSQL_TYPE_FLOAT, &max_offer, sizeof(max_offer), 0);

	if(mysql_stmt_bind_result(user_auctions_informations_procedure, param)) {
		print_stmt_error(user_auctions_informations_procedure, "Unable to bind output parameters for view_user_auctions_informations");
		free(auctions);
		auctions = NULL;
		goto out;
	}

	while (true) {
		status = mysql_stmt_fetch(user_auctions_informations_procedure);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy(auctions->auctions[row].object.code, code);
		strcpy(auctions->auctions[row].object.description, description);
		mysql_timestamp_to_string(&end_auction, auctions->auctions[row].end);
		auctions->auctions[row].number_offers = number_offers;
		auctions->auctions[row].max_offer = max_offer;

		row++;
	}
    out:
	mysql_stmt_free_result(user_auctions_informations_procedure);
	mysql_stmt_reset(user_auctions_informations_procedure);
	return auctions;
}


auction_t *view_closed_auctions_informations()
{
	int status;
	size_t row = 0;
	MYSQL_BIND param[13];

	code_t code;
	char state[STATE_LEN];
	unsigned short int lenght;
	unsigned short int width;
	unsigned short int height;
	char description[DESCRIPTION_LEN];
	float start_price;
	char first_level[LEVEL_LEN];
	char second_level[LEVEL_LEN];
	char third_level[LEVEL_LEN];
	unsigned short int number_offers;
	float max_offer;
	MYSQL_TIME end_auction;

	auction_t *auctions = NULL;

	if(mysql_stmt_bind_param(closed_auctions_informations, param)) {
		print_stmt_error(closed_auctions_informations, "Unable to bind output parameters for closed_auctions_informations");
		free(auctions);
		auctions = NULL;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(closed_auctions_informations) != 0) {
		print_stmt_error(closed_auctions_informations, "Could not execute closed_auctions_informations procedure");
		goto out;
	}

	mysql_stmt_store_result(closed_auctions_informations);

	auctions = malloc(sizeof(*auctions) + sizeof(struct auction_entry) * mysql_stmt_num_rows(closed_auctions_informations));
	if(auctions == NULL)
		goto out;
	memset(auctions, 0, sizeof(*auctions) + sizeof(struct auction_entry) * mysql_stmt_num_rows(closed_auctions_informations));
	auctions->num_entries = mysql_stmt_num_rows(closed_auctions_informations);

	// Get bound parameters
	mysql_stmt_store_result(closed_auctions_informations);

	set_binding_param(&param[0], MYSQL_TYPE_STRING, code, CODE_LEN, 0);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, state, STATE_LEN, 0);
	set_binding_param(&param[2], MYSQL_TYPE_TINY, &lenght, sizeof(lenght), 1);
	set_binding_param(&param[3], MYSQL_TYPE_TINY, &width, sizeof(width), 1);
	set_binding_param(&param[4], MYSQL_TYPE_TINY, &height, sizeof(height), 1);
	set_binding_param(&param[5], MYSQL_TYPE_BLOB, description, DESCRIPTION_LEN, 0);
	set_binding_param(&param[6], MYSQL_TYPE_FLOAT, &start_price, sizeof(start_price), 1);
	set_binding_param(&param[7], MYSQL_TYPE_DATE, &end_auction, sizeof(end_auction), 0);
	set_binding_param(&param[8], MYSQL_TYPE_VAR_STRING, first_level, LEVEL_LEN, 0);
	set_binding_param(&param[9], MYSQL_TYPE_VAR_STRING, second_level, LEVEL_LEN, 0);
	set_binding_param(&param[10], MYSQL_TYPE_VAR_STRING, third_level, LEVEL_LEN, 0);
	set_binding_param(&param[11], MYSQL_TYPE_FLOAT, &max_offer, sizeof(max_offer), 1);
	set_binding_param(&param[12], MYSQL_TYPE_SHORT, &number_offers, sizeof(number_offers), 1);

	if(mysql_stmt_bind_result(closed_auctions_informations, param)) {
		print_stmt_error(closed_auctions_informations, "Unable to bind output parameters for closed_auctions_informations");
		free(auctions);
		auctions = NULL;
		goto out;
	}

	while (true) {
		status = mysql_stmt_fetch(closed_auctions_informations);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy(auctions->auctions[row].object.code, code);
		strcpy(auctions->auctions[row].object.state, state);
		auctions->auctions[row].object.lenght = lenght;
		auctions->auctions[row].object.width = width;
		auctions->auctions[row].object.height = height;
		strcpy(auctions->auctions[row].object.description, description);
		auctions->auctions[row].object.start_price = start_price;
		mysql_timestamp_to_string(&end_auction, auctions->auctions[row].end);
		strcpy(auctions->auctions[row].object.category.first_level, first_level);
		strcpy(auctions->auctions[row].object.category.second_level, second_level);
		strcpy(auctions->auctions[row].object.category.third_level, third_level);
		auctions->auctions[row].number_offers = number_offers;
		auctions->auctions[row].max_offer = max_offer;

		row++;
	}
    out:
	mysql_stmt_free_result(closed_auctions_informations);
	mysql_stmt_reset(closed_auctions_informations);
	return auctions;
}


auction_t *view_auctions_in_progress_informations()
{
	int status;
	size_t row = 0;
	MYSQL_BIND param[13];

	code_t code;
	char state[STATE_LEN];
	unsigned short int lenght;
	unsigned short int width;
	unsigned short int height;
	char description[DESCRIPTION_LEN];
	float start_price;
	MYSQL_TIME end_auction;
	char first_level[LEVEL_LEN];
	char second_level[LEVEL_LEN];
	char third_level[LEVEL_LEN];
	unsigned short int number_offers;
	float max_offer;

	auction_t *auctions = NULL;

	if(mysql_stmt_bind_param(auctions_in_progress_informations_procedure, param)) {
		print_stmt_error(auctions_in_progress_informations_procedure, "Unable to bind output parameters for auctions_in_progress_informations");
		free(auctions);
		auctions = NULL;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(auctions_in_progress_informations_procedure) != 0) {
		print_stmt_error(auctions_in_progress_informations_procedure, "Could not execute auctions_in_progress_informationss procedure");
		goto out;
	}

	mysql_stmt_store_result(auctions_in_progress_informations_procedure);

	auctions = malloc(sizeof(*auctions) + sizeof(struct auction_entry) * mysql_stmt_num_rows(auctions_in_progress_informations_procedure));
	if(auctions == NULL)
		goto out;
	memset(auctions, 0, sizeof(*auctions) + sizeof(struct auction_entry) * mysql_stmt_num_rows(auctions_in_progress_informations_procedure));
	auctions->num_entries = mysql_stmt_num_rows(auctions_in_progress_informations_procedure);

	// Get bound parameters
	mysql_stmt_store_result(auctions_in_progress_informations_procedure);
	
	set_binding_param(&param[0], MYSQL_TYPE_STRING, code, CODE_LEN, 0);
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, state, STATE_LEN, 0);
	set_binding_param(&param[2], MYSQL_TYPE_TINY, &lenght, sizeof(lenght), 1);
	set_binding_param(&param[3], MYSQL_TYPE_TINY, &width, sizeof(width), 1);
	set_binding_param(&param[4], MYSQL_TYPE_TINY, &height, sizeof(height), 1);
	set_binding_param(&param[5], MYSQL_TYPE_BLOB, description, DESCRIPTION_LEN, 0);
	set_binding_param(&param[6], MYSQL_TYPE_FLOAT, &start_price, sizeof(start_price), 1);
	set_binding_param(&param[7], MYSQL_TYPE_DATE, &end_auction, sizeof(end_auction), 0);
	set_binding_param(&param[8], MYSQL_TYPE_VAR_STRING, first_level, LEVEL_LEN, 0);
	set_binding_param(&param[9], MYSQL_TYPE_VAR_STRING, second_level, LEVEL_LEN, 0);
	set_binding_param(&param[10], MYSQL_TYPE_VAR_STRING, third_level, LEVEL_LEN, 0);
	set_binding_param(&param[11], MYSQL_TYPE_FLOAT, &max_offer, sizeof(max_offer), 1);
	set_binding_param(&param[12], MYSQL_TYPE_SHORT, &number_offers, sizeof(number_offers), 1);

	if(mysql_stmt_bind_result(auctions_in_progress_informations_procedure, param)) {
		print_stmt_error(auctions_in_progress_informations_procedure, "Unable to bind output parameters for auctions_in_progress_informations");
		free(auctions);
		auctions = NULL;
		goto out;
	}

	while (true) {
		status = mysql_stmt_fetch(auctions_in_progress_informations_procedure);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy(auctions->auctions[row].object.code, code);
		strcpy(auctions->auctions[row].object.state, state);
		auctions->auctions[row].object.lenght = lenght;
		auctions->auctions[row].object.width = width;
		auctions->auctions[row].object.height = height;
		strcpy(auctions->auctions[row].object.description, description);
		auctions->auctions[row].object.start_price = start_price;
		mysql_timestamp_to_string(&end_auction, auctions->auctions[row].end);
		strcpy(auctions->auctions[row].object.category.first_level, first_level);
		strcpy(auctions->auctions[row].object.category.second_level, second_level);
		strcpy(auctions->auctions[row].object.category.third_level, third_level);
		auctions->auctions[row].max_offer = max_offer;
		auctions->auctions[row].number_offers = number_offers;

		row++;
	}
    out:
	mysql_stmt_free_result(auctions_in_progress_informations_procedure);
	mysql_stmt_reset(auctions_in_progress_informations_procedure);
	return auctions;
}


void auctions_dispose(auction_t *auctions)
{
	free(auctions);
}