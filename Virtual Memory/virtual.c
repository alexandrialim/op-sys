#include "oslabs.h"

// First-In-First-Out (FIFO)
int process_page_access_fifo(struct PTE page_table[TABLEMAX], int *table_count, int page_num, int frame_pool[POOLMAX], int *frame_count, int current_timestamp) {
    if (page_table[page_num].is_valid) {
        page_table[page_num].last_access_timestamp = current_timestamp;
        page_table[page_num].reference_count++;
        return page_table[page_num].frame_number;
    }

    if (*frame_count > 0) {
        int frame = frame_pool[0];
        for (int i = 0; i < *frame_count - 1; i++) {
            frame_pool[i] = frame_pool[i + 1];
        }
        (*frame_count)--;

        page_table[page_num].is_valid = 1;
        page_table[page_num].frame_number = frame;
        page_table[page_num].arrival_timestamp = current_timestamp;
        page_table[page_num].last_access_timestamp = current_timestamp;
        page_table[page_num].reference_count = 1;
        return frame;
    }

    int victim_page = -1;
    int earliest_arrival = -1;

    for (int i = 0; i < *table_count; i++) {
        if (page_table[i].is_valid) {
            if (victim_page == -1 || page_table[i].arrival_timestamp < earliest_arrival) {
                earliest_arrival = page_table[i].arrival_timestamp;
                victim_page = i;
            }
        }
    }

    int reclaimed_frame = page_table[victim_page].frame_number;

    page_table[victim_page].is_valid = 0;
    page_table[victim_page].frame_number = -1;
    page_table[victim_page].arrival_timestamp = -1;
    page_table[victim_page].last_access_timestamp = -1;
    page_table[victim_page].reference_count = -1;

    page_table[page_num].is_valid = 1;
    page_table[page_num].frame_number = reclaimed_frame;
    page_table[page_num].arrival_timestamp = current_timestamp;
    page_table[page_num].last_access_timestamp = current_timestamp;
    page_table[page_num].reference_count = 1;

    return reclaimed_frame;
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_count, int refrence_string[REFERENCEMAX], int reference_count, int frame_pool[POOLMAX], int frame_count) {
    int faults = 0;
    int timestamp = 1;

    for (int i = 0; i < reference_count; i++) {
        timestamp++; // INC CORRECTLY MOVED TO TOP
        int page_num = refrence_string[i];

        if (page_table[page_num].is_valid) {
            page_table[page_num].last_access_timestamp = timestamp;
            page_table[page_num].reference_count++;
        } else {
            faults++;
            if (frame_count > 0) {
                int frame = frame_pool[0];
                for (int j = 0; j < frame_count - 1; j++) {
                    frame_pool[j] = frame_pool[j + 1];
                }
                frame_count--;

                page_table[page_num].is_valid = 1;
                page_table[page_num].frame_number = frame;
                page_table[page_num].arrival_timestamp = timestamp;
                page_table[page_num].last_access_timestamp = timestamp;
                page_table[page_num].reference_count = 1;
            } else {
                int victim_page = -1;
                int earliest_arrival = -1;

                for (int j = 0; j < table_count; j++) {
                    if (page_table[j].is_valid) {
                        if (victim_page == -1 || page_table[j].arrival_timestamp < earliest_arrival) {
                            earliest_arrival = page_table[j].arrival_timestamp;
                            victim_page = j;
                        }
                    }
                }

                int reclaimed_frame = page_table[victim_page].frame_number;

                page_table[victim_page].is_valid = 0;
                page_table[victim_page].frame_number = -1;
                page_table[victim_page].arrival_timestamp = -1;
                page_table[victim_page].last_access_timestamp = -1;
                page_table[victim_page].reference_count = -1;

                page_table[page_num].is_valid = 1;
                page_table[page_num].frame_number = reclaimed_frame;
                page_table[page_num].arrival_timestamp = timestamp;
                page_table[page_num].last_access_timestamp = timestamp;
                page_table[page_num].reference_count = 1;
            }
        }
    }
    return faults;
}

