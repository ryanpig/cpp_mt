/* Goal: using condition_variable to control the thread execution sequence
* expected result: Producer can continuously generate stuffs and put them into the deque, while Consumer will retrieve them later on
* Role:
* - shared resource: deque
* - sequence control: condition_variable
* - access control: unique_lock
*/
#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <condition_variable>
#include <deque>
#include <mutex>

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
      int count = 10;
      while(count !=0)
      {
        unique_lock<mutex> lck(mtx);
        while(m_deque.size() >= 100){
          cv2.wait(lck);
        }
        for(int i = 0; i < 10; i++)
        {
          m_deque.push_back(i);   
          cout << "(producer)" << "-(id " << id << ")" << "size:" << m_deque.size() << endl;
        } 
        lck.unlock();
        this_thread::sleep_for(chrono::milliseconds(300));
        // cv.notify_one();
        cv.notify_all();
      }
    }

    void consumer(int id)
    {
      int data = 0;
      while(data == 0)
      {
        unique_lock<mutex> lck(mtx);
        while(m_deque.size() <= 2){
          cv.wait(lck);
        }
        for(int i=0;i<5;i++)
        {
          cout << "(consumer)" << "-(id " << id << ")" << "pop out:" << m_deque.front() << "-" << i << endl;
          m_deque.pop_front();   
        }
        lck.unlock();
        this_thread::sleep_for(chrono::milliseconds(100));
        // cv2.notify_one();
        cv2.notify_all();
      }
    }
};

int main()
{
  Factory f;
  thread p1(&Factory::producer, &f, 1);
  thread p2(&Factory::producer, &f, 2);
  thread c1(&Factory::consumer, &f, 1 );
  thread c2(&Factory::consumer, &f, 2 );
  thread c3(&Factory::consumer, &f, 3 );
  thread c4(&Factory::consumer, &f, 4 );

  p1.join();
  p2.join();
  c1.join();
  c2.join();
  c3.join();
  c4.join();

  
}
