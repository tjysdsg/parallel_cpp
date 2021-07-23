#include <cstdio>
#include <random>
#include <vector>

#ifdef _DEBUG
#warning "Build this in release mode so that the microbenchmark output is accurate"
#endif

#include <benchmark/benchmark.h>

#define N 4

std::vector<int> initialize_random_array() {
  std::vector<int> ret(N, 0);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(-1E9, 1E9);

  for (int i = 0; i < N; ++i) { ret[i] = distrib(gen); }
  return ret;
}

void f(int *a, int *b) {
  for (int i = 0; i < N; ++i) { a[i] += b[i]; }
}

void f_simd(int *a, int *b) {
#pragma omp simd
  for (int i = 0; i < N; ++i) { a[i] += b[i]; }
}

static void BM_f(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto a = initialize_random_array();
    auto b = initialize_random_array();
    state.ResumeTiming();
    f(a.data(), b.data());
  }
}
BENCHMARK(BM_f);

static void BM_f_simd(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto a = initialize_random_array();
    auto b = initialize_random_array();
    state.ResumeTiming();
    f_simd(a.data(), b.data());
  }
}
BENCHMARK(BM_f_simd);

BENCHMARK_MAIN();
