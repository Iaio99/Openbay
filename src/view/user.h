#pragma once
#include "../model/db.h"

enum actions {
	OFFER,
	USER_ASTE,
	ASTE_IN_CORSO,
	ASTE_FINITE,
	QUIT,
	END_OF_ACTIONS
};

extern int get_user_action(void);

void print_aste(asta_t *aste, char *message);
void print_my_aste(asta_t *aste);