#include <event-parse.h>

int main() 
{
    // Allocate and initialize a trace event handle
    struct tep_handle *tep = tep_alloc();

    // Set the number of CPUs to 5
    tep_set_cpus(tep, 5);

    // Set the TEP_NSEC_OUTPUT flag
    tep_set_flag(tep, TEP_NSEC_OUTPUT);

    // Test the TEP_NSEC_OUTPUT flag
    if (tep_test_flag(tep, TEP_NSEC_OUTPUT)) {
        printf("The TEP_NSEC_OUTPUT flag is set.\n");
    } else {
        printf("The TEP_NSEC_OUTPUT flag is not set.\n");
    }

    // Clear the TEP_NSEC_OUTPUT flag
    tep_clear_flag(tep, TEP_NSEC_OUTPUT);

    // Test the TEP_NSEC_OUTPUT flag after clearing
    if (tep_test_flag(tep, TEP_NSEC_OUTPUT)) {
        printf("The TEP_NSEC_OUTPUT flag is set after clearing (unexpected).\n");
    } else {
        printf("The TEP_NSEC_OUTPUT flag is not set after clearing.\n");
    }

    // Get the number of CPUs with a tracing buffer and print the information
    printf("We have tracing data for %d CPUs", tep_get_cpus(tep));

    // Don't forget to free the allocated resources when done
    tep_free(tep);

    return 0;
}

