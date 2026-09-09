// Lesson 1: core mental model -- Taskflow (graph) vs Executor (thread pool)
// vs Task (handle). Run this a few times; watch which of B/C prints first.
//
// Build & run:
//   clang++ -std=c++26 -I ../.. -pthread lesson1_demo.cpp -o lesson1_demo
//   ./lesson1_demo
//
// Visualize the graph:
//   dot -Tpng graph.dot -o graph.png && open graph.png

#include <taskflow/taskflow.hpp>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <thread>

void pretend_work(const char* name, int ms) {
  printf("%-6s start\n", name);
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  printf("%-6s done\n", name);
}

int main() {

  tf::Executor executor;   // spawns worker threads once, here
  tf::Taskflow taskflow;   // pure graph description, no threads involved

  auto A = taskflow.emplace([](){ pretend_work("A", 50); }).name("A");
  auto B = taskflow.emplace([](){ pretend_work("B", 100); }).name("B");
  auto C = taskflow.emplace([](){ pretend_work("C", 100); }).name("C");
  auto D = taskflow.emplace([](){ pretend_work("D", 20); }).name("D");

  // diamond: A -> {B, C} -> D
  A.precede(B, C);
  D.succeed(B, C);

  // dump the graph structure as DOT to its own file, kept separate from the
  // task-progress prints below so it can be fed straight to graphviz
  std::ofstream dot_file("graph.dot");
  taskflow.dump(dot_file);

  executor.run(taskflow).wait();

  return 0;
}
