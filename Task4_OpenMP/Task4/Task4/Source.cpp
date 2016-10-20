#include <iostream>
#include <vector>
#include <omp.h>

int main() {
	int n = 0, ans = 0, i = 0, pred_ans = 0;
	std::cin >> n;
	std::vector<int> vec(n);
	for (i = 0; i < n; i++) {
		std::cin >> vec[i];
	}
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
	std::cout << ans;
	return 0;
}