// g++ masterWorkersBarriers.cpp -o app -std=c++20
// ./app

#include <iostream>
#include <thread>
#include <atomic>
#include <barrier>
#include <vector>

using namespace std;

atomic_flag mt_flag{};
std::atomic<int> s{0};
const unsigned s_max{10};
const unsigned num_threads{4};

atomic_flag end_calculation{};

auto on_completion = []() noexcept
    {
        std::cout << "... step "<< s.load() <<" done! \n";
        mt_flag.clear();
        
        if (s.load() < s_max)
            s++;
        else
            end_calculation.test_and_set();
            
        std::cout << "end_calculation = "<< end_calculation.test() <<std::endl;
    };


std::barrier workDone(num_threads+1, on_completion);

void mainThreadFunc()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    while(!end_calculation.test())
    {
        mt_flag.test_and_set();
        printf("[MT] Step %d...\n", s.load());
        mt_flag.notify_all();
        
        printf("[MT] Step %d Working!\n", s.load());
        workDone.arrive_and_wait();
    }
    
    /*mt_flag.test_and_set();
    printf("[MT] Step 1...\n");
    mt_flag.notify_all();
    workDone.arrive_and_wait();
    
    mt_flag.test_and_set();
    printf("[MT] Step 2...\n");
    mt_flag.notify_all();
    workDone.arrive_and_wait();
    
    mt_flag.test_and_set();
    printf("[MT] Step 3...\n");
    mt_flag.notify_all();
    workDone.arrive_and_wait();*/
}

void calcThreadFunc(int tid)
{
    while(!end_calculation.test())
    {
        mt_flag.wait(false);

        if(s.load() > tid && s.load() <= tid + s_max - num_threads + 1)
            printf("[CT%d] Step %d Working!\n", tid, s.load());

        workDone.arrive_and_wait();
    }
    
    /*mt_flag.wait(false);
    printf("[CT%d] Step 1!\n", tid);
    workDone.arrive_and_wait();
    
    mt_flag.wait(false);
    printf("[CT%d] Step 2!\n", tid);
    workDone.arrive_and_wait();
    
    mt_flag.wait(false);
    printf("[CT%d] Step 3!\n", tid);
    workDone.arrive_and_wait();*/
}

int main()
{
    cout<<"Starting...\n";
    
    std::vector<std::thread> threads(num_threads+1);
    
    threads.push_back(std::thread(mainThreadFunc));
    //std::thread mt(mainThreadFunc);
    for(int i = 0; i < num_threads; i++)
        threads.push_back(std::thread(calcThreadFunc, i));
    /*std::thread ct1(calcThreadFunc, 1);
    std::thread ct2(calcThreadFunc, 2);
    std::thread ct3(calcThreadFunc, 3);
    std::thread ct4(calcThreadFunc, 4);     // */
    
    //mt.join();
    /*ct1.join();
    ct2.join();
    ct3.join();
    ct4.join();     // */
    
    for(auto& thread : threads)
        if(thread.joinable())
            thread.join();

    cout<<"*** All calculations completed! ***\n";
    return 0;
}