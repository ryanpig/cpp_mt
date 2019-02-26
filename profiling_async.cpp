/* 
 * Goal: profiling in a multithreading env. 
*/
#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <deque>
#include <mutex>
#include "iprof/iprof.hpp"
#include <future>

using namespace std;
class Factory
{
  private:
    std::condition_variable cv, cv2;
    std::mutex mtx;
    std::deque<int> m_deque; 
  public:
    void producer(int id)
    {
      IPROF_FUNC;
      {
        bool leave = false;
        while(!leave)
        {
          unique_lock<mutex> lck(mtx);
          if(m_deque.size() >= 20){
            cout << "no need to produce new stuffs" << endl;
            lck.unlock();
            leave = true;
          }else
          {
            for(int i = 0; i < 5; i++)
            {
              m_deque.push_back(i);   
              cout << "(producer)" << "-(id " << id << ")" << "size:" << m_deque.size() << endl;
            } 
            lck.unlock();
            this_thread::sleep_for(chrono::milliseconds(10));
          }
        }
      }
    }

    void consumer(int id)
    {
      IPROF_FUNC;
      {
        bool leave = false;
        while(!leave)
        {
          unique_lock<mutex> lck(mtx);
          if(m_deque.size() == 0){
            leave = true;
          }else
          {
            for(int i=0;i<1;i++)
            {
              cout << "(consumer)" << "-(id " << id << ")" << "pop out:" << m_deque.front() << "-" << i << endl;
              m_deque.pop_front();   
            }
            lck.unlock();
          this_thread::sleep_for(chrono::milliseconds(10));
          }
        }
        this_thread::sleep_for(chrono::milliseconds(500));
      }
    }
};

int main()
{
  auto startTime = HighResClock::now();
  cout << "sizeof(InternalProfiler::Tree): " << sizeof(InternalProfiler::Tree) << " bytes" << endl;


  // profiling in a multithreading
  auto load = [](int i)
  {
    Factory f;
    f.producer(i);
    f.consumer(i);
    return true;
  };
  std::future<bool> ret = std::async(std::launch::deferred, load, 1);
  std::future<bool> ret2 = std::async(std::launch::deferred,load,  3);
  Factory f;
  f.producer(2);
  f.consumer(2);
  ret.get();
  ret2.get();
  InternalProfiler::aggregateEntries();
  InternalProfiler::addEntriesToAllThreadStats();
cout << "\nThe profiler all thread stats:\n" << InternalProfiler::allThreadStats << endl;
  
}
