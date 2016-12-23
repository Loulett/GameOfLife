#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>

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

void worker1(std::vector<std::vector<std::vector<int>>>& field, size_t start, size_t finish, size_t h, size_t w, size_t turn) {
	for (size_t i = start; i < finish; i++) {
		for (size_t j = 0; j < w; j++) {
			if (field[(turn + 1) % 2][i][j] == -1) {
				return;
			}
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

void worker2(std::vector<std::vector<std::vector<int>>>& field, size_t start, size_t finish, size_t h, size_t w, size_t turn) {
	for (size_t i = finish - 1; i <= start; i--) {
		for (size_t j = w - 1; j <= 0; j--) {
			if (field[(turn + 1) % 2][i][j] == -1) {
				return;
			}
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

double simpleWorker(size_t w, size_t h, size_t threads, size_t turns, std::vector<std::vector<std::vector<int>>>& data) {

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	//size_t threads = std::thread::hardware_concurrency();
	for (int j = 1; j <= turns; j++) {
		std::vector<std::thread> workers;

		int range = h / threads;
		for (int i = 0; i < threads; i++) {
				workers.push_back(std::thread(worker, ref(data), i * range, std::min(h, (i + 1u) * range), h, w, j));
		}

		for (int i = 0; i < threads; i++) {
			workers[i].join();
		}

	}
	end = std::chrono::system_clock::now();

	/*for (size_t i = 0; i < h; i++) {
		for (size_t j = 0; j < w; j++) {
			std::cout << data[turns % 2][i][j] << ' ';
		}
		std::cout << '\n';
	}*/


	std::chrono::duration<double> elapsed_seconds = end - start;
	//std::cout << "time with " << threads << " threads = " << elapsed_seconds.count() << std::endl;
	return elapsed_seconds.count();


}

double cleverWorker(size_t w, size_t h, size_t threads, size_t turns, std::vector<std::vector<std::vector<int>>>& data) {
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	for (int j = 1; j <= turns; j++) {


		std::vector<std::thread> workers;

		for (int i = 0; i < h; i++) {
			for (int k = 0; k < w; k++) {
				data[j % 2][i][k] = -1;
			}
		}

		int range = h / (threads / 2);
		for (int i = 0; i < threads / 2; i++) {
			workers.push_back(std::thread(worker1, ref(data), i * range, std::min(h, (i + 1u) * range), h, w, j));
			workers.push_back(std::thread(worker2, ref(data), i * range, std::min(h, (i + 1u) * range), h, w, j));
		}

		for (int i = 0; i < threads; i++) {
			workers[i].join();
		}

	}
	end = std::chrono::system_clock::now();
	/*for (size_t i = 0; i < h; i++) {
	for (size_t j = 0; j < w; j++) {
	std::cout << data[turns % 2][i][j] << ' ';
	}
	std::cout << '\n';
	}*/


	std::chrono::duration<double> elapsed_seconds = end - start;
	//std::cout << "time with " << threads << " threads = " << elapsed_seconds.count() << std::endl;
	return elapsed_seconds.count();
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

	//cleverWorker(w, h, threads, turns, ref(data));

	for (size_t i = 2; i <= 20; i=i+2) {
		fout << "time with " << i << " threads = " << cleverWorker(w, h, i, turns, ref(data)) << std::endl;
	}

	return 0;
}