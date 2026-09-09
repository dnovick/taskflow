#include <taskflow/taskflow.hpp>
#include <cstdio>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>

int main() {
    static constexpr size_t num_workers = 4; //std::thread::hardware_concurrency();
    static constexpr size_t num_tasks = 40;
    static constexpr size_t wait_ms = 5;

    tf::Executor executor(num_workers);

    tf::Taskflow taskflow;


    std::mutex mutex;
    std::vector<int> per_worker_count(num_workers, 0);

    auto R = taskflow.emplace([]() {}).name("R"); // does nothing, just triggers fan-out

    for (auto i = 0U; i < num_tasks; ++i) {
        auto child = taskflow.emplace([i, &executor, &mutex, &per_worker_count]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
            const int w = executor.this_worker_id();
            std::lock_guard lock(mutex);
            per_worker_count[w]++;
            printf("task %2d ran on worker %d\n", i, w);
        });
        R.precede(child); // or: child.succeed(R);
    }
    auto start = std::chrono::steady_clock::now();
    executor.run(taskflow).wait();
    auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();

    printf("\n\nelapsed: %.2f ms\n", elapsed);

    printf("\nper-worker task counts:\n");
    for (size_t w = 0; w < num_workers; ++w) {
        printf("  worker %zu: %d tasks\n", w, per_worker_count[w]);
    }
}
