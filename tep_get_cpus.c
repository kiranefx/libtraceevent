#include <event-parse.h>

int main() 
{
    // Allocate and initialize a trace event handle
    struct tep_handle *tep = tep_alloc();

    // Set the number of CPUs to 5
    tep_set_cpus(tep, 5);

    // Get the number of CPUs with a tracing buffer and print the information
    printf("We have tracing data for %d CPUs", tep_get_cpus(tep));

    // Don't forget to free the allocated resources when done
    tep_free(tep);

    return 0;
}

