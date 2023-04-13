#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <semaphore>
#include <thread>
#include <vector>

// void increment(int* a, std::mutex* mtx) {
//   std::lock_guard<std::mutex> lock(*mtx);
//   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//   *a = *a + 1;
// }

static class Tasker {
 private:
  static std::mutex taskQueueMutex;
  static std::queue<std::packaged_task<void()>> taskQueue;
  static int threadCount;

  static void execute() {
    std::packaged_task<void()> task;
    {
      std::lock_guard<std::mutex> lock(taskQueueMutex);
      if (taskQueue.empty()) {
        threadCount--;
        return;
      }
      task = std::move(taskQueue.front());
      taskQueue.pop();
    }
    task();
    task.get_future().get();
    threadCount--;
    std::cout << "\n"
              << "thread id " << std::this_thread::get_id() << "\n";
    std::cout << "thread count " << threadCount << "\n";
  }

 public:
  Tasker() {
  }

  static void enqueue(std::packaged_task<void()> task) {
    {
      std::lock_guard<std::mutex> lock(taskQueueMutex);
      taskQueue.push(std::move(task));
    }
  }

  static void update() {
    if (threadCount < 3 && !taskQueue.empty()) {
      {
        std::thread t(execute);
        t.detach();
        threadCount++;
      }
    }
  }
};

std::mutex Tasker::taskQueueMutex;
std::queue<std::packaged_task<void()>> Tasker::taskQueue;
int Tasker::threadCount = 0;

void coutFunction(std::string text) {
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << text << std::endl;
}

int return1(int a) {
  return a;
}

void input() {
  std::string input;
  while (std::cin.get()) {
    std::cout << "insert to queue"
              << "\n";

    std::packaged_task<void()> task = std::packaged_task<void()>(std::bind(coutFunction, "done"));
    Tasker::enqueue(std::move(task));
  }
}

void addToQueue(std::queue<std::function<void()>>* taskQueue, std::mutex* queueMtx) {
}

int main() {
  coutFunction("start");
  int n{0};
  std::thread t(coutFunction, "asdf");
  t.join();
  auto inputProcess = std::async(std::launch::async, input);
  std::packaged_task<int()> task = std::packaged_task<int()>(std::bind(return1, 2));
  std::packaged_task<int()> task2 = std::packaged_task<int()>(std::bind(return1, 3));
  task();
  task2();
  std::cout << task.get_future().get();
  std::cout << task2.get_future().get();
  while (true) {
    // std::this_thread::sleep_for(std::chrono::milliseconds(20));
    Tasker::update();
  }
  return 0;
}