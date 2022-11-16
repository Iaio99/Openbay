#pragma once
#include "../model/db.h"

enum actions {
	ADD_USER,
	ADD_CERTIFICATE,
	ADD_COURSE,
	ADD_SUBSCRIPTION,
	REMOVE_COURSE,
	REMOVE_USER,
	REMOVE_SUBSCRIPTION,
	UPDATE_LAST_VISIT,
	MODIFY_LESSON,
	QUIT,
	END_OF_ACTIONS
};

extern int get_pool_action(void);