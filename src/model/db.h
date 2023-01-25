#pragma once
#include <stdbool.h>
#include <stdlib.h>

#define DATE_LEN 11
#define TIME_LEN 6
#define DATETIME_LEN (DATE_LEN + TIME_LEN)
#define USERNAME_LEN 64
#define PASSWORD_LEN 32
#define NOME_LEN 64
#define RESPONSABILE_LEN 64
#define INDIRIZZO_LEN 64
#define CF_LEN 16

typedef char cf_t[CF_LEN+1];
typedef char code_t[8];

typedef enum {
	LOGIN_ROLE,
	ADMIN,
	USER,
	FAILED_LOGIN
} role_t;

typedef struct credentials {
	char username[USERNAME_LEN];
	char password[PASSWORD_LEN];
}credentials_t;

/*
Return type of query example
struct pool_entry {
	char nome_piscina[NOME_LEN];
	char telefono[TELEFONO_LEN];
	char indirizzo[INDIRIZZO_LEN];
	char apertura[DATE_LEN];
	char chiusura[DATE_LEN];
	char nome_corso[NOME_LEN];
};

typedef struct pool {
	unsigned num_entries;
	struct pool_entry pool[];
}pool_t;
*/

//DB operations
extern bool init_db(void);
extern void fini_db(void);

//Login operations
extern role_t attempt_login(struct credentials *cred);
extern void db_switch_to_login(void);
extern void db_switch_to_user(void);
extern void db_switch_to_administrator(void);