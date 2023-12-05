// g++ masterWorkers.cpp -o app
// ./app
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

const unsigned numWorkerThreads = 3;

std::mutex mutex;

bool startWorking = false;

void workerFunc(int tid)
{
    printf("[worker %d] Started\n", tid);

    while (true)
    {
        bool isBreak = false;
        mutex.lock();
        if(startWorking) isBreak=true;
        mutex.unlock();

        if(isBreak) break;
    }
    
    printf("[worker %d] Working\n", tid);
    

    printf("[worker %d] Ended\n", tid);
}

void managerFunc()
{
    printf("[manager] Started\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    mutex.lock();
    printf("[manager] Start calculation command\n");
    startWorking=true;
    mutex.unlock();
    // Проверяем, что текущий этап работ выполнен всеми потоками-исполнителями



    printf("[manager] Ended\n");
}

int main()
{
    printf("Starting working threads\n");
    std::vector<std::thread> threads;

    for(int i = 0; i < numWorkerThreads; i++)
        threads.push_back(std::thread(workerFunc,i));

    threads.push_back(std::thread(managerFunc));

    

    for(auto& thread : threads)
    {
        if(thread.joinable())
            thread.join();
    }

    return 0;
}