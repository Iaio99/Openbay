#pragma once
#include <stdbool.h>
#include <stdlib.h>

#define DATE_LEN 11
#define TIME_LEN 6
#define DATETIME_LEN (DATE_LEN + TIME_LEN)
#define USERNAME_LEN 64
#define PASSWORD_LEN 64
#define NOME_LEN 32
#define ADDRESS_LEN 64
#define CF_LEN 17
#define CODE_LEN 8
#define CREDIT_CARD_NUMBER_LEN 20
#define LEVEL_LEN 32
#define CITY_NAME_LEN 32
#define DESCRIPTION_LEN 65535
#define STATE_LEN 32

typedef char cf_t[CF_LEN];
typedef char code_t[CODE_LEN];
typedef char credit_card_number_t[CREDIT_CARD_NUMBER_LEN];

typedef enum {
	LOGIN_ROLE,
	ADMIN,
	USER,
	FAILED_LOGIN
} role_t;

struct login_data {
	int role;
	cf_t cf;
};

typedef struct credentials {
	char username[USERNAME_LEN];
	char password[PASSWORD_LEN];
}credentials_t;

typedef struct credit_card {
	char number[CREDIT_CARD_NUMBER_LEN];
	unsigned short int cvv;
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
	char state[STATE_LEN];
	unsigned short int lenght;
	unsigned short int width;
	unsigned short int height;
	char description[DESCRIPTION_LEN];
	float start_price;
	category_t category;
}object_t;

struct asta_entry {
	object_t object;
	float max_offer;
	unsigned short int number_offers;
	char end[DATETIME_LEN];
};

typedef struct asta {
	unsigned num_entries;
	struct asta_entry aste[];
}asta_t;


//DB operations
extern bool init_db(void);
extern void fini_db(void);

//Login operations
extern struct login_data attempt_login(credentials_t *cred);
extern void do_user_registration(user_t user, credentials_t credentials, credit_card_t credit_card);
extern void db_switch_to_login(void);
extern void db_switch_to_user(void);
extern void db_switch_to_administrator(void);
extern void do_indici_asta(object_t object, unsigned char duration);
extern void do_inserisci_categoria(category_t category);
extern void do_fai_offerta(cf_t user, float import, code_t object);
extern void do_imposta_controfferta(cf_t user, float import, code_t object);
extern asta_t *do_stato_aste_utente(cf_t user);
extern asta_t *do_visualizza_aste_passate();
extern asta_t *do_visualizza_oggetti_asta();
extern void aste_dispose();