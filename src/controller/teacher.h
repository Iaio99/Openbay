#pragma once

#define WEEKLY_REPORT_TYPE false
#define MONTHLY_REPORT_TYPE true

extern void teacher_controller(void);
static bool report(bool type);
static bool report_weekly(void);
static bool report_monthly(void);
static bool quit(void);