#pragma once
#include <stdbool.h>
#include <stdlib.h>

#define DATE_LEN 11
#define TIME_LEN 6
#define DATETIME_LEN (DATE_LEN + TIME_LEN)
#define USERNAME_LEN 32
#define PASSWORD_LEN 32
#define NOME_LEN 32
#define ADDRESS_LEN 64
#define CF_LEN 16
#define CODE_LEN 7
#define CREDIT_CARD_NUMBER_LEN 19
#define LEVEL_LEN 32
#define CITY_NAME_LEN 32

typedef char cf_t[CF_LEN];
typedef char code_t[CODE_LEN];
typedef char credit_card_number_t[CREDIT_CARD_NUMBER_LEN];

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

typedef struct credit_card {
	char number[CREDIT_CARD_NUMBER_LEN];
	unsigned short int CVV;
	char expiration_date[DATE_LEN];
}credit_card_t;

typedef struct category {
	char first_level[LEVEL_LEN];
	char second_level[LEVEL_LEN];
	char third_level[LEVEL_LEN];
}category_t;

typedef struct user {
	cf_t cf;
	char name[NOME_LEN];
	char surname[NOME_LEN];
	char address[ADDRESS_LEN];
	char birthday[DATE_LEN];
	char birthcity[CITY_NAME_LEN];
}user_t;

typedef struct object {
	code_t code;
	char state[32];
	unsigned short int lenght;
	unsigned short int width;
	unsigned short int height;
	char description[65535];
	float start_price;
	category_t *category;
}object_t;

struct asta_entry {
	object_t object;
	float max_offer;
};

typedef struct asta {
	unsigned num_entries;
	struct asta_entry aste[];
}asta_t;

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
extern role_t attempt_login(credentials_t *cred);
void do_user_registration(user_t *user, credentials_t *credentials, credit_card_t *credit_card);
extern void db_switch_to_login(void);
extern void db_switch_to_user(void);
extern void db_switch_to_administrator(void);
void do_indici_asta(object_t *object, unsigned short int duration);
void do_inserisci_categoria(category_t *category);
