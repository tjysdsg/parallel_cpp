/**
 * \brief An ABA problem example adapted from
 * https://en.wikipedia.org/wiki/ABA_problem
 */
#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

class Node {
public:
  Node(std::string val, Node *next) : val(std::move(val)), next(next) {}
  std::string val;
  Node *next = nullptr;
};

std::ostream &operator<<(std::ostream &os, Node *node) {
  os << node->val;
  if (node->next) {
    os << " -> " << node->next;
  }
  return os;
}

class Stack {
public:
  std::atomic<Node *> top_ptr;

  Node *pop() {
    while (true) {
      Node *ret_ptr = top_ptr;
      if (!ret_ptr)
        return nullptr;
      Node *next_ptr = ret_ptr->next;
      if (top_ptr.compare_exchange_weak(ret_ptr, next_ptr)) {
        return ret_ptr;
      }
    }
  }

  Node *pop_sleep() {
    while (true) {
      Node *ret_ptr = top_ptr;
      if (!ret_ptr)
        return nullptr;
      Node *next_ptr = ret_ptr->next;

      std::this_thread::sleep_for(std::chrono::seconds(1));
      if (top_ptr.compare_exchange_weak(ret_ptr, next_ptr)) {
        return ret_ptr;
      }
    }
  }

  void push(Node *obj_ptr) {
    while (true) {
      Node *next_ptr = top_ptr;
      obj_ptr->next = next_ptr;
      if (top_ptr.compare_exchange_weak(next_ptr, obj_ptr)) {
        return;
      }
    }
  }
};

Stack g_stack;
Node *C;
Node *B;
Node *A;

void thread1() {
  g_stack.pop_sleep();
  Node *top = g_stack.top_ptr;
  if (top->val == "Invalid B") {
    std::cout << "ABA bug occurred. At the end of Thread 1, the stack is: "
              << g_stack.top_ptr.load() << "\n";
  }
}

void thread2() {
  /// stack: top A B C
  g_stack.pop(); /// stack: top B C
  g_stack.pop(); /// stack: top C
  A->next = C;
  g_stack.push(A); /// stack: top A C

  std::cout << "Thread 2 finished, the stack is: " << g_stack.top_ptr.load()
            << '\n';
  B->val = "Invalid B";
}

int main() {
  C = new Node("C", nullptr);
  B = new Node("B", C);
  A = new Node("A", B);
  g_stack.push(C);
  g_stack.push(B);
  g_stack.push(A);

  std::cout << "The stack is originally: " << g_stack.top_ptr.load() << '\n';
  auto p1 = std::thread(thread1);
  auto p2 = std::thread(thread2);

  p1.join();
  p2.join();

  delete A;
  delete B;
  delete C;

  return 0;
}
