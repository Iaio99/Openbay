#pragma once
#include "../model/db.h"

enum actions {
	NEW_OBJECT,
	NEW_CATEGORY,
	QUIT,
	END_OF_ACTIONS
};


extern int get_administrator_action(void);
