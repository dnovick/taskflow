// Demonstrates tf::TaskPriority scheduling.
//
// Build with priority scheduling enabled:
//   clang++ -std=c++26 -I ../.. -pthread -DTF_ENABLE_TASK_PRIORITY \
//     priority_demo.cpp -o priority_demo_on
//
// Build with priority scheduling disabled (priority calls become no-ops):
//   clang++ -std=c++26 -I ../.. -pthread priority_demo.cpp -o priority_demo_off
//
// Root tasks with no predecessors are dispatched from a shared, non-priority
// buffer (they're launched by the calling thread, not a worker), so priority
// has no effect on them. Priority only kicks in when a task finishes *inside*
// a worker thread and several of its successors become ready at once -- the
// worker pushes them onto its own priority-partitioned queue. So this test
// uses one root task with several children of mixed priority to exercise
// that path.
//
// A single worker is used so execution order is fully deterministic.

#include <taskflow/taskflow.hpp>
#include <cstdio>
#include <vector>

int main() {

  tf::Executor executor(1);  // one worker -> deterministic order
  tf::Taskflow taskflow;

  tf::Task root = taskflow.emplace([]() {
    printf("running root\n");
  }).name("root");

  struct Spec { const char* name; tf::TaskPriority prio; };

  // Children of root, added in this order. They all become ready at the
  // same instant (when root finishes), so this is exactly the scenario
  // priority scheduling is meant to affect.
  std::vector<Spec> specs = {
    {"low-1",  tf::TaskPriority::LOW},
    {"norm-1", tf::TaskPriority::NORMAL},
    {"high-1", tf::TaskPriority::HIGH},
    {"low-2",  tf::TaskPriority::LOW},
    {"high-2", tf::TaskPriority::HIGH},
    {"norm-2", tf::TaskPriority::NORMAL},
  };

  for(auto& s : specs) {
    auto t = taskflow.emplace([name = s.name]() {
      printf("running %s\n", name);
    }).name(s.name).priority(s.prio);
    root.precede(t);
  }

  executor.run(taskflow).wait();

  return 0;
}