// Least-Recently-Used (LRU)
int process_page_access_lru(struct PTE page_table[TABLEMAX], int *table_count, int page_num, int frame_pool[POOLMAX], int *frame_count, int current_timestamp) {
    if (page_table[page_num].is_valid) {
        page_table[page_num].last_access_timestamp = current_timestamp;
        page_table[page_num].reference_count++;
        return page_table[page_num].frame_number;
    }

    if (*frame_count > 0) {
        int frame = frame_pool[0];
        for (int i = 0; i < *frame_count - 1; i++) {
            frame_pool[i] = frame_pool[i + 1];
        }
        (*frame_count)--;

        page_table[page_num].is_valid = 1;
        page_table[page_num].frame_number = frame;
        page_table[page_num].arrival_timestamp = current_timestamp;
        page_table[page_num].last_access_timestamp = current_timestamp;
        page_table[page_num].reference_count = 1;
        return frame;
    }

    int victim_page = -1;
    int oldest_access = -1;

    for (int i = 0; i < *table_count; i++) {
        if (page_table[i].is_valid) {
            if (victim_page == -1 || page_table[i].last_access_timestamp < oldest_access) {
                oldest_access = page_table[i].last_access_timestamp;
                victim_page = i;
            }
        }
    }

    int reclaimed_frame = page_table[victim_page].frame_number;

    page_table[victim_page].is_valid = 0;
    page_table[victim_page].frame_number = -1;
    page_table[victim_page].arrival_timestamp = -1;
    page_table[victim_page].last_access_timestamp = -1;
    page_table[victim_page].reference_count = -1;

    page_table[page_num].is_valid = 1;
    page_table[page_num].frame_number = reclaimed_frame;
    page_table[page_num].arrival_timestamp = current_timestamp;
    page_table[page_num].last_access_timestamp = current_timestamp;
    page_table[page_num].reference_count = 1;

    return reclaimed_frame;
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX], int table_count, int refrence_string[REFERENCEMAX], int reference_count, int frame_pool[POOLMAX], int frame_count) {
    int faults = 0;
    int timestamp = 1;

    for (int i = 0; i < reference_count; i++) {
        timestamp++; // INC CORRECTLY MOVED TO TOP
        int page_num = refrence_string[i];

        if (page_table[page_num].is_valid) {
            page_table[page_num].last_access_timestamp = timestamp;
            page_table[page_num].reference_count++;
        } else {
            faults++;
            if (frame_count > 0) {
                int frame = frame_pool[0];
                for (int j = 0; j < frame_count - 1; j++) {
                    frame_pool[j] = frame_pool[j + 1];
                }
                frame_count--;

                page_table[page_num].is_valid = 1;
                page_table[page_num].frame_number = frame;
                page_table[page_num].arrival_timestamp = timestamp;
                page_table[page_num].last_access_timestamp = timestamp;
                page_table[page_num].reference_count = 1;
            } else {
                int victim_page = -1;
                int oldest_access = -1;

                for (int j = 0; j < table_count; j++) {
                    if (page_table[j].is_valid) {
                        if (victim_page == -1 || page_table[j].last_access_timestamp < oldest_access) {
                            oldest_access = page_table[j].last_access_timestamp;
                            victim_page = j;
                        }
                    }
                }

                int reclaimed_frame = page_table[victim_page].frame_number;

                // CORRECTED LRU INVALIDATION VALUES
                page_table[victim_page].is_valid = 0;
                page_table[victim_page].frame_number = -1;
                page_table[victim_page].arrival_timestamp = 0;
                page_table[victim_page].last_access_timestamp = 0;
                page_table[victim_page].reference_count = 0;

                page_table[page_num].is_valid = 1;
                page_table[page_num].frame_number = reclaimed_frame;
                page_table[page_num].arrival_timestamp = timestamp;
                page_table[page_num].last_access_timestamp = timestamp;
                page_table[page_num].reference_count = 1;
            }
        }
    }
    return faults;
}

