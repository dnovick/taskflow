// Lesson 3: dynamic tasking with tf::Subflow -- building part of the graph
// at runtime, from inside a running task, via recursive divide-and-conquer.
//
// Build & run:
//   clang++ -std=c++26 -I ../.. -pthread lesson3_demo.cpp -o lesson3_demo
//   ./lesson3_demo

#include <taskflow/taskflow.hpp>
#include <cstdio>
#include <numeric>
#include <vector>

// Below this many elements, just sum sequentially -- spawning tasks for tiny
// ranges would cost more than it saves.
constexpr size_t THRESHOLD = 8;

// Recursively sums data[begin, end). Called from inside a subflow, so it can
// itself spawn a nested subflow when it recurses -- the graph's shape (how
// deep it recurses) depends on data.size(), which is only known at runtime.
long parallel_sum(tf::Subflow& sf, const std::vector<int>& data, size_t begin, size_t end) {

  size_t n = end - begin;
  if (n <= THRESHOLD) {
    return std::accumulate(data.begin() + begin, data.begin() + end, 0L);
  }

  size_t mid = begin + n / 2;
  long left_sum = 0, right_sum = 0;

  tf::Task left = sf.emplace([&](tf::Subflow& sf2) {
    left_sum = parallel_sum(sf2, data, begin, mid);
  }).name("left");

  tf::Task right = sf.emplace([&](tf::Subflow& sf2) {
    right_sum = parallel_sum(sf2, data, mid, end);
  }).name("right");

  // explicit join: wait right here for left/right (and everything THEY
  // spawned) to finish, so left_sum/right_sum are ready before we combine.
  sf.join();

  return left_sum + right_sum;
}

int main() {

  tf::Executor executor;
  tf::Taskflow taskflow;

  std::vector<int> data(1000);
  std::iota(data.begin(), data.end(), 1);   // 1, 2, 3, ..., 1000

  long total = 0;

  taskflow.emplace([&](tf::Subflow& sf) {
    total = parallel_sum(sf, data, 0, data.size());
  }).name("root");

  executor.run(taskflow).wait();

  long expected = std::accumulate(data.begin(), data.end(), 0L);
  printf("parallel_sum result: %ld (expected %ld) -> %s\n",
         total, expected, total == expected ? "OK" : "MISMATCH");

  return 0;
}
