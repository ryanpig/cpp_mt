#include <thread>
#include <iostream>
#include <chrono>

using namespace std;

void func(int n)
{
  for(int i = 0; i < n ; i++){
    cout << "(child)thread id:" << this_thread::get_id() << ", value:" << i << endl;
    this_thread::sleep_for(chrono::milliseconds(100));
    
  }

}

int main()
{

  thread t(func, 20);

  for(int i = 0; i < 10 ; i++){
    cout << "(parent)thread id:" << this_thread::get_id() << ", value:" << i << endl;
    this_thread::sleep_for(chrono::milliseconds(70));

  }

  t.join();
}
