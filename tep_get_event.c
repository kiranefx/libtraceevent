#include <event-parse.h>

int main() 
{
    // Allocate and initialize a trace event handle
    struct tep_handle *tep = tep_alloc();

    // Get the total number of events
    int i, count = tep_get_events_count(tep);

    // Get a pointer to the first event
    struct tep_event *event, *events = tep_get_first_event(tep);

    if (events == NULL) {
        /* There are no events */
    } else {
        // Iterate through the events
        for (i = 0; i < count; i++) {
            event = (events + i);
            /* Process events[i] */
            /* You can access information about the event using 'event' pointer */
        }

        // Get the last event
        event = tep_get_event(tep, count - 1);
        /* You can access information about the last event using 'event' pointer */
    }

    // Don't forget to free the allocated resources
    tep_free(tep);

    return 0;
}