// Least-Frequently-Used (LFU)
int process_page_access_lfu(struct PTE page_table[TABLEMAX], int *table_count, int page_num, int frame_pool[POOLMAX], int *frame_count, int current_timestamp) {
    if (page_table[page_num].is_valid) {
        page_table[page_num].last_access_timestamp = current_timestamp;
        page_table[page_num].reference_count++;
        return page_table[page_num].frame_number;
    }

    if (*frame_count > 0) {
        int frame = frame_pool[0];
        for (int i = 0; i < *frame_count - 1; i++) {
            frame_pool[i] = frame_pool[i + 1];
        }
        (*frame_count)--;

        page_table[page_num].is_valid = 1;
        page_table[page_num].frame_number = frame;
        page_table[page_num].arrival_timestamp = current_timestamp;
        page_table[page_num].last_access_timestamp = current_timestamp;
        page_table[page_num].reference_count = 1;
        return frame;
    }

    int victim_page = -1;
    int min_ref = -1;
    int earliest_arrival = -1;

    for (int i = 0; i < *table_count; i++) {
        if (page_table[i].is_valid) {
            if (victim_page == -1 || page_table[i].reference_count < min_ref || 
               (page_table[i].reference_count == min_ref && page_table[i].arrival_timestamp < earliest_arrival)) {
                min_ref = page_table[i].reference_count;
                earliest_arrival = page_table[i].arrival_timestamp;
                victim_page = i;
            }
        }
    }

    int reclaimed_frame = page_table[victim_page].frame_number;

    page_table[victim_page].is_valid = 0;
    page_table[victim_page].frame_number = -1;
    page_table[victim_page].arrival_timestamp = -1;
    page_table[victim_page].last_access_timestamp = -1;
    page_table[victim_page].reference_count = -1;

    page_table[page_num].is_valid = 1;
    page_table[page_num].frame_number = reclaimed_frame;
    page_table[page_num].arrival_timestamp = current_timestamp;
    page_table[page_num].last_access_timestamp = current_timestamp;
    page_table[page_num].reference_count = 1;

    return reclaimed_frame;
}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX], int table_count, int refrence_string[REFERENCEMAX], int reference_count, int frame_pool[POOLMAX], int frame_count) {
    int faults = 0;
    int timestamp = 1;

    for (int i = 0; i < reference_count; i++) {
        timestamp++; // INC CORRECTLY MOVED TO TOP
        int page_num = refrence_string[i];

        if (page_table[page_num].is_valid) {
            page_table[page_num].last_access_timestamp = timestamp;
            page_table[page_num].reference_count++;
        } else {
            faults++;
            if (frame_count > 0) {
                int frame = frame_pool[0];
                for (int j = 0; j < frame_count - 1; j++) {
                    frame_pool[j] = frame_pool[j + 1];
                }
                frame_count--;

                page_table[page_num].is_valid = 1;
                page_table[page_num].frame_number = frame;
                page_table[page_num].arrival_timestamp = timestamp;
                page_table[page_num].last_access_timestamp = timestamp;
                page_table[page_num].reference_count = 1;
            } else {
                int victim_page = -1;
                int min_ref = -1;
                int earliest_arrival = -1;

                for (int j = 0; j < table_count; j++) {
                    if (page_table[j].is_valid) {
                        if (victim_page == -1 || page_table[j].reference_count < min_ref || 
                           (page_table[j].reference_count == min_ref && page_table[j].arrival_timestamp < earliest_arrival)) {
                            min_ref = page_table[j].reference_count;
                            earliest_arrival = page_table[j].arrival_timestamp;
                            victim_page = j;
                        }
                    }
                }

                int reclaimed_frame = page_table[victim_page].frame_number;

                // CORRECTED LFU INVALIDATION VALUES
                page_table[victim_page].is_valid = 0;
                page_table[victim_page].frame_number = -1;
                page_table[victim_page].arrival_timestamp = 0;
                page_table[victim_page].last_access_timestamp = 0;
                page_table[victim_page].reference_count = 0;

                page_table[page_num].is_valid = 1;
                page_table[page_num].frame_number = reclaimed_frame;
                page_table[page_num].arrival_timestamp = timestamp;
                page_table[page_num].last_access_timestamp = timestamp;
                page_table[page_num].reference_count = 1;
            }
        }
    }
    return faults;
}