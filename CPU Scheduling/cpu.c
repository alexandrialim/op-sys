#include "oslabs.h"

struct PCB NULLPCB = {0, 0, 0, 0, 0, 0, 0};

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_count, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    } else {
        if (new_process.process_priority >= current_process.process_priority) {
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            
            ready_queue[*queue_count] = new_process;
            (*queue_count)++;
            return current_process;
        } else {
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            
            current_process.remaining_bursttime -= (timestamp - current_process.execution_starttime);
            current_process.execution_endtime = 0; 
            
            ready_queue[*queue_count] = current_process;
            (*queue_count)++;
            return new_process;
        }
    }
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_count, int timestamp) {
    if (*queue_count == 0) {
        return NULLPCB;
    }
    
    int highest_priority_idx = 0;
    for (int i = 1; i < *queue_count; i++) {
        if (ready_queue[i].process_priority < ready_queue[highest_priority_idx].process_priority) {
            highest_priority_idx = i;
        }
    }
    
    struct PCB next_process = ready_queue[highest_priority_idx];
    
    for (int i = highest_priority_idx; i < (*queue_count) - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_count)--;
    
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
    
    return next_process;
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_count, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    } else {
        int current_rbt = current_process.remaining_bursttime - (timestamp - current_process.execution_starttime);
        
        if (new_process.total_bursttime >= current_rbt) {
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            
            ready_queue[*queue_count] = new_process;
            (*queue_count)++;
            return current_process;
        } else {
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            
            current_process.remaining_bursttime = current_rbt;
            current_process.execution_starttime = 0; 
            current_process.execution_endtime = 0;
            
            ready_queue[*queue_count] = current_process;
            (*queue_count)++;
            return new_process;
        }
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_count, int timestamp) {
    if (*queue_count == 0) {
        return NULLPCB;
    }
    
    int shortest_rbt_idx = 0;
    for (int i = 1; i < *queue_count; i++) {
        if (ready_queue[i].remaining_bursttime < ready_queue[shortest_rbt_idx].remaining_bursttime) {
            shortest_rbt_idx = i;
        }
    }
    
    struct PCB next_process = ready_queue[shortest_rbt_idx];
    
    for (int i = shortest_rbt_idx; i < (*queue_count) - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_count)--;
    
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
    
    return next_process;
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_count, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + MIN(time_quantum, new_process.total_bursttime);
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    } else {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        
        ready_queue[*queue_count] = new_process;
        (*queue_count)++;
        return current_process;
    }
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_count, int timestamp, int time_quantum) {
    if (*queue_count == 0) {
        return NULLPCB;
    }
    
    int earliest_at_idx = 0;
    for (int i = 1; i < *queue_count; i++) {
        if (ready_queue[i].arrival_timestamp < ready_queue[earliest_at_idx].arrival_timestamp) {
            earliest_at_idx = i;
        }
    }
    
    struct PCB next_process = ready_queue[earliest_at_idx];
    
    for (int i = earliest_at_idx; i < (*queue_count) - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_count)--;
    
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + MIN(time_quantum, next_process.remaining_bursttime);
    
    return next_process;
}