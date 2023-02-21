#pragma once
#include "../model/db.h"

enum actions {
	OFFER,
	USER_AUCTIONS,
	AUCTIONS_IN_PROGRESS,
	CLOSED_AUCTIONS,
	QUIT,
	END_OF_ACTIONS
};

extern int get_user_action(void);

void print_auctions(auction_t *auctions, char *message);
void print_user_auctions(auction_t *auctions);