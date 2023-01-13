#pragma once
#include "../model/db.h"

enum actions {
//	JOB_END,
//	JOB_NEW,
//	TITLE_NEW,
//	UPDATE_MANAGER,
	QUIT,
	END_OF_ACTIONS
};

extern int get_administrator_action(void);
