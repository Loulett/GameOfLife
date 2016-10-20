#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

int main() {
	std::ifstream finp("input.txt");
	int n = 0, ans = 0, i = 0, pred_ans = 0;
	double start = 0, finish = 0;
	finp >> n;
	std::vector<int> vec(n);
	for (i = 0; i < n; i++) {
		finp >> vec[i];
	}
	finp.close();
	start = omp_get_wtime();
#pragma omp parallel shared(vec, ans, n) private(i) firstprivate(pred_ans)
	{
#pragma omp for
		for (i = 0; i < n; i++)
		{
			pred_ans += vec[i];
		}
#pragma omp atomic
			ans += pred_ans;
	}
	finish = omp_get_wtime();
	std::cout << ans << '\n';
	std::cout << finish - start;
	return 0;
}