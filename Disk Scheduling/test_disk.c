#include <stdio.h>
#include "oslabs.h"

void print_rcb(struct RCB r) {
    if (r.request_id == 0) printf("NULLRCB\n");
    else printf("[RID:%d, AT:%d, CYL:%d, ADDR:%d, PID:%d]\n", r.request_id, r.arrival_timestamp, r.cylinder, r.address, r.process_id);
}

void print_queue(struct RCB q[], int cnt) {
    if (cnt == 0) printf("  EMPTY\n");
    else for (int i = 0; i < cnt; i++) { printf("  "); print_rcb(q[i]); }
}

void print_test_header(const char* name) {
    printf("\n====================================================\n%s\n====================================================\n", name);
}

int main() {
    // ---------------------------------------------------------
    // FCFS Tests
    // ---------------------------------------------------------
    print_test_header("FCFS Arrival Case 1");
    struct RCB q_fcfs[QUEUEMAX]; int cnt_fcfs = 0;
    struct RCB curr = {51, 1, 53, 53, 51};
    struct RCB next = {52, 2, 54, 54, 52};
    struct RCB res = handle_request_arrival_fcfs(q_fcfs, &cnt_fcfs, curr, next, 2);
    printf("Output Queue:\n"); print_queue(q_fcfs, cnt_fcfs);
    printf("Output RCB: "); print_rcb(res);

    print_test_header("FCFS Completion Case 2");
    struct RCB q_comp[QUEUEMAX] = {{4, 2, 53, 53, 4}, {5, 3, 54, 54, 5}, {6, 5, 51, 59, 6}};
    int cnt_comp = 3;
    struct RCB res_comp = handle_request_completion_fcfs(q_comp, &cnt_comp);
    printf("Output Queue:\n"); print_queue(q_comp, cnt_comp);
    printf("Output RCB: "); print_rcb(res_comp);

    // ---------------------------------------------------------
    // SSTF Tests
    // ---------------------------------------------------------
    print_test_header("SSTF Completion Case 1");
    struct RCB q_sstf[QUEUEMAX] = {{1, 72, 45, 45, 1}, {2, 71, 47, 47, 2}, {3, 73, 43, 43, 3}};
    int cnt_sstf = 3;
    struct RCB res_sstf = handle_request_completion_sstf(q_sstf, &cnt_sstf, 48);
    printf("Output Queue:\n"); print_queue(q_sstf, cnt_sstf);
    printf("Output RCB: "); print_rcb(res_sstf);

    // ---------------------------------------------------------
    // LOOK Tests
    // ---------------------------------------------------------
    print_test_header("LOOK Completion Case 2 (Scan 0, smaller requests present)");
    struct RCB q_look[QUEUEMAX] = {
        {51, 1, 90, 53, 51}, {52, 3, 15, 54, 52}, {53, 4, 45, 55, 53}, {54, 7, 28, 56, 54}
    };
    int cnt_look = 4;
    struct RCB res_look = handle_request_completion_look(q_look, &cnt_look, 27, 0);
    printf("Output Queue:\n"); print_queue(q_look, cnt_look);
    printf("Output RCB: "); print_rcb(res_look);

    return 0;
}