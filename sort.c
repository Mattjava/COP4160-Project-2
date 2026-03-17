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