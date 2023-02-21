#pragma once
#include "../model/db.h"

extern void user_controller(cf_t cf);
static bool make_offer(void);
static void set_counteroffer(code_t object_code);
static bool user_auctions(void);
static bool auctions_in_progress(void);
static bool closed_auctions(void);
static bool quit(void);