#include "stdafx.h"
#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	int size = 0;
	int *arr, *sizes, *disps, *arr1, *ans1;
	size = 10;
	int num_of_workers, my_num, ans = 0;
	MPI_Comm_size(MPI_COMM_WORLD, &num_of_workers);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_num);
	arr = (int*)malloc(size * sizeof(int));
	sizes = (int*)malloc(num_of_workers * sizeof(int));
	disps = (int*)malloc(num_of_workers * sizeof(int));
	ans1 = (int*)malloc(num_of_workers * sizeof(int));
	for (int i = 0; i < num_of_workers - 1; i++) {
		sizes[i] = size / num_of_workers;
		disps[i] = i*sizes[i];
	}
	sizes[num_of_workers - 1] = size - (num_of_workers - 1) * (size / num_of_workers);
	disps[num_of_workers - 1] = size - sizes[num_of_workers - 1];
	if (my_num == 0) {
		for (int i = 0; i < size; i++) {
			arr[i] = i*i;
		}
	}
	arr1 = (int*)malloc(num_of_workers + size / num_of_workers);
	MPI_Scatterv(arr, sizes, disps, MPI_INT, arr1, size, MPI_INT, 0, MPI_COMM_WORLD);
	free(arr);
	free(disps);
	printf("size %d rank %d\n", sizes[my_num], my_num);
	for (int i = 0; i < sizes[my_num]; i++) {
		ans += arr1[i];
	}
	free(sizes);
	MPI_Gather(&ans, 1, MPI_INT, ans1, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (my_num == 0) {
		int res = 0;
		for (int i = 0; i < num_of_workers; i++) {
			res += ans1[i];
		}
		free(ans1);
		printf("%d\n", res);
	}

	MPI_Finalize();
}