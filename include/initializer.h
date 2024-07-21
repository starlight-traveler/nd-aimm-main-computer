#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "thread_safe_gui_status_logger.h"
#include "aimm_test_manager.h"
#include "aimm_test_detail.h"

void primary_initialization();
void status_logger_initializer();
void test_manager_adder();
void test_manager_initialization();
void secondary_initialization();
void tertiary_initialization();

#endif // INITIALIZER_H
