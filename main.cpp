#include <iostream>
#include <thread>

using std::cout;
using std::thread;

int global = 10;

void worker() { global = 0; }

int *stack_addr = nullptr;

void stack_holder() {
  int local = 1024;
  stack_addr = &local;
  cout << "local: " << local << '\n';
  std::this_thread::sleep_for(std::chrono::seconds(1));
  cout << "local now: " << local << '\n';
}

int main() {
  { /// thread can shared memory of the main process
    /// stack of main
    int a = 999;
    auto p1 = thread([&]() { a -= 1; });
    p1.join();

    /// global variable
    auto p2 = thread(worker);
    p2.join();

    cout << a << '\n';
    cout << global << '\n';
  }

  { /// a thread can modify the stack of another thread
    auto p1 = thread(stack_holder);
    auto p2 = thread([&]() { *stack_addr = 2048; });
    p1.join();
    p2.join();
  }

  return 0;
}
