

#pragma once

#include <sys/time.h>

void fill_duration(int frequence, int time_limit, struct timeval *res);
void update_duration(struct timeval *timer, struct timeval *duration);
bool timer_is_up(struct timeval *timer);
