#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define SIZE 10

/* Global arrays */
int original[SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int result[SIZE];

/* Parameters passed to each sorting thread */
typedef struct {
    int start;
    int size;
} SortParams;

int* merge(int* segment1, int* segment2, int size1, int size2) {
    int size3 = size1 + size2;
    int point1 = 0;
    int point2 = 0;
    int point3 = 0;

    int* result = (int*) malloc(size3 * sizeof(int));
    while(point1 < size1 && point2 < size2) {
        if(segment1[point1] < segment2[point2]) {
            result[point3] = segment1[point1];
            point1 += 1;
        } else {
            result[point3] = segment2[point2];
            point2 += 1;
        }
        point3 += 1;
    }

    while(point1 < size1) {
        result[point3] = segment1[point1];
        point1 += 1;
        point3 += 1;
    }

    while(point2 < size2) {
        result[point3] = segment2[point2];
        point2 += 1;
        point3 += 1;
    }

    return result;
}

/*
    Based off of the merge sort algorithm
    Make sure the input array is dynamically allocated to ensure the the code runs smoothly.
*/
int* mergeSort(int* arr, int size) {
    if (size < 2) {
        int* result = malloc(size * sizeof(int));
        for(int i = 0; i < size; i++) {
            result[i] = arr[i];
        }
        return result;
    }

    int half = size / 2;
    int* segment1 = (int*) malloc(half * sizeof(int));
    int* segment2 = (int*) malloc((size - half) * sizeof(int));
    int point1 = 0;
    int point2 = 0;

    for(int i = 0; i < size; i++) {
        if(point1 < half) {
            segment1[point1] = arr[i];
            point1 += 1;
        } else {
            segment2[point2] = arr[i];
            point2 += 1;
        }
    }

    int* sorted1 = mergeSort(segment1, half);
    int* sorted2 = mergeSort(segment2, size - half);
    free(segment1);
    free(segment2);

    int* result = merge(sorted1, sorted2, half, size - half);
    free(sorted1);
    free(sorted2);

    return result;

}

/* Sorting thread: sorts one half of original[] in place */
void *sort_thread(void *arg) {
    SortParams *p = (SortParams *)arg;
    printf("Sorting thread: sorting %d elements starting at index %d\n",
           p->size, p->start);

    int *sorted = mergeSort(&original[p->start], p->size);
    memcpy(&original[p->start], sorted, p->size * sizeof(int));
    free(sorted);

    pthread_exit(NULL);
}

/* Merge thread: merges the two sorted halves into result[] */
void *merge_thread(void *arg) {
    int mid = *(int *)arg;
    int size1 = mid;
    int size2 = SIZE - mid;

    printf("Merge thread: merging first %d and last %d elements into result[]\n",
           size1, size2);

    int *merged = merge(original, &original[mid], size1, size2);
    memcpy(result, merged, SIZE * sizeof(int));
    free(merged);

    pthread_exit(NULL);
}

void print_array(const char *label, int arr[], int n) {
    printf("%s", label);
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("\n");
}

int main(void) {
    pthread_t tid_sort1, tid_sort2, tid_merge;
    int mid = SIZE / 2;

    print_array("Original array: ", original, SIZE);

    /* Set up parameters for sorting threads */
    SortParams params1 = {0, mid};
    SortParams params2 = {mid, SIZE - mid};

    /* Create two sorting threads */
    if (pthread_create(&tid_sort1, NULL, sort_thread, &params1) != 0) {
        perror("Failed to create sorting thread 1");
        return 1;
    }
    if (pthread_create(&tid_sort2, NULL, sort_thread, &params2) != 0) {
        perror("Failed to create sorting thread 2");
        return 1;
    }

    /* Wait for both sorting threads to finish */
    pthread_join(tid_sort1, NULL);
    pthread_join(tid_sort2, NULL);
    print_array("After sorting:  ", original, SIZE);

    /* Create the merging thread */
    if (pthread_create(&tid_merge, NULL, merge_thread, &mid) != 0) {
        perror("Failed to create merge thread");
        return 1;
    }

    /* Wait for merge thread to finish */
    pthread_join(tid_merge, NULL);

    /* Parent thread outputs the final sorted array */
    print_array("Sorted array:   ", result, SIZE);

    return 0;
}