// Lesson 2: executor & thread-pool internals -- workers, work-stealing, and
// the difference between the calling (main) thread and a worker thread.
//
// Build & run:
//   clang++ -std=c++26 -I ../.. -pthread lesson2_demo.cpp -o lesson2_demo
//   ./lesson2_demo

#include <taskflow/taskflow.hpp>
#include <cstdio>
#include <mutex>
#include <vector>

int main() {

  const size_t num_workers = 3;
  tf::Executor executor(num_workers);
  tf::Taskflow taskflow;

  // main() is not a worker thread -- this always prints -1
  printf("this_worker_id() from main(): %d\n\n", executor.this_worker_id());

  std::mutex mutex;
  std::vector<int> per_worker_count(num_workers, 0);

  // 15 independent root tasks -- no dependencies among them
  for(int i = 0; i < 15; ++i) {
    taskflow.emplace([i, &executor, &mutex, &per_worker_count]() {
      int w = executor.this_worker_id();
      std::lock_guard lock(mutex);
      per_worker_count[w]++;
      printf("task %2d ran on worker %d\n", i, w);
    });
  }

  executor.run(taskflow).wait();

  printf("\nper-worker task counts:\n");
  for(size_t w = 0; w < num_workers; ++w) {
    printf("  worker %zu: %d tasks\n", w, per_worker_count[w]);
  }

  return 0;
}
