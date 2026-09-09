//
// Created by David Novick on 9/8/26.
//
/*
//Build a small "build pipeline" graph with these dependencies:

compile_a ─┐
           ├─> link ─┬─> test ─┐
compile_b ─┘         └─> package ─┴─> deploy

- compile_a and compile_b have no dependencies — they should be able to run concurrently.
- link depends on both compiles.
- test and package both depend on link, but not on each other — they should be able to run concurrently.
- deploy depends on both test and package.

Each task should print "<name> start" and "<name> done" (use a small sleep_for in between, like the demo, to make interleaving visible). Run it several times and confirm:

1. link never starts before both compiles print done.
2. deploy never starts before both test and package print done.
3. test/package (and separately compile_a/compile_b) show interleaved start/done — proving they actually ran concurrently, not just "in some order."
*/

#include <taskflow/taskflow.hpp>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <thread>

inline std::mutex log_mutex;

inline constexpr size_t sleep_ms = 100;

static auto log_task_start(const std::string &task_name) -> void {
    std::lock_guard<std::mutex> lg(log_mutex);

    std::cout << task_name << " start\n";
}

static auto log_task_end(const std::string &task_name) -> void {
    std::lock_guard<std::mutex> lg(log_mutex);

    std::cout << task_name << " end\n";
}

static auto link() -> void {
    log_task_start("link");

    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

    log_task_end("link");
}

static auto compile_a() -> void {
    log_task_start("compile_a");

    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

    log_task_end("compile_a");
}

static auto compile_b() -> void {
    log_task_start("compile_b");

    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

    log_task_end("compile_b");
}

static auto test() -> void {
    log_task_start("test");

    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

    log_task_end("test");

}

static auto package() -> void {
    log_task_start("package");

    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

    log_task_end("package");

}

static auto deploy() -> void {
    log_task_start("deploy");

    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

    log_task_end("deploy");

}

auto main() -> int {

    tf::Executor executor;
    tf::Taskflow taskflow;


    auto taskLink = taskflow.emplace(link).name("link");
    auto taskCompileA = taskflow.emplace(compile_a).name("compile_a");
    auto taskCompileB = taskflow.emplace(compile_b).name("compile_b");
    auto taskTest = taskflow.emplace(test).name("test");
    auto taskPackage = taskflow.emplace(package).name("package");
    auto taskDeploy = taskflow.emplace(deploy).name("deploy");

    taskCompileA.precede(taskLink);
    taskCompileB.precede(taskLink);

    taskTest.succeed(taskLink);
    taskPackage.succeed(taskLink);

    taskDeploy.succeed(taskTest);
    taskDeploy.succeed(taskPackage);


    std::ofstream dot_file("exercise1.dot");
    taskflow.dump(dot_file);

    executor.run(taskflow).wait();

    return 0;
}
