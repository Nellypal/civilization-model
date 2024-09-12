

#include "engine.h"
#include <sys/time.h>

bool timer_is_up(struct timeval *timer, struct timeval *duration)
{
    struct timeval delta = {};
    struct timeval old_duration = *duration;
    struct timeval now = {};

    gettimeofday(&now, NULL);
    update_duration(&delta, &now, timer);
    if (update_duration(duration, &old_duration, &delta) ||
        (!duration->tv_sec && !duration->tv_usec))
        return true;
    *timer = now;
    return false;
}

bool update_duration(struct timeval *result,
    const struct timeval *present, const struct timeval *past)
{
    struct timeval tmp = {.tv_sec = past->tv_sec, .tv_usec = past->tv_usec};
    int carry = 0;

    if (present->tv_usec < tmp.tv_usec) {
        carry = (tmp.tv_usec - present->tv_usec) / 1000000 + 1;
        tmp.tv_usec -= 1000000 * carry;
        tmp.tv_sec += carry;
    }
    if (present->tv_usec - tmp.tv_usec > 1000000) {
        carry = (present->tv_usec - tmp.tv_usec) / 1000000;
        tmp.tv_usec += 1000000 * carry;
        tmp.tv_sec -= carry;
    }
    result->tv_sec = present->tv_sec - tmp.tv_sec;
    result->tv_usec = present->tv_usec - tmp.tv_usec;
    return present->tv_sec < tmp.tv_sec;
}

void fill_duration(int frequence, int time_limit, struct timeval *res)
{
    double total = ((double)time_limit / (double)frequence) * 1000000;

    res->tv_sec = total / 1000000;
    res->tv_usec = (suseconds_t)total % 1000000;
}
