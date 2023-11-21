#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <traceevent/event-parse.h>
#include <traceevent/kbuffer.h>
#include <traceevent/trace-seq.h>
static char *read_file(const char *file)
{
        char buf[BUFSIZ];
        char *f = NULL;
        int fd, r, size = 0;
        fd = open(file, O_RDONLY);
        do
        {
                r = read(fd, buf, BUFSIZ);
                f = realloc(f, r + size); //
                memcpy(f + size, buf, r);
                size += r;
        }
        while (r > 0);
        return f;
}
static void read_record(struct kbuffer *kbuf, struct tep_record *record)
{
        unsigned long long ts;
        record->data = kbuffer_read_event(kbuf, &ts);
        record->ts = ts;
        record->missed_events = kbuffer_missed_events(kbuf);
        record->size = kbuffer_event_size(kbuf);
        record->record_size = kbuffer_curr_size(kbuf);
        record->cpu = 0;
}
int main(int argc, char **argv)
{
        struct tep_record record;
        struct tep_handle *tep;
        struct kbuffer *kbuf;
        struct trace_seq seq;
        unsigned long long ts;
        void *buf;
        int sub_buf_size;
        int fd;
        fd = open(argv[1], O_RDONLY);
        trace_seq_init(&seq);
        tep = tep_alloc();
        buf = read_file("/sys/kernel/tracing/events/header_page");
        tep_parse_header_page(tep, buf, strlen(buf), 0);
        free(buf);
        buf = read_file("/sys/kernel/tracing/saved_cmdlines");
        tep_parse_saved_cmdlines(tep, buf);
        free(buf);
        buf = read_file("/sys/kernel/tracing/events/sched/sched_switch/format");
        tep_parse_event(tep, buf, strlen(buf), "sched");
        free(buf);
        buf = read_file("/sys/kernel/tracing/events/ftrace/function/format");
        tep_parse_event(tep, buf, strlen(buf), "ftrace");
        free(buf);
        buf = read_file("/proc/kallsyms");
        tep_parse_kallsyms(tep, buf);
        free(buf);
 
        sub_buf_size = tep_get_sub_buffer_size(tep);
        printf("the valu of sub buffer size %d\n",sub_buf_size);
        buf = malloc(sub_buf_size*4);
        read(fd, buf, sub_buf_size);
        kbuf = kbuffer_alloc(tep_get_header_page_size(tep) == 8, !tep_is_bigendian());
        kbuffer_load_subbuffer(kbuf, buf);
        int x=0;
        int r=0;
        for (;;)
        {
                read_record(kbuf, &record);
                if(kbuffer_next_event(kbuf, NULL)== NULL)
                {
                       r = read(fd, buf, sub_buf_size);
                        if(r < sub_buf_size)
                                break;
                        else
                        {
                                kbuffer_load_subbuffer(kbuf, buf);
                                continue;
 
                        }
                }
                trace_seq_reset(&seq);
                tep_print_event(tep, &seq, &record, "%6.1000d %s-%d %s %s\n",TEP_PRINT_TIME, TEP_PRINT_COMM, TEP_PRINT_PID,TEP_PRINT_NAME, TEP_PRINT_INFO);
                trace_seq_do_printf(&seq);
               // printf("%d\n",x++);
        }
 
 
        trace_seq_destroy(&seq);
        return 0;
}
