#pragma once
#include "../model/db.h"

extern bool ask_for_relogin(void);
extern int get_login_action();
extern void view_login(struct credentials *cred);
extern void view_registration(credentials_t *cred, user_t *user, credit_card_t *credit_card);