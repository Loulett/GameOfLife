#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>
#include <omp.h>

size_t neighbours(std::vector<std::vector<std::vector<int>>>& field, size_t i, size_t j, size_t h, size_t w, size_t turn) {
	int t = 0, b = 0, l = 0, r = 0;

	t = i == 0 ? w - 1 : i - 1;
	b = (i + 1) % w;
	l = j == 0 ? h - 1 : j - 1;
	r = (j + 1) % h;

	std::vector<int> n = { field[(turn + 1) % 2][t][j], field[(turn + 1) % 2][t][r], field[(turn + 1) % 2][i][r], field[(turn + 1) % 2][b][r], field[(turn + 1) % 2][b][j], field[(turn + 1) % 2][b][l], field[(turn + 1) % 2][i][l], field[(turn + 1) % 2][t][l] };
	return std::accumulate(n.begin(), n.end(), 0);
}

void worker(std::vector<std::vector<std::vector<int>>>& field, size_t start, size_t finish, size_t h, size_t w, size_t turn) {
	for (size_t i = start; i < finish; i++) {
		for (size_t j = 0; j < w; j++) {
			if (field[(turn + 1) % 2][i][j] == 0 && neighbours(field, i, j, h, w, turn) == 3) {
				field[turn % 2][i][j] = 1;
			}
			else if (field[(turn + 1) % 2][i][j] == 1 && (neighbours(field, i, j, h, w, turn) == 2 || neighbours(field, i, j, h, w, turn) == 3)) {
				field[turn % 2][i][j] = 1;
			}
			else {
				field[turn % 2][i][j] = 0;
			}
		}
	}
}

double TaskWorker(size_t w, size_t h, size_t threads, size_t turns, std::vector<std::vector<std::vector<int>>>& data) {

	double start = 0, finish = 0;
	start = omp_get_wtime();
	int j = 1;
	size_t i = 0, k = 0;

#pragma omp parallel shared(data, w, h, j) num_threads(threads) private(i, k)
	{
#pragma omp for
	for (j = 1; j <= turns; j++) {
		int range = h / threads;
		for (i = 0; i < h; i++) {
			for (k = 0; k < w; k++) {
				if (data[(j + 1) % 2][i][k] == 0 && neighbours(data, i, k, h, w, j) == 3) {
					data[j % 2][i][k] = 1;
				}
				else if (data[(j + 1) % 2][i][k] == 1 && (neighbours(data, i, k, h, w, j) == 2 || neighbours(data, i, k, h, w, j) == 3)) {
					data[j % 2][i][k] = 1;
				}
				else {
					data[j % 2][i][k] = 0;
				}
			}
		}
	}
#pragma omp barrier
    }
	finish = omp_get_wtime();

	/*for (size_t i = 0; i < h; i++) {
	for (size_t j = 0; j < w; j++) {
	std::cout << data[turns % 2][i][j] << ' ';
	}
	std::cout << '\n';
	}*/


	//std::chrono::duration<double> elapsed_seconds = end - start;
	//std::cout << "time with " << threads << " threads = " << elapsed_seconds.count() << std::endl;
	return finish - start;


}

int main() {

	std::ifstream finp;
	std::ofstream fout;
	finp.open("input.txt");
	fout.open("output.txt");

	size_t w = 0, h = 0, turns = 0, threads = 0;
	finp >> w >> h;
	finp >> turns >> threads;

	std::vector<std::vector<std::vector<int>>> data(2, std::vector<std::vector<int>>(h, std::vector<int>(w)));
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			finp >> data[0][i][j];
		}
	}

	for (size_t i = 2; i <= 20; i = i + 2) {
		fout << "time with " << i << " threads = " << TaskWorker(w, h, i, turns, ref(data)) << std::endl;
	}

	return 0;
}