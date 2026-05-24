#include <stdio.h>
#include "oslabs.h"

void print_block(struct MEMORY_BLOCK b) {
    if (b.start_address == 0 && b.end_address == 0 && b.segment_size == 0 && b.process_id == 0) {
        printf("NULLBLOCK\n");
    } else {
        printf("[start_address: %d, end_address: %d, segment_size: %d, process_id: %d]\n",
               b.start_address, b.end_address, b.segment_size, b.process_id);
    }
}

void print_map(struct MEMORY_BLOCK map[], int count) {
    if (count == 0) {
        printf("EMPTY\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("  ");
            print_block(map[i]);
        }
    }
}

void print_test_header(const char* test_name) {
    printf("\n====================================================\n");
    printf("%s\n", test_name);
    printf("====================================================\n");
}

int main() {
    // ---------------------------------------------------------
    // Best Fit Allocation Tests
    // ---------------------------------------------------------
    print_test_header("best_fit_allocate - Case 1: Shared Test Case");
    struct MEMORY_BLOCK m_bf_c1[MAPMAX] = {{0, 1023, 1024, 0}}; 
    int cnt_bf_c1 = 1;
    struct MEMORY_BLOCK r_bf_c1 = best_fit_allocate(10, m_bf_c1, &cnt_bf_c1, 32);
    printf("Output MEMORY_BLOCK: "); print_block(r_bf_c1);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_bf_c1); 
    print_map(m_bf_c1, cnt_bf_c1);

    print_test_header("best_fit_allocate - Case 2: Free block is exactly the same size");
    struct MEMORY_BLOCK m_bf_c2[MAPMAX] = {
        {0, 19, 20, 10}, {20, 39, 20, 0}, {40, 49, 10, 20}, {50, 1023, 974, 0}
    }; 
    int cnt_bf_c2 = 4;
    struct MEMORY_BLOCK r_bf_c2 = best_fit_allocate(20, m_bf_c2, &cnt_bf_c2, 40);
    printf("Output MEMORY_BLOCK: "); print_block(r_bf_c2);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_bf_c2); 
    print_map(m_bf_c2, cnt_bf_c2);

    print_test_header("best_fit_allocate - Case 3: All free blocks smaller than request");
    struct MEMORY_BLOCK m_bf_c3[MAPMAX] = {
        {0, 19, 20, 10}, {20, 39, 20, 0}, {40, 49, 10, 20}, {50, 1023, 974, 30}
    }; 
    int cnt_bf_c3 = 4;
    struct MEMORY_BLOCK r_bf_c3 = best_fit_allocate(30, m_bf_c3, &cnt_bf_c3, 40);
    printf("Output MEMORY_BLOCK: "); print_block(r_bf_c3);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_bf_c3); 
    print_map(m_bf_c3, cnt_bf_c3);

    print_test_header("best_fit_allocate - Case 4: Free block is larger, request splits block");
    struct MEMORY_BLOCK m_bf_c4[MAPMAX] = {
        {0, 19, 20, 0}, {20, 39, 20, 20}, {40, 54, 15, 0}, {55, 1023, 969, 30}
    }; 
    int cnt_bf_c4 = 4;
    struct MEMORY_BLOCK r_bf_c4 = best_fit_allocate(10, m_bf_c4, &cnt_bf_c4, 40);
    printf("Output MEMORY_BLOCK: "); print_block(r_bf_c4);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_bf_c4); 
    print_map(m_bf_c4, cnt_bf_c4);


    // ---------------------------------------------------------
    // First Fit Allocation Tests
    // ---------------------------------------------------------
    print_test_header("first_fit_allocate - Case 1: Published Case");
    struct MEMORY_BLOCK m_ff_c1[MAPMAX] = {{0, 1023, 1024, 0}}; 
    int cnt_ff_c1 = 1;
    struct MEMORY_BLOCK r_ff_c1 = first_fit_allocate(10, m_ff_c1, &cnt_ff_c1, 32);
    printf("Output MEMORY_BLOCK: "); print_block(r_ff_c1);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_ff_c1); 
    print_map(m_ff_c1, cnt_ff_c1);

    print_test_header("first_fit_allocate - Case 2: Free block is exactly the same size");
    struct MEMORY_BLOCK m_ff_c2[MAPMAX] = {
        {0, 19, 20, 10}, {20, 39, 20, 0}, {40, 49, 10, 20}, {50, 1023, 974, 0}
    }; 
    int cnt_ff_c2 = 4;
    struct MEMORY_BLOCK r_ff_c2 = first_fit_allocate(20, m_ff_c2, &cnt_ff_c2, 40);
    printf("Output MEMORY_BLOCK: "); print_block(r_ff_c2);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_ff_c2); 
    print_map(m_ff_c2, cnt_ff_c2);

    print_test_header("first_fit_allocate - Case 3: All free blocks smaller than request");
    struct MEMORY_BLOCK m_ff_c3[MAPMAX] = {
        {0, 19, 20, 10}, {20, 39, 20, 0}, {40, 49, 10, 20}, {50, 1023, 974, 30}
    }; 
    int cnt_ff_c3 = 4;
    struct MEMORY_BLOCK r_ff_c3 = first_fit_allocate(30, m_ff_c3, &cnt_ff_c3, 40);
    printf("Output MEMORY_BLOCK: "); print_block(r_ff_c3);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_ff_c3); 
    print_map(m_ff_c3, cnt_ff_c3);

    print_test_header("first_fit_allocate - Case 4: Free block is larger, request splits block");
    struct MEMORY_BLOCK m_ff_c4[MAPMAX] = {
        {0, 19, 20, 0}, {20, 39, 20, 20}, {40, 54, 15, 0}, {55, 1023, 969, 30}
    }; 
    int cnt_ff_c4 = 4;
    struct MEMORY_BLOCK r_ff_c4 = first_fit_allocate(15, m_ff_c4, &cnt_ff_c4, 40);
    printf("Output MEMORY_BLOCK: "); print_block(r_ff_c4);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_ff_c4); 
    print_map(m_ff_c4, cnt_ff_c4);


    // ---------------------------------------------------------
    // Worst Fit Allocation Tests
    // ---------------------------------------------------------
    print_test_header("worst_fit_allocate - Case 1: Published Case");
    struct MEMORY_BLOCK m_wf_c1[MAPMAX] = {{0, 1023, 1024, 0}}; 
    int cnt_wf_c1 = 1;
    struct MEMORY_BLOCK r_wf_c1 = worst_fit_allocate(10, m_wf_c1, &cnt_wf_c1, 32);
    printf("Output MEMORY_BLOCK: "); print_block(r_wf_c1);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_wf_c1); 
    print_map(m_wf_c1, cnt_wf_c1);

    print_test_header("worst_fit_allocate - Case 2: Free block is exactly the same size");
    struct MEMORY_BLOCK m_wf_c2[MAPMAX] = {
        {0, 19, 20, 10}, {20, 39, 20, 0}, {40, 49, 10, 20}, {50, 1023, 974, 30}
    }; 
    int cnt_wf_c2 = 4;
    struct MEMORY_BLOCK r_wf_c2 = worst_fit_allocate(20, m_wf_c2, &cnt_wf_c2, 40);
    printf("Output MEMORY_BLOCK: "); print_block(r_wf_c2);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_wf_c2); 
    print_map(m_wf_c2, cnt_wf_c2);

    print_test_header("worst_fit_allocate - Case 3: All free blocks smaller than request");
    struct MEMORY_BLOCK m_wf_c3[MAPMAX] = {
        {0, 19, 20, 10}, {20, 39, 20, 0}, {40, 49, 10, 20}, {50, 1023, 974, 30}
    }; 
    int cnt_wf_c3 = 4;
    struct MEMORY_BLOCK r_wf_c3 = worst_fit_allocate(30, m_wf_c3, &cnt_wf_c3, 40);
    printf("Output MEMORY_BLOCK: "); print_block(r_wf_c3);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_wf_c3); 
    print_map(m_wf_c3, cnt_wf_c3);

    print_test_header("worst_fit_allocate - Case 4: Free block is larger, request splits block");
    struct MEMORY_BLOCK m_wf_c4[MAPMAX] = {
        {0, 19, 20, 0}, {20, 39, 20, 20}, {40, 54, 15, 30}, {55, 1023, 969, 0}
    }; 
    int cnt_wf_c4 = 4;
    struct MEMORY_BLOCK r_wf_c4 = worst_fit_allocate(10, m_wf_c4, &cnt_wf_c4, 40);
    printf("Output MEMORY_BLOCK: "); print_block(r_wf_c4);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_wf_c4); 
    print_map(m_wf_c4, cnt_wf_c4);


    // ---------------------------------------------------------
    // Next Fit Allocation Tests
    // ---------------------------------------------------------
    print_test_header("next_fit_allocate - Case 1: Published Case");
    struct MEMORY_BLOCK m_nf_c1[MAPMAX] = {{0, 1023, 1024, 0}}; 
    int cnt_nf_c1 = 1;
    struct MEMORY_BLOCK r_nf_c1 = next_fit_allocate(10, m_nf_c1, &cnt_nf_c1, 32, 0);
    printf("Output MEMORY_BLOCK: "); print_block(r_nf_c1);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_nf_c1); 
    print_map(m_nf_c1, cnt_nf_c1);

    print_test_header("next_fit_allocate - Case 2: Free block is exactly the same size");
    struct MEMORY_BLOCK m_nf_c2[MAPMAX] = {
        {0, 19, 20, 10}, {20, 39, 20, 0}, {40, 49, 10, 20}, {50, 1023, 974, 30}
    }; 
    int cnt_nf_c2 = 4;
    struct MEMORY_BLOCK r_nf_c2 = next_fit_allocate(20, m_nf_c2, &cnt_nf_c2, 40, 0);
    printf("Output MEMORY_BLOCK: "); print_block(r_nf_c2);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_nf_c2); 
    print_map(m_nf_c2, cnt_nf_c2);

    print_test_header("next_fit_allocate - Case 3: All free blocks smaller than request");
    struct MEMORY_BLOCK m_nf_c3[MAPMAX] = {
        {0, 19, 20, 10}, {20, 39, 20, 0}, {40, 49, 10, 20}, {50, 1023, 974, 30}
    }; 
    int cnt_nf_c3 = 4;
    struct MEMORY_BLOCK r_nf_c3 = next_fit_allocate(30, m_nf_c3, &cnt_nf_c3, 40, 0);
    printf("Output MEMORY_BLOCK: "); print_block(r_nf_c3);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_nf_c3); 
    print_map(m_nf_c3, cnt_nf_c3);

    print_test_header("next_fit_allocate - Case 4: Free block is larger, request splits block");
    struct MEMORY_BLOCK m_nf_c4[MAPMAX] = {
        {0, 19, 20, 0}, {20, 39, 20, 20}, {40, 54, 15, 0}, {55, 74, 20, 30}, {75, 1023, 949, 0}
    }; 
    int cnt_nf_c4 = 5;
    struct MEMORY_BLOCK r_nf_c4 = next_fit_allocate(30, m_nf_c4, &cnt_nf_c4, 40, 2);
    printf("Output MEMORY_BLOCK: "); print_block(r_nf_c4);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_nf_c4); 
    print_map(m_nf_c4, cnt_nf_c4);


    // ---------------------------------------------------------
    // Release Memory Tests
    // ---------------------------------------------------------
    print_test_header("release_memory - Case 1: Published Case");
    struct MEMORY_BLOCK m_rel_c1[MAPMAX] = {
        {0, 7, 8, 12}, {8, 15, 8, 0}, {16, 23, 8, 13}, {24, 27, 4, 0}, {28, 29, 2, 11}
    }; 
    int cnt_rel_c1 = 5;
    struct MEMORY_BLOCK f_rel_c1 = {16, 23, 8, 13};
    release_memory(f_rel_c1, m_rel_c1, &cnt_rel_c1);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_rel_c1); 
    print_map(m_rel_c1, cnt_rel_c1);

    print_test_header("release_memory - Case 2: Adjacent block before newly freed block is merged");
    struct MEMORY_BLOCK m_rel_c2[MAPMAX] = {
        {0, 19, 20, 0}, {20, 39, 20, 20}, {40, 54, 15, 40}, {55, 74, 20, 30}, {75, 1023, 949, 0}
    }; 
    int cnt_rel_c2 = 5;
    struct MEMORY_BLOCK f_rel_c2 = {20, 39, 20, 20};
    release_memory(f_rel_c2, m_rel_c2, &cnt_rel_c2);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_rel_c2); 
    print_map(m_rel_c2, cnt_rel_c2);

    print_test_header("release_memory - Case 3: Adjacent block after newly freed block is merged");
    struct MEMORY_BLOCK m_rel_c3[MAPMAX] = {
        {0, 19, 20, 0}, {20, 39, 20, 20}, {40, 54, 15, 40}, {55, 74, 20, 30}, {75, 1023, 949, 0}
    }; 
    int cnt_rel_c3 = 5;
    struct MEMORY_BLOCK f_rel_c3 = {55, 74, 20, 30};
    release_memory(f_rel_c3, m_rel_c3, &cnt_rel_c3);
    printf("Output map_cnt: %d\nOutput memory_map:\n", cnt_rel_c3); 
    print_map(m_rel_c3, cnt_rel_c3);

    return 0;
}