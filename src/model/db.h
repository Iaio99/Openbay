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
#define TELEFONO_LEN 11
#define CELLULARE_LEN 11
#define CF_LEN 16
#define TIPO_LEN 11
#define RECAPITO_LEN 64
#define TITLE_LEN 128

typedef char cf_t[CF_LEN+1];

typedef enum {
	LOGIN_ROLE,
	ADDETTO_COMUNALE,
	PISCINA,
	INSEGNANTE,
	USER,
	FAILED_LOGIN
} role_t;

typedef struct credentials {
	char username[USERNAME_LEN];
	char password[PASSWORD_LEN];
}credentials_t;

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

struct course_entry {
	char nome_corso[NOME_LEN];
	char nome_piscina[NOME_LEN];
	char min_p;
	char max_p;
	int costo;
	cf_t insegnante;
};

typedef struct course {
	unsigned num_entries;
	struct course_entry course[];
}course_t;

struct report_entry {
	char giorno[DATE_LEN];
	char ora[TIME_LEN];
	char nome_piscina[NOME_LEN];
};

typedef struct report {
	unsigned num_entries;
	struct report_entry report[];
}report_t;

typedef struct user {
	cf_t utente;
	char nome[NOME_LEN];
	char indirizzo[INDIRIZZO_LEN];
	unsigned char età;
}user_t;

typedef struct contact {
	cf_t utente;
	char *tipo;
	char recapito[RECAPITO_LEN];
}contact_t;

typedef	struct certificate {
	cf_t iscritto;
	char medico[NOME_LEN];
}certificate_t;

typedef struct teacher {
	cf_t insegnante;
	char nome_insegnante[NOME_LEN];
	char indirizzo[INDIRIZZO_LEN];
	char cellulare[CELLULARE_LEN];
	char *qualifiche[];
}teacher_t;

typedef struct job {
	cf_t insegnante;
	char data_inizio[DATE_LEN];
	char nome_piscina[NOME_LEN];
}job_t;

typedef struct lesson {
	char giorno[DATE_LEN];
	char ora[TIME_LEN];
	char nome_corso[NOME_LEN];
	char nome_piscina[NOME_LEN];
}lesson_t;


//DB operations
extern bool init_db(void);
extern void fini_db(void);

//Login operations
extern role_t attempt_login(struct credentials *cred);
extern void db_switch_to_login(void);
extern void db_switch_to_teacher(void);
extern void db_switch_to_user(void);
extern void db_switch_to_pool(void);
extern void db_switch_to_administrator(void);

//Anonymous user operations
extern pool_t *do_view_pool();
extern void pool_dispose(pool_t *pool);
extern course_t *do_view_course(char [NOME_LEN]);
extern void course_dispose(course_t *course);

//Teachers' operations
extern report_t *do_view_report(cf_t teacher, bool type);
extern void report_dispose(report_t *report);

//pools' workers operations
extern void do_add_certificate(certificate_t certificate);
extern void do_add_contact(contact_t contact);
extern void do_add_course(struct course_entry course, char nome_piscina[NOME_LEN]);
extern void do_add_lesson(lesson_t lesson);
extern void do_add_subscription(cf_t subscriber, char course_name[NOME_LEN], char nome_piscina[NOME_LEN]);
extern void do_add_user(user_t user, bool type);
extern void do_remove_course(char course_name[NOME_LEN], char nome_piscina[NOME_LEN]);
extern void do_remove_subscription(cf_t user, char course_name[NOME_LEN], char nome_piscina[NOME_LEN]);
extern void do_remove_user(cf_t user);
extern void do_modify_lesson(lesson_t lesson);
extern void do_update_last_visit(cf_t free_swimmer, char nome_piscina[NOME_LEN]);

//Addetti comunali (administrators) operations
extern void do_end_job(cf_t insegnante);
extern void do_add_title(cf_t insegnante, char title[TITLE_LEN]);
extern void do_add_new_job(job_t job);
extern void do_update_manager(char nome_piscina[NOME_LEN], char nuovo_responsabile[NOME_LEN]);
/*
extern void do_open_open_pool(char nome_piscina[NOME_LEN]);
extern void do_close_open_pool(char nome_piscina[NOME_LEN]);
*/