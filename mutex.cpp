#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <mutex>

// shared resource: output stream between func1, func2, and main;
// expected result w/o mutex: the ouput print would be a mess due to race condition. 
// expected result w/ mutex & lock_guard: the output print is neat and it's thread safe now. 
using namespace std;

mutex mtx;

void func1(int n)
{
  lock_guard<mutex> lck(mtx);
  for(int i = 0; i < n ; i++){
    cout << "(child)thread id:" << this_thread::get_id() << ", value:" << i << endl;
  }
}

void func2(int n)
{
  lock_guard<mutex> lck(mtx);
  for(int i = 0; i < n ; i++){
    cout << "(child)thread id:" << this_thread::get_id() << ", value:" << i << endl;
  }
}

int main()
{
  vector<thread> threads;
  for(int i = 0; i < 20; i++)
  {
    if(i % 2 == 0)
      threads.push_back(thread(func1, 20));
    else
      threads.push_back(thread(func2, 20));
  }

  for(int i = 0; i < 10 ; i++){
    lock_guard<mutex> lck(mtx);
    cout << "(parent)thread id:" << this_thread::get_id() << ", value:" << i << endl;
    // this_thread::sleep_for(chrono::milliseconds(70));
  }

  for(auto &t : threads)
    t.join();
}
