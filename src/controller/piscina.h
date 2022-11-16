#pragma once

#include "../model/db.h"

#define SIGNED_TYPE false
#define FREE_SWIMMER_TYPE true

extern void pool_controller(char username_pool[USERNAME_LEN]);
static bool add_contact(cf_t user);
static bool add_certificate(void);
static bool add_course(void);
static void add_lesson(char nome_corso[NOME_LEN], char nome_piscina[NOME_LEN]);
static bool add_subscription(void);
static bool add_user(void);
static bool remove_course(void);
static bool remove_user(void);
static bool remove_subscription(void);
static bool update_last_visit(void);
static bool modify_lesson(void);
static bool quit(void);
