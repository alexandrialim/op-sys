#include <stdio.h>
#include "oslabs.h"

// Helper function to print individual PTEs
void print_pte(struct PTE p) {
    printf("[%s, FN: %d, ATS: %d, LATS: %d, RC: %d]\n",
           p.is_valid ? "IV: true " : "IV: false", 
           p.frame_number, p.arrival_timestamp, p.last_access_timestamp, p.reference_count);
}

// Helper function to print the page table array
void print_page_table(struct PTE table[], int count) {
    for (int i = 0; i < count; i++) {
        print_pte(table[i]);
    }
}

// Helper function to print the frame pool array
void print_frame_pool(int pool[], int count) {
    if (count == 0) {
        printf("EMPTY\n");
    } else {
        printf("[");
        for (int i = 0; i < count; i++) {
            printf("%d%s", pool[i], i < count - 1 ? ", " : "");
        }
        printf("]\n");
    }
}

// Helper function to print section headers
void print_test_header(const char* test_name) {
    printf("\n====================================================\n");
    printf("%s\n", test_name);
    printf("====================================================\n");
}

int main() {
    // ---------------------------------------------------------
    // process_page_access_fifo
    // ---------------------------------------------------------
    print_test_header("process_page_access_fifo - Case 1: published test case");
    struct PTE pt_fifo_1[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 3, 3, 1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_fifo_1 = 8;
    int fp_fifo_1[POOLMAX] = {0};
    int fc_fifo_1 = 0;
    int res_fifo_1 = process_page_access_fifo(pt_fifo_1, &tc_fifo_1, 0, fp_fifo_1, &fc_fifo_1, 12);
    printf("Output page_table:\n"); print_page_table(pt_fifo_1, tc_fifo_1);
    printf("Output table_cnt: %d\nOutput frame_pool: ", tc_fifo_1); print_frame_pool(fp_fifo_1, fc_fifo_1);
    printf("Output frame_cnt: %d\nOutput int (frame number): %d\n", fc_fifo_1, res_fifo_1);

    print_test_header("process_page_access_fifo - Case 2: page is already in memory");
    struct PTE pt_fifo_2[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 3, 3, 1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_fifo_2 = 8;
    int fp_fifo_2[POOLMAX] = {0};
    int fc_fifo_2 = 0;
    int res_fifo_2 = process_page_access_fifo(pt_fifo_2, &tc_fifo_2, 2, fp_fifo_2, &fc_fifo_2, 14);
    printf("Output page_table:\n"); print_page_table(pt_fifo_2, tc_fifo_2);
    printf("Output table_cnt: %d\nOutput int (frame number): %d\n", tc_fifo_2, res_fifo_2);

    print_test_header("process_page_access_fifo - Case 3: page is not in memory but there is a free frame available");
    struct PTE pt_fifo_3[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_fifo_3 = 8;
    int fp_fifo_3[POOLMAX] = {10};
    int fc_fifo_3 = 1;
    int res_fifo_3 = process_page_access_fifo(pt_fifo_3, &tc_fifo_3, 3, fp_fifo_3, &fc_fifo_3, 15);
    printf("Output page_table:\n"); print_page_table(pt_fifo_3, tc_fifo_3);
    printf("Output table_cnt: %d\nOutput frame_pool: ", tc_fifo_3); print_frame_pool(fp_fifo_3, fc_fifo_3);
    printf("Output frame_cnt: %d\nOutput int (frame number): %d\n", fc_fifo_3, res_fifo_3);

    print_test_header("process_page_access_fifo - Case 4: page is not in memory, no free frames available");
    struct PTE pt_fifo_4[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 15, 15, 1},
        {0, -1, -1, -1, -1}, {1, 20, 12, 16, 2}, {0, -1, -1, -1, -1}, {1, 30, 14, 14, 1}
    };
    int tc_fifo_4 = 8;
    int fp_fifo_4[POOLMAX] = {0};
    int fc_fifo_4 = 0;
    int res_fifo_4 = process_page_access_fifo(pt_fifo_4, &tc_fifo_4, 2, fp_fifo_4, &fc_fifo_4, 17);
    printf("Output page_table:\n"); print_page_table(pt_fifo_4, tc_fifo_4);
    printf("Output table_cnt: %d\nOutput int (frame number): %d\n", tc_fifo_4, res_fifo_4);


    // ---------------------------------------------------------
    // count_page_faults_fifo
    // ---------------------------------------------------------
    print_test_header("count_page_faults_fifo - Case 1: published test case");
    struct PTE pt_cfifo_1[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}
    };
    int tc_cfifo_1 = 8;
    int ref_cfifo_1[REFERENCEMAX] = {0, 3, 2, 6, 3, 4, 5, 2, 4, 5, 6};
    int fp_cfifo_1[POOLMAX] = {0, 1, 2};
    int faults_cfifo_1 = count_page_faults_fifo(pt_cfifo_1, tc_cfifo_1, ref_cfifo_1, 11, fp_cfifo_1, 3);
    printf("Output faults: %d\n", faults_cfifo_1);

    print_test_header("count_page_faults_fifo - Case 2: different reference string");
    struct PTE pt_cfifo_2[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 3, 3, 1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_cfifo_2 = 8;
    int ref_cfifo_2[REFERENCEMAX] = {2, 5, 0, 7, 4, 2, 3, 5, 1, 2, 6, 0};
    int fp_cfifo_2[POOLMAX] = {0};
    int faults_cfifo_2 = count_page_faults_fifo(pt_cfifo_2, tc_cfifo_2, ref_cfifo_2, 12, fp_cfifo_2, 0);
    printf("Output faults: %d\n", faults_cfifo_2);


    // ---------------------------------------------------------
    // process_page_access_lru
    // ---------------------------------------------------------
    print_test_header("process_page_access_lru - Case 1: published test case");
    struct PTE pt_lru_1[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 3, 3, 1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_lru_1 = 8;
    int fp_lru_1[POOLMAX] = {0};
    int fc_lru_1 = 0;
    int res_lru_1 = process_page_access_lru(pt_lru_1, &tc_lru_1, 0, fp_lru_1, &fc_lru_1, 12);
    printf("Output page_table:\n"); print_page_table(pt_lru_1, tc_lru_1);
    printf("Output table_cnt: %d\nOutput int (frame number): %d\n", tc_lru_1, res_lru_1);

    print_test_header("process_page_access_lru - Case 2: page is already in memory");
    struct PTE pt_lru_2[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 3, 3, 1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_lru_2 = 8;
    int fp_lru_2[POOLMAX] = {0};
    int fc_lru_2 = 0;
    int res_lru_2 = process_page_access_lru(pt_lru_2, &tc_lru_2, 2, fp_lru_2, &fc_lru_2, 14);
    printf("Output page_table:\n"); print_page_table(pt_lru_2, tc_lru_2);
    printf("Output table_cnt: %d\nOutput int (frame number): %d\n", tc_lru_2, res_lru_2);

    print_test_header("process_page_access_lru - Case 3: page is not in memory but there is a free frame available");
    struct PTE pt_lru_3[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_lru_3 = 8;
    int fp_lru_3[POOLMAX] = {10};
    int fc_lru_3 = 1;
    int res_lru_3 = process_page_access_lru(pt_lru_3, &tc_lru_3, 3, fp_lru_3, &fc_lru_3, 15);
    printf("Output page_table:\n"); print_page_table(pt_lru_3, tc_lru_3);
    printf("Output table_cnt: %d\nOutput frame_pool: ", tc_lru_3); print_frame_pool(fp_lru_3, fc_lru_3);
    printf("Output frame_cnt: %d\nOutput int (frame number): %d\n", fc_lru_3, res_lru_3);

    print_test_header("process_page_access_lru - Case 4: page is not in memory, no free frames available");
    struct PTE pt_lru_4[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 15, 15, 1},
        {0, -1, -1, -1, -1}, {1, 20, 12, 16, 2}, {0, -1, -1, -1, -1}, {1, 30, 14, 14, 1}
    };
    int tc_lru_4 = 8;
    int fp_lru_4[POOLMAX] = {0};
    int fc_lru_4 = 0;
    int res_lru_4 = process_page_access_lru(pt_lru_4, &tc_lru_4, 2, fp_lru_4, &fc_lru_4, 17);
    printf("Output page_table:\n"); print_page_table(pt_lru_4, tc_lru_4);
    printf("Output table_cnt: %d\nOutput int (frame number): %d\n", tc_lru_4, res_lru_4);


    // ---------------------------------------------------------
    // count_page_faults_lru
    // ---------------------------------------------------------
    print_test_header("count_page_faults_lru - Case 1: published test case");
    struct PTE pt_clru_1[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}
    };
    int tc_clru_1 = 8;
    int ref_clru_1[REFERENCEMAX] = {0, 3, 2, 6, 3, 4, 5, 2, 4, 6, 5};
    int fp_clru_1[POOLMAX] = {0, 1, 2};
    int faults_clru_1 = count_page_faults_lru(pt_clru_1, tc_clru_1, ref_clru_1, 11, fp_clru_1, 3);
    printf("Output faults: %d\n", faults_clru_1);

    print_test_header("count_page_faults_lru - Case 2: different reference string");
    struct PTE pt_clru_2[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 3, 3, 1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_clru_2 = 8;
    int ref_clru_2[REFERENCEMAX] = {2, 5, 0, 2, 5, 2, 3, 5, 1, 2, 6, 0};
    int fp_clru_2[POOLMAX] = {0};
    int faults_clru_2 = count_page_faults_lru(pt_clru_2, tc_clru_2, ref_clru_2, 12, fp_clru_2, 0);
    printf("Output faults: %d\n", faults_clru_2);


    // ---------------------------------------------------------
    // process_page_access_lfu
    // ---------------------------------------------------------
    print_test_header("process_page_access_lfu - Case 1: published test case");
    struct PTE pt_lfu_1[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 3, 3, 1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_lfu_1 = 8;
    int fp_lfu_1[POOLMAX] = {0};
    int fc_lfu_1 = 0;
    int res_lfu_1 = process_page_access_lfu(pt_lfu_1, &tc_lfu_1, 0, fp_lfu_1, &fc_lfu_1, 12);
    printf("Output page_table:\n"); print_page_table(pt_lfu_1, tc_lfu_1);
    printf("Output table_cnt: %d\nOutput int (frame number): %d\n", tc_lfu_1, res_lfu_1);

    print_test_header("process_page_access_lfu - Case 2: page is already in memory");
    struct PTE pt_lfu_2[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 3, 3, 1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_lfu_2 = 8;
    int fp_lfu_2[POOLMAX] = {0};
    int fc_lfu_2 = 0;
    int res_lfu_2 = process_page_access_lfu(pt_lfu_2, &tc_lfu_2, 2, fp_lfu_2, &fc_lfu_2, 14);
    printf("Output page_table:\n"); print_page_table(pt_lfu_2, tc_lfu_2);
    printf("Output table_cnt: %d\nOutput int (frame number): %d\n", tc_lfu_2, res_lfu_2);

    print_test_header("process_page_access_lfu - Case 3: page is not in memory but there is a free frame available");
    struct PTE pt_lfu_3[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_lfu_3 = 8;
    int fp_lfu_3[POOLMAX] = {10};
    int fc_lfu_3 = 1;
    int res_lfu_3 = process_page_access_lfu(pt_lfu_3, &tc_lfu_3, 3, fp_lfu_3, &fc_lfu_3, 15);
    printf("Output page_table:\n"); print_page_table(pt_lfu_3, tc_lfu_3);
    printf("Output table_cnt: %d\nOutput frame_pool: ", tc_lfu_3); print_frame_pool(fp_lfu_3, fc_lfu_3);
    printf("Output frame_cnt: %d\nOutput int (frame number): %d\n", fc_lfu_3, res_lfu_3);

    print_test_header("process_page_access_lfu - Case 4: page is not in memory, no free frames available");
    struct PTE pt_lfu_4[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 15, 15, 1},
        {0, -1, -1, -1, -1}, {1, 20, 12, 16, 2}, {0, -1, -1, -1, -1}, {1, 30, 14, 17, 4}
    };
    int tc_lfu_4 = 8;
    int fp_lfu_4[POOLMAX] = {0};
    int fc_lfu_4 = 0;
    int res_lfu_4 = process_page_access_lfu(pt_lfu_4, &tc_lfu_4, 2, fp_lfu_4, &fc_lfu_4, 18);
    printf("Output page_table:\n"); print_page_table(pt_lfu_4, tc_lfu_4);
    printf("Output table_cnt: %d\nOutput int (frame number): %d\n", tc_lfu_4, res_lfu_4);


    // ---------------------------------------------------------
    // count_page_faults_lfu
    // ---------------------------------------------------------
    print_test_header("count_page_faults_lfu - Case 1: published test case");
    struct PTE pt_clfu_1[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}
    };
    int tc_clfu_1 = 8;
    int ref_clfu_1[REFERENCEMAX] = {0, 3, 2, 6, 3, 4, 5, 2, 6, 4, 5};
    int fp_clfu_1[POOLMAX] = {0, 1, 2};
    int faults_clfu_1 = count_page_faults_lfu(pt_clfu_1, tc_clfu_1, ref_clfu_1, 11, fp_clfu_1, 3);
    printf("Output faults: %d\n", faults_clfu_1);

    print_test_header("count_page_faults_lfu - Case 2: different reference string");
    struct PTE pt_clfu_2[TABLEMAX] = {
        {0, -1, -1, -1, -1}, {0, -1, -1, -1, -1}, {1, 10, 3, 3, 1}, {0, -1, -1, -1, -1},
        {0, -1, -1, -1, -1}, {1, 20, 2, 4, 2}, {0, -1, -1, -1, -1}, {1, 30, 1, 1, 1}
    };
    int tc_clfu_2 = 8;
    int ref_clfu_2[REFERENCEMAX] = {2, 5, 0, 2, 5, 2, 3, 5, 1, 2, 6, 0};
    int fp_clfu_2[POOLMAX] = {0};
    int faults_clfu_2 = count_page_faults_lfu(pt_clfu_2, tc_clfu_2, ref_clfu_2, 12, fp_clfu_2, 0);
    printf("Output faults: %d\n", faults_clfu_2);

    return 0;
}