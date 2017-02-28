#include <stdlib.h>
#include <stdio.h>

int invCount(int *, int, int);
int invCountMerge(int *, int, int, int);

int main(int argc, char **argv) {

    int n = 0, i, *v;


    if (argc < 2) {
        fprintf(stderr, "Usage: invCount <elements_count>\n");
        exit(-1);
    }

    n = atoi(argv[1]);

    if (n<=0) {
        fprintf(stderr, "Invalid number of elements provided\n");
        exit(-1);
    }

    v = (int*) malloc (n * sizeof(int));
    if (v == NULL) {
        fprintf(stderr, "Allocation failed\n");
        exit(-1);
    }

    fprintf(stdout, "Please type the %d element(s) required to fill the array\n", n);
    for(i=0;i<n;i++) {
        fprintf(stdout, "Element %d of %d > ", i+1, n);
        fscanf(stdin, "%d", &v[i]);
        if (v[i] < 0 || v[i] >= n) {
            fprintf(stderr, "Invalid element\n");
            exit(-1);
        }
    }

    fprintf(stdout, "%d inversions in given array\n", invCount(v, 0, n-1));
    free(v);

    return 0;
}

int invCount(int *v, int left, int right) {
    if (right <= left)
        return 0;
    int mid = (left + right) / 2;
    int count = invCount(v, left, mid);
    count += invCount(v, mid+1, right);
    count += invCountMerge(v, left, mid, right);
    return count;
}

int invCountMerge(int *v, int left, int mid, int right) {
    int i = left, j = mid+1, k = 0;
    int *tmp = (int*) malloc((right-left+1) * sizeof(int));

    if (tmp == NULL) {
        fprintf(stderr, "Allocation failed\n");
        exit(-1);
    }

    int count = 0;

    // Scan the two left and right subarrays to merge them,
    // if an element v[j] is selected from the left sub-array (else branch)
    // it will actually be smaller that all the elements
    // still in the right subarray, thus increment count accordingly
    while ((i <= mid) && (j <= right))
    {
        if (v[i] <= v[j])
        {
            tmp[k++] = v[i++];
        } else {
            tmp[k++] = v[j++];
            count += (mid - i +1);
        }
    }

    // Leftover elements
    while (i <= mid)
        tmp[k++] = v[i++];

    while (j <= right)
        tmp[k++] = v[j++];

    // Copy back to original v
    for (i=left, k=0; i <= right; i++)
        v[i] = tmp[k++];

    free(tmp);
    return count;
}
