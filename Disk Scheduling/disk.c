#include "oslabs.h"

struct RCB NULLRCB = {0, 0, 0, 0, 0};

// Helper function to calculate absolute value for distances
int calculate_distance(int a, int b) {
    int diff = a - b;
    return diff < 0 ? -diff : diff;
}

// First-Come-First-Served (FCFS)
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_count, struct RCB current_request, struct RCB new_request, int timestamp) {
    // If the disk is currently free, service the new request immediately
    if (current_request.request_id == 0) {
        return new_request;
    } else {
        // Disk is busy, add the newly arriving request to the queue
        request_queue[*queue_count] = new_request;
        (*queue_count)++;
        return current_request;
    }
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_count) {
    // Return NULLRCB if there are no pending requests
    if (*queue_count == 0) {
        return NULLRCB;
    }

    int earliest_idx = 0;

    // Find the request with the earliest arrival timestamp
    for (int i = 1; i < *queue_count; i++) {
        if (request_queue[i].arrival_timestamp < request_queue[earliest_idx].arrival_timestamp) {
            earliest_idx = i;
        }
    }

    struct RCB next_request = request_queue[earliest_idx];

    // Remove the chosen request from the queue
    for (int i = earliest_idx; i < *queue_count - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_count)--;

    return next_request;
}

// Shortest-Seek-Time-First (SSTF)
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_count, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.request_id == 0) {
        return new_request;
    } else {
        request_queue[*queue_count] = new_request;
        (*queue_count)++;
        return current_request;
    }
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_count, int current_cylinder) {
    if (*queue_count == 0) {
        return NULLRCB;
    }

    int best_idx = 0;
    int min_dist = calculate_distance(request_queue[0].cylinder, current_cylinder);

    // Find the request closest to the current cylinder
    for (int i = 1; i < *queue_count; i++) {
        int dist = calculate_distance(request_queue[i].cylinder, current_cylinder);
        
        if (dist < min_dist) {
            min_dist = dist;
            best_idx = i;
        } else if (dist == min_dist) {
            if (request_queue[i].arrival_timestamp < request_queue[best_idx].arrival_timestamp) {
                best_idx = i;
            }
        }
    }

    struct RCB next_request = request_queue[best_idx];

    for (int i = best_idx; i < *queue_count - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_count)--;

    return next_request;
}

// LOOK Scheduling (LOOK)
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_count, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.request_id == 0) {
        return new_request;
    } else {
        request_queue[*queue_count] = new_request;
        (*queue_count)++;
        return current_request;
    }
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_count, int current_cylinder, int scan_direction) {
    if (*queue_count == 0) {
        return NULLRCB;
    }

    int best_idx = -1;

    // Check for requests at the exact same cylinder first
    for (int i = 0; i < *queue_count; i++) {
        if (request_queue[i].cylinder == current_cylinder) {
            if (best_idx == -1 || request_queue[i].arrival_timestamp < request_queue[best_idx].arrival_timestamp) {
                best_idx = i;
            }
        }
    }
    
    if (best_idx != -1) {
        struct RCB next_request = request_queue[best_idx];
        for (int i = best_idx; i < *queue_count - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_count)--;
        return next_request;
    }

    // Scan Direction is UP (1)
    if (scan_direction == 1) {
        int min_dist = -1;
        
        for (int i = 0; i < *queue_count; i++) {
            if (request_queue[i].cylinder > current_cylinder) {
                int dist = request_queue[i].cylinder - current_cylinder;
                if (best_idx == -1 || dist < min_dist || (dist == min_dist && request_queue[i].arrival_timestamp < request_queue[best_idx].arrival_timestamp)) {
                    min_dist = dist;
                    best_idx = i;
                }
            }
        }
        
        // If no larger cylinder found, reverse and find closest smaller cylinder
        if (best_idx == -1) {
            for (int i = 0; i < *queue_count; i++) {
                int dist = calculate_distance(request_queue[i].cylinder, current_cylinder);
                if (best_idx == -1 || dist < min_dist || (dist == min_dist && request_queue[i].arrival_timestamp < request_queue[best_idx].arrival_timestamp)) {
                    min_dist = dist;
                    best_idx = i;
                }
            }
        }
    }
    // Scan Direction is DOWN (0)
    else if (scan_direction == 0) {
        int min_dist = -1;
        
        for (int i = 0; i < *queue_count; i++) {
            if (request_queue[i].cylinder < current_cylinder) {
                int dist = current_cylinder - request_queue[i].cylinder;
                if (best_idx == -1 || dist < min_dist || (dist == min_dist && request_queue[i].arrival_timestamp < request_queue[best_idx].arrival_timestamp)) {
                    min_dist = dist;
                    best_idx = i;
                }
            }
        }
        
        // If no smaller cylinder found, reverse and find closest larger cylinder
        if (best_idx == -1) {
            for (int i = 0; i < *queue_count; i++) {
                int dist = calculate_distance(request_queue[i].cylinder, current_cylinder);
                if (best_idx == -1 || dist < min_dist || (dist == min_dist && request_queue[i].arrival_timestamp < request_queue[best_idx].arrival_timestamp)) {
                    min_dist = dist;
                    best_idx = i;
                }
            }
        }
    }

    struct RCB next_request = request_queue[best_idx];

    for (int i = best_idx; i < *queue_count - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_count)--;

    return next_request;
}