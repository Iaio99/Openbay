#pragma once
#include "../model/db.h"

enum actions {
	OBJECT_NEW,
	CATEGORY_NEW,
	QUIT,
	END_OF_ACTIONS
};


extern int get_administrator_action(void);
