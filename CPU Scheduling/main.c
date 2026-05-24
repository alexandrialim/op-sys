#include <stdio.h>
#include "oslabs.h"

void print_pcb(struct PCB p) {
    if (p.process_id == 0 && p.total_bursttime == 0) {
        printf("[PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0]\n");
    } else {
        printf("[PID:%d, AT:%d, TBT:%d, EST:%d, EET:%d, RBT:%d, Priority:%d]\n",
               p.process_id, p.arrival_timestamp, p.total_bursttime,
               p.execution_starttime, p.execution_endtime, p.remaining_bursttime, p.process_priority);
    }
}

void print_queue(struct PCB queue[], int cnt) {
    if (cnt == 0) {
        printf("EMPTY\n");
    } else {
        for (int i = 0; i < cnt; i++) {
            print_pcb(queue[i]);
        }
    }
}

void print_test_header(const char* test_name) {
    printf("\n====================================================\n");
    printf("%s\n", test_name);
    printf("====================================================\n");
}

int main() {
    // Run tests for Priority-based Preemptive (PP) Arrivals
    print_test_header("handle_process_arrival_pp - Case 1: published test case");
    struct PCB q_pp_a1[QUEUEMAX]; int cnt_pp_a1 = 0;
    struct PCB c_pp_a1 = {1, 1, 4, 1, 5, 4, 8};
    struct PCB n_pp_a1 = {2, 2, 3, 0, 0, 3, 6};
    struct PCB r_pp_a1 = handle_process_arrival_pp(q_pp_a1, &cnt_pp_a1, c_pp_a1, n_pp_a1, 2);
    printf("Output PCB: "); print_pcb(r_pp_a1);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_pp_a1); print_queue(q_pp_a1, cnt_pp_a1);

    print_test_header("handle_process_arrival_pp - Case 2: no currently running process");
    struct PCB q_pp_a2[QUEUEMAX]; int cnt_pp_a2 = 0;
    struct PCB c_pp_a2 = {0, 0, 0, 0, 0, 0, 0};
    struct PCB n_pp_a2 = {5, 2, 5, 0, 0, 5, 4};
    struct PCB r_pp_a2 = handle_process_arrival_pp(q_pp_a2, &cnt_pp_a2, c_pp_a2, n_pp_a2, 2);
    printf("Output PCB: "); print_pcb(r_pp_a2);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_pp_a2); print_queue(q_pp_a2, cnt_pp_a2);

    print_test_header("handle_process_arrival_pp - Case 3: new process has lower priority");
    struct PCB q_pp_a3[QUEUEMAX] = {{6, 3, 4, 0, 0, 4, 5}}; int cnt_pp_a3 = 1;
    struct PCB c_pp_a3 = {5, 2, 5, 2, 7, 3, 4};
    struct PCB n_pp_a3 = {7, 4, 5, 0, 0, 5, 7};
    struct PCB r_pp_a3 = handle_process_arrival_pp(q_pp_a3, &cnt_pp_a3, c_pp_a3, n_pp_a3, 4);
    printf("Output PCB: "); print_pcb(r_pp_a3);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_pp_a3); print_queue(q_pp_a3, cnt_pp_a3);

    // Run tests for Priority-based Preemptive (PP) Completions
    print_test_header("handle_process_completion_pp - Case 1: published test case");
    struct PCB q_pp_c1[QUEUEMAX] = {
        {1, 1, 4, 0, 0, 4, 23}, {2, 1, 4, 0, 0, 4, 22}, {3, 1, 4, 0, 0, 4, 24}
    }; int cnt_pp_c1 = 3;
    struct PCB r_pp_c1 = handle_process_completion_pp(q_pp_c1, &cnt_pp_c1, 2);
    printf("Output PCB: "); print_pcb(r_pp_c1);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_pp_c1); print_queue(q_pp_c1, cnt_pp_c1);

    print_test_header("handle_process_completion_pp - Case 2: ready queue is empty");
    struct PCB q_pp_c2[QUEUEMAX]; int cnt_pp_c2 = 0;
    struct PCB r_pp_c2 = handle_process_completion_pp(q_pp_c2, &cnt_pp_c2, 3);
    printf("Output PCB: "); print_pcb(r_pp_c2);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_pp_c2); print_queue(q_pp_c2, cnt_pp_c2);

    print_test_header("handle_process_completion_pp - Case 3: highest priority process");
    struct PCB q_pp_c3[QUEUEMAX] = {
        {6, 3, 4, 0, 0, 4, 5}, {7, 4, 5, 0, 0, 5, 7}, 
        {8, 5, 6, 0, 0, 6, 3}, {9, 7, 5, 0, 0, 5, 9}
    }; int cnt_pp_c3 = 4;
    struct PCB r_pp_c3 = handle_process_completion_pp(q_pp_c3, &cnt_pp_c3, 8);
    printf("Output PCB: "); print_pcb(r_pp_c3);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_pp_c3); print_queue(q_pp_c3, cnt_pp_c3);

    // Run tests for Shortest Remaining Time Next (SRTP) Arrivals
    print_test_header("handle_process_arrival_srtp - Case 1: published test case");
    struct PCB q_srtp_a1[QUEUEMAX]; int cnt_srtp_a1 = 0;
    struct PCB c_srtp_a1 = {1, 1, 8, 1, 9, 8, 0};
    struct PCB n_srtp_a1 = {2, 2, 6, 0, 0, 6, 0};
    struct PCB r_srtp_a1 = handle_process_arrival_srtp(q_srtp_a1, &cnt_srtp_a1, c_srtp_a1, n_srtp_a1, 2);
    printf("Output PCB: "); print_pcb(r_srtp_a1);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_srtp_a1); print_queue(q_srtp_a1, cnt_srtp_a1);

    print_test_header("handle_process_arrival_srtp - Case 2: no currently running process");
    struct PCB q_srtp_a2[QUEUEMAX]; int cnt_srtp_a2 = 0;
    struct PCB c_srtp_a2 = {0, 0, 0, 0, 0, 0, 0};
    struct PCB n_srtp_a2 = {5, 2, 5, 0, 0, 5, 4};
    struct PCB r_srtp_a2 = handle_process_arrival_srtp(q_srtp_a2, &cnt_srtp_a2, c_srtp_a2, n_srtp_a2, 2);
    printf("Output PCB: "); print_pcb(r_srtp_a2);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_srtp_a2); print_queue(q_srtp_a2, cnt_srtp_a2);

    print_test_header("handle_process_arrival_srtp - Case 3: longer remaining burst time");
    struct PCB q_srtp_a3[QUEUEMAX]; int cnt_srtp_a3 = 0;
    struct PCB c_srtp_a3 = {5, 2, 5, 2, 7, 3, 4};
    struct PCB n_srtp_a3 = {6, 4, 7, 0, 0, 7, 2};
    struct PCB r_srtp_a3 = handle_process_arrival_srtp(q_srtp_a3, &cnt_srtp_a3, c_srtp_a3, n_srtp_a3, 4);
    printf("Output PCB: "); print_pcb(r_srtp_a3);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_srtp_a3); print_queue(q_srtp_a3, cnt_srtp_a3);

    // Run tests for Shortest Remaining Time Next (SRTP) Completions
    print_test_header("handle_process_completion_srtp - Case 1: published test case");
    struct PCB q_srtp_c1[QUEUEMAX] = {
        {1, 1, 23, 0, 0, 23, 0}, {2, 1, 22, 0, 0, 22, 0}, {3, 1, 24, 0, 0, 24, 0}
    }; int cnt_srtp_c1 = 3;
    struct PCB r_srtp_c1 = handle_process_completion_srtp(q_srtp_c1, &cnt_srtp_c1, 2);
    printf("Output PCB: "); print_pcb(r_srtp_c1);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_srtp_c1); print_queue(q_srtp_c1, cnt_srtp_c1);

    print_test_header("handle_process_completion_srtp - Case 2: ready queue is empty");
    struct PCB q_srtp_c2[QUEUEMAX]; int cnt_srtp_c2 = 0;
    struct PCB r_srtp_c2 = handle_process_completion_srtp(q_srtp_c2, &cnt_srtp_c2, 3);
    printf("Output PCB: "); print_pcb(r_srtp_c2);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_srtp_c2); print_queue(q_srtp_c2, cnt_srtp_c2);

    print_test_header("handle_process_completion_srtp - Case 3: smallest remaining burst time");
    struct PCB q_srtp_c3[QUEUEMAX] = {
        {6, 3, 4, 0, 0, 4, 5}, {7, 4, 5, 0, 0, 5, 7}, 
        {8, 5, 6, 0, 0, 6, 3}, {9, 7, 5, 0, 0, 5, 9}
    }; int cnt_srtp_c3 = 4;
    struct PCB r_srtp_c3 = handle_process_completion_srtp(q_srtp_c3, &cnt_srtp_c3, 8);
    printf("Output PCB: "); print_pcb(r_srtp_c3);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_srtp_c3); print_queue(q_srtp_c3, cnt_srtp_c3);

    // Run tests for Round Robin (RR) Arrivals
    print_test_header("handle_process_arrival_rr - Case 1: published test case");
    struct PCB q_rr_a1[QUEUEMAX]; int cnt_rr_a1 = 0;
    struct PCB c_rr_a1 = {1, 1, 8, 1, 9, 8, 0};
    struct PCB n_rr_a1 = {2, 2, 8, 0, 0, 8, 0};
    struct PCB r_rr_a1 = handle_process_arrival_rr(q_rr_a1, &cnt_rr_a1, c_rr_a1, n_rr_a1, 2, 6);
    printf("Output PCB: "); print_pcb(r_rr_a1);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_rr_a1); print_queue(q_rr_a1, cnt_rr_a1);

    print_test_header("handle_process_arrival_rr - Case 2: no currently running process");
    struct PCB q_rr_a2[QUEUEMAX]; int cnt_rr_a2 = 0;
    struct PCB c_rr_a2 = {0, 0, 0, 0, 0, 0, 0};
    struct PCB n_rr_a2 = {5, 2, 5, 0, 0, 5, 4};
    struct PCB r_rr_a2 = handle_process_arrival_rr(q_rr_a2, &cnt_rr_a2, c_rr_a2, n_rr_a2, 2, 6);
    printf("Output PCB: "); print_pcb(r_rr_a2);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_rr_a2); print_queue(q_rr_a2, cnt_rr_a2);

    print_test_header("handle_process_arrival_rr - Case 3: currently running process");
    struct PCB q_rr_a3[QUEUEMAX]; int cnt_rr_a3 = 0;
    struct PCB c_rr_a3 = {5, 2, 5, 2, 7, 3, 4};
    struct PCB n_rr_a3 = {6, 4, 7, 0, 0, 7, 2};
    struct PCB r_rr_a3 = handle_process_arrival_rr(q_rr_a3, &cnt_rr_a3, c_rr_a3, n_rr_a3, 4, 6);
    printf("Output PCB: "); print_pcb(r_rr_a3);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_rr_a3); print_queue(q_rr_a3, cnt_rr_a3);

    // Run tests for Round Robin (RR) Completions
    print_test_header("handle_process_completion_rr - Case 1: published test case");
    struct PCB q_rr_c1[QUEUEMAX] = {
        {1, 22, 8, 0, 0, 8, 0}, {2, 21, 8, 0, 0, 8, 0}, {3, 23, 8, 0, 0, 8, 0}
    }; int cnt_rr_c1 = 3;
    struct PCB r_rr_c1 = handle_process_completion_rr(q_rr_c1, &cnt_rr_c1, 24, 10);
    printf("Output PCB: "); print_pcb(r_rr_c1);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_rr_c1); print_queue(q_rr_c1, cnt_rr_c1);

    print_test_header("handle_process_completion_rr - Case 2: ready queue is empty");
    struct PCB q_rr_c2[QUEUEMAX]; int cnt_rr_c2 = 0;
    struct PCB r_rr_c2 = handle_process_completion_rr(q_rr_c2, &cnt_rr_c2, 2, 6);
    printf("Output PCB: "); print_pcb(r_rr_c2);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_rr_c2); print_queue(q_rr_c2, cnt_rr_c2);

    print_test_header("handle_process_completion_rr - Case 3: different values from sample case");
    struct PCB q_rr_c3[QUEUEMAX] = {
        {6, 3, 4, 0, 0, 4, 5}, {7, 4, 5, 0, 0, 5, 7}, 
        {8, 5, 6, 0, 0, 6, 3}, {9, 7, 5, 0, 0, 5, 9}
    }; int cnt_rr_c3 = 4;
    struct PCB r_rr_c3 = handle_process_completion_rr(q_rr_c3, &cnt_rr_c3, 12, 6);
    printf("Output PCB: "); print_pcb(r_rr_c3);
    printf("Output Queue Cnt: %d\nOutput Queue:\n", cnt_rr_c3); print_queue(q_rr_c3, cnt_rr_c3);

    return 0;
}