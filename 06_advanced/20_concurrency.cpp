/*
================================================================================
  LESSON 20: Concurrency — Threads, Mutexes, and Async
================================================================================

  THEORY:
  -------
  CONCURRENCY: multiple tasks making progress (may not be simultaneous)
  PARALLELISM: multiple tasks running at the SAME TIME (requires multiple cores)

  WHY CONCURRENCY?
  ----------------
  1. Performance: use multiple CPU cores
  2. Responsiveness: UI stays responsive while background work runs
  3. I/O overlap: while waiting for disk/network, do other work

  THREAD:
  -------
  A thread is an independent execution path within a process.
  All threads in a process share the same memory (heap, globals).
  Each thread has its own stack.

  RACE CONDITION:
  ---------------
  When two threads access shared data and at least one writes,
  the result depends on the ORDER of execution — undefined behavior!

  Example:
  Thread 1: reads counter (value=5)
  Thread 2: reads counter (value=5)
  Thread 1: writes counter = 5+1 = 6
  Thread 2: writes counter = 5+1 = 6  ← LOST UPDATE! Should be 7.

  SYNCHRONIZATION:
  ----------------
  mutex:       mutual exclusion — only one thread at a time
  lock_guard:  RAII mutex lock (auto-unlocks when destroyed)
  unique_lock: more flexible than lock_guard
  atomic:      lock-free thread-safe operations on simple types
  condition_variable: wait for a condition to become true

  COMPILE:
  --------
  g++ -std=c++17 20_concurrency.cpp -o concurrency -lpthread

================================================================================
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <future>
#include <vector>
#include <queue>
#include <chrono>
#include <string>
#include <functional>

// =========================================================================
// HELPERS
// =========================================================================

void sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

std::mutex cout_mutex;  // protect cout from interleaved output
void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "[Thread " << std::this_thread::get_id() << "] " << msg << std::endl;
}

// =========================================================================
// SECTION 1: BASIC THREADS
// =========================================================================

void worker_function(int id, int iterations) {
    for (int i = 0; i < iterations; i++) {
        safe_print("Worker " + std::to_string(id) + " iteration " + std::to_string(i));
        sleep_ms(10);
    }
}

void demo_basic_threads() {
    std::cout << "\n=== BASIC THREADS ===" << std::endl;

    // Create threads:
    std::thread t1(worker_function, 1, 3);
    std::thread t2(worker_function, 2, 3);

    // join(): wait for thread to finish (MUST call join or detach!)
    t1.join();
    t2.join();
    std::cout << "Both threads finished." << std::endl;

    // Thread with lambda:
    std::thread t3([]() {
        safe_print("Lambda thread running!");
        sleep_ms(50);
        safe_print("Lambda thread done.");
    });
    t3.join();

    // detach(): let thread run independently (fire and forget)
    // WARNING: detached thread must not access destroyed objects!
    std::thread t4([]() {
        sleep_ms(100);
        // safe_print("Detached thread done.");  // might crash if main exits first
    });
    t4.detach();  // t4 runs independently now

    // Thread with reference argument (must use std::ref):
    int result = 0;
    std::thread t5([&result]() {
        result = 42;
    });
    t5.join();
    std::cout << "Result from thread: " << result << std::endl;

    // Hardware concurrency:
    std::cout << "Hardware threads: " << std::thread::hardware_concurrency() << std::endl;
}

// =========================================================================
// SECTION 2: RACE CONDITION AND MUTEX
// =========================================================================

int unsafe_counter = 0;
int safe_counter = 0;
std::mutex counter_mutex;

void increment_unsafe(int times) {
    for (int i = 0; i < times; i++) {
        unsafe_counter++;  // RACE CONDITION! Not atomic.
    }
}

void increment_safe(int times) {
    for (int i = 0; i < times; i++) {
        std::lock_guard<std::mutex> lock(counter_mutex);  // auto-unlocks when destroyed
        safe_counter++;  // protected by mutex
    }
}

void demo_mutex() {
    std::cout << "\n=== MUTEX ===" << std::endl;

    // Race condition demo:
    unsafe_counter = 0;
    std::thread t1(increment_unsafe, 10000);
    std::thread t2(increment_unsafe, 10000);
    t1.join(); t2.join();
    std::cout << "Unsafe counter (expected 20000): " << unsafe_counter << std::endl;
    // Often NOT 20000 due to race condition!

    // Mutex-protected:
    safe_counter = 0;
    std::thread t3(increment_safe, 10000);
    std::thread t4(increment_safe, 10000);
    t3.join(); t4.join();
    std::cout << "Safe counter (expected 20000):   " << safe_counter << std::endl;
    // Always 20000!

    // unique_lock — more flexible than lock_guard:
    std::mutex m;
    {
        std::unique_lock<std::mutex> lock(m);
        // Can unlock early:
        lock.unlock();
        // Do non-critical work...
        lock.lock();  // re-lock
        // Critical section again
    }  // auto-unlocks here

    // try_lock — non-blocking:
    std::mutex try_m;
    if (try_m.try_lock()) {
        std::cout << "Got the lock!" << std::endl;
        try_m.unlock();
    } else {
        std::cout << "Lock not available." << std::endl;
    }
}

// =========================================================================
// SECTION 3: ATOMIC
// =========================================================================

std::atomic<int> atomic_counter{0};
std::atomic<bool> stop_flag{false};

void demo_atomic() {
    std::cout << "\n=== ATOMIC ===" << std::endl;

    // Atomic operations are lock-free and thread-safe:
    atomic_counter = 0;
    std::thread t1([](){ for(int i=0;i<10000;i++) atomic_counter++; });
    std::thread t2([](){ for(int i=0;i<10000;i++) atomic_counter++; });
    t1.join(); t2.join();
    std::cout << "Atomic counter (expected 20000): " << atomic_counter << std::endl;

    // Atomic operations:
    std::atomic<int> val{10};
    val.store(20);                          // write
    int old = val.load();                   // read
    int prev = val.exchange(30);            // swap, returns old value
    bool success = val.compare_exchange_strong(old, 40);  // CAS operation

    std::cout << "val after operations: " << val << std::endl;

    // Atomic flag (simplest atomic):
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    bool was_set = flag.test_and_set();  // atomically set and return old value
    std::cout << "Flag was set: " << was_set << std::endl;
    flag.clear();
}

// =========================================================================
// SECTION 4: CONDITION VARIABLE — producer/consumer
// =========================================================================

std::queue<int> work_queue;
std::mutex queue_mutex;
std::condition_variable queue_cv;
bool producer_done = false;

void producer(int items) {
    for (int i = 0; i < items; i++) {
        sleep_ms(20);
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            work_queue.push(i);
            safe_print("Produced: " + std::to_string(i));
        }
        queue_cv.notify_one();  // wake up one waiting consumer
    }
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        producer_done = true;
    }
    queue_cv.notify_all();  // wake up all consumers
}

void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        // wait() releases lock and blocks until notified, then re-acquires lock
        queue_cv.wait(lock, []() {
            return !work_queue.empty() || producer_done;
        });

        if (work_queue.empty() && producer_done) break;

        int item = work_queue.front();
        work_queue.pop();
        lock.unlock();  // release lock before processing

        safe_print("Consumer " + std::to_string(id) + " processed: " + std::to_string(item));
        sleep_ms(30);
    }
    safe_print("Consumer " + std::to_string(id) + " done.");
}

void demo_condition_variable() {
    std::cout << "\n=== CONDITION VARIABLE (Producer-Consumer) ===" << std::endl;

    producer_done = false;
    while (!work_queue.empty()) work_queue.pop();

    std::thread prod(producer, 5);
    std::thread cons1(consumer, 1);
    std::thread cons2(consumer, 2);

    prod.join();
    cons1.join();
    cons2.join();
}

// =========================================================================
// SECTION 5: FUTURES AND ASYNC
// =========================================================================

int heavy_computation(int n) {
    sleep_ms(100);  // simulate work
    int sum = 0;
    for (int i = 1; i <= n; i++) sum += i;
    return sum;
}

std::string fetch_data(const std::string& url) {
    sleep_ms(150);  // simulate network request
    return "Data from " + url;
}

void demo_async() {
    std::cout << "\n=== ASYNC / FUTURE ===" << std::endl;

    // std::async: run function asynchronously, get result via future
    auto f1 = std::async(std::launch::async, heavy_computation, 100);
    auto f2 = std::async(std::launch::async, heavy_computation, 200);
    auto f3 = std::async(std::launch::async, fetch_data, "https://api.example.com");

    std::cout << "Tasks launched, doing other work..." << std::endl;
    sleep_ms(50);
    std::cout << "Still doing other work..." << std::endl;

    // get() blocks until result is ready:
    std::cout << "Sum 1-100: " << f1.get() << std::endl;
    std::cout << "Sum 1-200: " << f2.get() << std::endl;
    std::cout << f3.get() << std::endl;

    // std::promise / std::future — manual future:
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread setter([&promise]() {
        sleep_ms(50);
        promise.set_value(42);  // fulfill the promise
    });

    std::cout << "Waiting for promise..." << std::endl;
    std::cout << "Promise value: " << future.get() << std::endl;
    setter.join();

    // std::packaged_task — wrap a callable for async execution:
    std::packaged_task<int(int, int)> task([](int a, int b) { return a + b; });
    std::future<int> task_future = task.get_future();

    std::thread task_thread(std::move(task), 10, 20);
    std::cout << "Packaged task result: " << task_future.get() << std::endl;
    task_thread.join();
}

// =========================================================================
// SECTION 6: THREAD POOL PATTERN
// =========================================================================

class ThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex task_mutex;
    std::condition_variable task_cv;
    bool stop = false;

public:
    ThreadPool(int num_threads) {
        for (int i = 0; i < num_threads; i++) {
            workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(task_mutex);
                        task_cv.wait(lock, [this]() { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    void submit(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(task_mutex);
            tasks.push(std::move(task));
        }
        task_cv.notify_one();
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(task_mutex);
            stop = true;
        }
        task_cv.notify_all();
        for (auto& w : workers) w.join();
    }
};

void demo_thread_pool() {
    std::cout << "\n=== THREAD POOL ===" << std::endl;

    ThreadPool pool(3);  // 3 worker threads

    for (int i = 0; i < 9; i++) {
        pool.submit([i]() {
            safe_print("Task " + std::to_string(i) + " running");
            sleep_ms(30);
            safe_print("Task " + std::to_string(i) + " done");
        });
    }

    sleep_ms(500);  // wait for tasks to complete
    std::cout << "All tasks submitted." << std::endl;
}

int main() {
    demo_basic_threads();
    demo_mutex();
    demo_atomic();
    demo_condition_variable();
    demo_async();
    demo_thread_pool();

    std::cout << "\n=== CONCURRENCY SUMMARY ===" << std::endl;
    std::cout << "std::thread          — create and manage threads" << std::endl;
    std::cout << "std::mutex           — mutual exclusion" << std::endl;
    std::cout << "std::lock_guard      — RAII mutex lock" << std::endl;
    std::cout << "std::atomic<T>       — lock-free thread-safe operations" << std::endl;
    std::cout << "std::condition_variable — wait/notify between threads" << std::endl;
    std::cout << "std::async/future    — run tasks asynchronously" << std::endl;
    std::cout << "std::promise         — manually set future value" << std::endl;

    return 0;
}
