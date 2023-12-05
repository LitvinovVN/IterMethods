// g++ masterWorkers.cpp -o app
// ./app
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

const unsigned numWorkerThreads = 4;

std::mutex mutex;
bool startWorking = false;
bool isWorkersReady[numWorkerThreads];
std::mutex isWorkersReady_mutex;

void printIsWorkersReady()
{
    isWorkersReady_mutex.lock();
    printf("isWorkersReady: ");
    for(int i = 0; i < numWorkerThreads; i++)
        printf("%d ", isWorkersReady[i]);
    printf("\n");
    isWorkersReady_mutex.unlock();
}

// Ожидание команды начала работы
inline void waitingStartWorkingCommand()
{
    while (true)
    {
        bool isBreak = false;
        mutex.lock();
        if(startWorking) isBreak=true;
        mutex.unlock();

        if(isBreak) break;
    }
}

inline void setIsWorkerReady(int tid)
{
    isWorkersReady_mutex.lock();
    isWorkersReady[tid] = true;    
    isWorkersReady_mutex.unlock();
}

void workerFunc(int tid)
{
    printf("[worker %d] Started\n", tid);
    waitingStartWorkingCommand();
        
    printf("[worker %d] Working\n", tid);
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    setIsWorkerReady(tid);
    


    printf("[worker %d] Ended\n", tid);
}

inline void setStartWorking(bool value)
{
    mutex.lock();
    printf("[manager] Start calculation command: %d\n", value);
    startWorking = value;
    mutex.unlock();
}

inline void waitingAllWorkersReady()
{
    // Проверяем, что текущий этап работ выполнен всеми потоками-исполнителями
    while (true)
    {
        bool isBreak = true;
        isWorkersReady_mutex.lock();
        for(int i = 0; i < numWorkerThreads; i++)
            if(!isWorkersReady[i]) isBreak=false;
        isWorkersReady_mutex.unlock();

        if(isBreak) break;
    }
}

void managerFunc()
{
    printf("[manager] Started\n");
    printIsWorkersReady();
    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    setStartWorking(true);
    
    waitingAllWorkersReady();
    
    printf("[manager] Step 1 completed!\n");
    printIsWorkersReady();

    printf("[manager] All work completed!\n");
}

int main()
{
    clock_t start, end;
    double cpu_time_used;

    start = clock();
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

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cpu_time_used = %lf sec\n", cpu_time_used);

    return 0;
}