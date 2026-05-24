#include "oslabs.h"

struct MEMORY_BLOCK NULLBLOCK = {0, 0, 0, 0};

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_count, int process_id) {
    int target_idx = -1;

    // Scan for the first free block that fits the requested size
    for (int i = 0; i < *map_count; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            target_idx = i;
            break;
        }
    }

    // Return NULLBLOCK if no suitable free block is found
    if (target_idx == -1) {
        return NULLBLOCK;
    }

    struct MEMORY_BLOCK allocated_block = memory_map[target_idx];

    if (allocated_block.segment_size == request_size) {
        // update process ID directly in place
        memory_map[target_idx].process_id = process_id;
        return memory_map[target_idx];
    } else {
        // Larger block: split into an allocated piece and a free piece
        struct MEMORY_BLOCK free_remainder;
        free_remainder.start_address = allocated_block.start_address + request_size;
        free_remainder.end_address = allocated_block.end_address;
        free_remainder.segment_size = allocated_block.segment_size - request_size;
        free_remainder.process_id = 0;

        // Shift array elements right to make room for the new free block
        for (int i = *map_count; i > target_idx + 1; i--) {
            memory_map[i] = memory_map[i - 1];
        }
        (*map_count)++;

        // Assign the allocated piece and insert the remainder block
        memory_map[target_idx].end_address = allocated_block.start_address + request_size - 1;
        memory_map[target_idx].segment_size = request_size;
        memory_map[target_idx].process_id = process_id;
        memory_map[target_idx + 1] = free_remainder;

        return memory_map[target_idx];
    }
}

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_count, int process_id) {
    int target_idx = -1;
    int min_waste = -1;

    // Scan for the free block whose size is closest to the request size
    for (int i = 0; i < *map_count; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            int waste = memory_map[i].segment_size - request_size;
            if (min_waste == -1 || waste < min_waste) {
                min_waste = waste;
                target_idx = i;
            }
        }
    }

    if (target_idx == -1) {
        return NULLBLOCK;
    }

    struct MEMORY_BLOCK allocated_block = memory_map[target_idx];

    if (allocated_block.segment_size == request_size) {
        memory_map[target_idx].process_id = process_id;
        return memory_map[target_idx];
    } else {
        struct MEMORY_BLOCK free_remainder;
        free_remainder.start_address = allocated_block.start_address + request_size;
        free_remainder.end_address = allocated_block.end_address;
        free_remainder.segment_size = allocated_block.segment_size - request_size;
        free_remainder.process_id = 0;

        for (int i = *map_count; i > target_idx + 1; i--) {
            memory_map[i] = memory_map[i - 1];
        }
        (*map_count)++;

        memory_map[target_idx].end_address = allocated_block.start_address + request_size - 1;
        memory_map[target_idx].segment_size = request_size;
        memory_map[target_idx].process_id = process_id;
        memory_map[target_idx + 1] = free_remainder;

        return memory_map[target_idx];
    }
}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_count, int process_id) {
    int target_idx = -1;
    int max_size = -1;

    // Scan for the largest candidate free memory block
    for (int i = 0; i < *map_count; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            if (memory_map[i].segment_size > max_size) {
                max_size = memory_map[i].segment_size;
                target_idx = i;
            }
        }
    }

    if (target_idx == -1) {
        return NULLBLOCK;
    }

    struct MEMORY_BLOCK allocated_block = memory_map[target_idx];

    if (allocated_block.segment_size == request_size) {
        memory_map[target_idx].process_id = process_id;
        return memory_map[target_idx];
    } else {
        struct MEMORY_BLOCK free_remainder;
        free_remainder.start_address = allocated_block.start_address + request_size;
        free_remainder.end_address = allocated_block.end_address;
        free_remainder.segment_size = allocated_block.segment_size - request_size;
        free_remainder.process_id = 0;

        for (int i = *map_count; i > target_idx + 1; i--) {
            memory_map[i] = memory_map[i - 1];
        }
        (*map_count)++;

        memory_map[target_idx].end_address = allocated_block.start_address + request_size - 1;
        memory_map[target_idx].segment_size = request_size;
        memory_map[target_idx].process_id = process_id;
        memory_map[target_idx + 1] = free_remainder;

        return memory_map[target_idx];
    }
}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_count, int process_id, int last_address) {
    int target_idx = -1;
    int start_search_idx = 0;

    // Locate where the search should begin based on the last allocated address
    for (int i = 0; i < *map_count; i++) {
        if (memory_map[i].start_address <= last_address && last_address <= memory_map[i].end_address) {
            start_search_idx = i;
            break;
        }
    }

    // Pass 1: Search from the last address to the end of the memory map
    for (int i = start_search_idx; i < *map_count; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            target_idx = i;
            break;
        }
    }

    // Pass 2: Wrap around to the beginning if no block was found
    if (target_idx == -1) {
        for (int i = 0; i < start_search_idx; i++) {
            if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
                target_idx = i;
                break;
            }
        }
    }

    if (target_idx == -1) {
        return NULLBLOCK;
    }

    struct MEMORY_BLOCK allocated_block = memory_map[target_idx];

    if (allocated_block.segment_size == request_size) {
        memory_map[target_idx].process_id = process_id;
        return memory_map[target_idx];
    } else {
        struct MEMORY_BLOCK free_remainder;
        free_remainder.start_address = allocated_block.start_address + request_size;
        free_remainder.end_address = allocated_block.end_address;
        free_remainder.segment_size = allocated_block.segment_size - request_size;
        free_remainder.process_id = 0;

        for (int i = *map_count; i > target_idx + 1; i--) {
            memory_map[i] = memory_map[i - 1];
        }
        (*map_count)++;

        memory_map[target_idx].end_address = allocated_block.start_address + request_size - 1;
        memory_map[target_idx].segment_size = request_size;
        memory_map[target_idx].process_id = process_id;
        memory_map[target_idx + 1] = free_remainder;

        return memory_map[target_idx];
    }
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_count) {
    int target_idx = -1;

    // Find the block matching the start address of the freed target
    for (int i = 0; i < *map_count; i++) {
        if (memory_map[i].start_address == freed_block.start_address) {
            target_idx = i;
            break;
        }
    }

    if (target_idx == -1) {
        return;
    }

    // Mark the selected block as free
    memory_map[target_idx].process_id = 0;

    // Merge with right neighbor if it is free
    if (target_idx + 1 < *map_count && memory_map[target_idx + 1].process_id == 0) {
        memory_map[target_idx].end_address = memory_map[target_idx + 1].end_address;
        memory_map[target_idx].segment_size += memory_map[target_idx + 1].segment_size;

        for (int i = target_idx + 1; i < (*map_count) - 1; i++) {
            memory_map[i] = memory_map[i + 1];
        }
        (*map_count)--;
    }

    // Merge with left neighbor if it is free
    if (target_idx - 1 >= 0 && memory_map[target_idx - 1].process_id == 0) {
        memory_map[target_idx - 1].end_address = memory_map[target_idx].end_address;
        memory_map[target_idx - 1].segment_size += memory_map[target_idx].segment_size;

        for (int i = target_idx; i < (*map_count) - 1; i++) {
            memory_map[i] = memory_map[i + 1];
        }
        (*map_count)--;
    }
}