#include <taskflow/taskflow.hpp>
#include <cstdio>
#include <numeric>
#include <vector>


constexpr size_t THRESHOLD = 10;

static auto fibonacci_manual(uint32_t n) -> uint64_t {

}
static auto fibonacci_tf(tf::Subflow& sf, uint32_t n) -> uint64_t {


}
static auto fibonacci(tf::Subflow& sf, uint32_t n) -> uint64_t{
    if (n <= THRESHOLD) {
        return fibonacci_manual(n);
    }

    return fibonacci_tf(sf, n);
}

auto  main() -> int {



    return 0;
}
