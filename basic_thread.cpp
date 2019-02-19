/* Goal: familar w/ different ways to create thread
 * Expect outcome: the issue comes from the race of shared resource (ofstream)
 */
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

class Obj
{
  public:
    void operator()(int n)
    {
      for(int i = 0; i < n ; i++){
        cout << "(obj func)thread id:" << this_thread::get_id() << ", value:" << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
      }
    }
};

int main()
{
  // call thread constructor w/ function name
  thread t1(func, 20);

  // call thread constructor w/ lambda function 
  thread t2([](int n){
    cout << "(lambda)thread id:" << this_thread::get_id() << ", value:" << n << endl;
    // this_thread::sleep_for(chrono::milliseconds(100));
    }, 20);

  // call thread constructor w/ member function of object 
  thread t3(Obj(), 20); 
  
  // from main thread
  for(int i = 0; i < 10 ; i++){
    cout << "(parent)thread id:" << this_thread::get_id() << ", value:" << i << endl;
    // this_thread::sleep_for(chrono::milliseconds(70));
  }

  if(t1.joinable())
    t1.join();
  if(t2.joinable())
    t2.join();
  if(t3.joinable())
    t3.join();
}
