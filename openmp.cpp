#include <atomic>
#include <cstdio>

int main() {
  int index;
  std::atomic<int> team = 0;
#pragma omp parallel num_threads(3) shared(team)
  {
    int t = ++team;
#pragma omp for lastprivate(index)
    for (int i = 0; i < 18; ++i) {
      index = i;
      printf("team: %d; loop index: %d\n", t, i);
    }
  }
  printf("last finished team: %d, index: %d\n", team.load(), index);
  return 0;
}
