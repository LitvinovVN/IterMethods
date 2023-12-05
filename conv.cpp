#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>

using namespace std::literals;

const unsigned numThreads{4};
const unsigned Nbx{10};// Количество блоков сетки по Ох
const unsigned numSteps{Nbx+numThreads};

// global semaphore instances
// object counts are set to zero
// objects are in non-signaled state
std::counting_semaphore<numThreads>
    smphSignalMainToThread{0},
    smphSignalThreadToMain{0};
 
void ThreadProc(int thread_id)
{
    for(int i=0; i<numSteps;i++)
    {
        // wait for a signal from the main proc
        // by attempting to decrement the semaphore
        smphSignalMainToThread.acquire();
     
        // this call blocks until the semaphore's count
        // is increased from the main proc
        if(i-thread_id>=0 && i-thread_id<=Nbx)
        {
            //printf("[thread %d] %i Calculation starting... i-thread_id = %d\n", thread_id, i, i-thread_id); // response message
         
            // wait to imitate some work
            std::this_thread::sleep_for(10ms);
         
            printf("[thread %d] %i Calculation Completed! i-thread_id = %d\n", thread_id, i, i-thread_id); // response message
        }
        else
        {
            //printf("[thread %d] %i waiting... i-thread_id = %d\n", thread_id, i, i-thread_id); // response message
            std::this_thread::sleep_for(1ms);
        }
     
        // signal the main proc back
        smphSignalThreadToMain.release();
    }
}

void signalToWorkers()
{
    // signal the worker threads to start working
    // by increasing the semaphore's count
    smphSignalMainToThread.release(numThreads);
}

void waitingSygnalFromWorkers()
{
    // wait until the worker threads is done doing the work
    // by attempting to decrement the semaphore's count
    for(int i=0; i<numThreads;i++)
        smphSignalThreadToMain.acquire();
        
    std::cout << "[main] Got the signals from all workers\n"; // response message
}
 
int main()
{
    std::vector<std::jthread> threads;
    threads.reserve(numThreads);
 
    for (auto id{0U}; id != numThreads; ++id)
        threads.push_back(std::jthread(ThreadProc, id));
    
    
    for(int s = 0; s < numSteps; s++)
    {
        std::cout << "[main] Step " << s <<"\n"; // message
        signalToWorkers();
        waitingSygnalFromWorkers();
    }
    
    
    std::cout << "[main] The end\n";
}