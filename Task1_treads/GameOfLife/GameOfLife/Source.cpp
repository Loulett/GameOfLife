#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>

size_t neighbours(std::vector<std::vector<int>>& field, size_t i, size_t j, size_t h, size_t w) {
	int t = 0, b = 0, l = 0, r = 0;

	t = i == 0 ? w - 1 : i - 1;
	b = (i + 1) % w;
	l = j == 0 ? h - 1 : j - 1;
	r = (j + 1) % h;

	std::vector<int> n = { field[t][j], field[t][r], field[i][r], field[b][r], field[b][j], field[b][l], field[i][l], field[t][l] };
	return std::accumulate(n.begin(), n.end(), 0);
}

void worker(std::vector<std::vector<int>>& field, std::vector<std::vector<int>>& new_field, size_t start, size_t finish, size_t h, size_t w) {
	for (size_t i = start; i < finish; i++) {
		for (size_t j = 0; j < w; j++) {
			if (field[i][j] == 0 && neighbours(field, i, j, h, w) == 3) {
				new_field[i][j] = 1;
			}
			else if (field[i][j] == 1 && (neighbours(field, i, j, h, w) == 2 || neighbours(field, i, j, h, w) == 3)) {
				new_field[i][j] = 1;
			}
			else {
				new_field[i][j] = 0;
			}
		}
	}
}

void simpleWorker(size_t w, size_t h, size_t threads, size_t turns, std::vector<std::vector<int>>& data, std::vector<std::vector<int>>& new_data) {

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	size_t t = turns;

	//size_t threads = std::thread::hardware_concurrency();
	while (turns > 0) {
		std::vector<std::thread> workers;

		int range = h / threads;
		if ((t - turns) % 2 == 0) {
			for (int i = 0; i < threads; i++) {
				workers.push_back(std::thread(worker, ref(data), ref(new_data), i * range, std::min(h, (i + 1u) * range), h, w));
			}
		}
		else {
			for (int i = 0; i < threads; i++) {
				workers.push_back(std::thread(worker, ref(new_data), ref(data), i * range, std::min(h, (i + 1u) * range), h, w));
			}
		}

		for (int i = 0; i < threads; i++) {
			workers[i].join();
		}

		turns--;
	}
	end = std::chrono::system_clock::now();

	if (t % 2 == 0) {
		for (size_t i = 0; i < h; i++) {
			for (size_t j = 0; j < w; j++) {
				std::cout << data[i][j] << ' ';
			}
			std::cout << '\n';
		}
	}
	else {
		for (size_t i = 0; i < h; i++) {
			for (size_t j = 0; j < w; j++) {
				std::cout << new_data[i][j] << ' ';
			}
			std::cout << '\n';
		}
	}


	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "time with " << threads << " threads = " << elapsed_seconds.count() << std::endl;


}

int main() {

	std::ifstream finp;
	finp.open("input.txt");

	size_t w = 0, h = 0, turns = 0, threads = 0;
	finp >> w >> h;
	finp >> turns >> threads;

	std::vector<std::vector<int>> data(h, std::vector<int>(w));
	std::vector<std::vector<int>> new_data(h, std::vector<int>(w));
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			finp >> data[i][j];
		}
	}

	simpleWorker(w, h, threads, turns, ref(data), ref(new_data));

	/*for (size_t i = 1; i < 20; i++) {
		simpleWorker(w, h, i, turns, ref(data), ref(new_data));
	}*/

	return 0;
}