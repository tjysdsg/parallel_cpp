#include <atomic>
#include <cstdio>

int main() {
  int index; /// index in the loop
  std::atomic<int> team = 0;
#pragma omp parallel num_threads(3) default(none) shared(team, index) /// team is shared across 3 threads
  {
    /// increment-and-get, this is atomic https://en.cppreference.com/w/cpp/atomic/atomic/operator_arith
    int t = ++team;

#pragma omp for lastprivate(index) /// the latest value of index is kept as the value of index in the outer scope
    for (int i = 0; i < 18; ++i) {
      index = i;
      printf("team: %d; loop index: %d\n", t, i);
    }
  }
  printf("last finished team: %d, index: %d\n", team.load(), index);
  return 0;
}
