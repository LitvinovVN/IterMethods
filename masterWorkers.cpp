// g++ masterWorkers.cpp -o app
// ./app
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

const unsigned numWorkerThreads = 4;
const unsigned numSteps = 2;

std::mutex mutex;
bool startWorking = false;
bool isWorkersReady[numWorkerThreads];
int workersCurrentS[numWorkerThreads];// Массив текущих обрабатываемых шагов
std::mutex isWorkersReady_mutex;

std::atomic_int s{0};

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

inline void setIsWorkerReady(int tid, bool isReady)
{
    isWorkersReady_mutex.lock();
    isWorkersReady[tid] = isReady;    
    isWorkersReady_mutex.unlock();
}

inline void setIsWorkerReadyAll(bool isReady)
{
    isWorkersReady_mutex.lock();
    for(int i=0;i<numWorkerThreads; i++)
        isWorkersReady[i] = isReady;
    isWorkersReady_mutex.unlock();
}

// Функция имитации вычислений одним потоом
void calculationFunc(int tid, int s)
{
    printf("[worker %d] s=%d Working...\n", tid, s);
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

inline void waitingWorkersCurrentS(int tid)
{
    while (true)
    {
        bool isBreak = false;
        mutex.lock();
        int cur_thread_s = workersCurrentS[tid];
        int cur_s = s.load();
        if(cur_s>cur_thread_s) isBreak=true;
        mutex.unlock();

        if(isBreak) break;
    }
}

void workerFunc(int tid)
{
    setIsWorkerReady(tid, true);
    printf("[worker %d] Started and ready!\n", tid);

    int cur_s;
    do
    {
        waitingStartWorkingCommand();
        // Если текущее значение s=0 или s <= значению обработанного s текущим потоком (workersCurrentS), то ждём
        waitingWorkersCurrentS(tid);    
        setIsWorkerReady(tid, false);
        cur_s = s.load();
        calculationFunc(tid, cur_s);
        mutex.lock();
            workersCurrentS[tid]=cur_s;
            printf("[worker %d] workersCurrentS[%d]=cur_s=%d\n", tid, tid, workersCurrentS[tid]);
            mutex.unlock();    
        setIsWorkerReady(tid, true);
    } while (cur_s < numSteps);
    
    

    /*waitingStartWorkingCommand();
    // Если текущее значение s=0 или s <= значению обработанного s текущим потоком (workersCurrentS), то ждём
    waitingWorkersCurrentS(tid);    
    setIsWorkerReady(tid, false);
    cur_s = s.load();
    calculationFunc(tid, cur_s);
    mutex.lock();
        workersCurrentS[tid]=cur_s;
        printf("[worker %d] workersCurrentS[%d]=cur_s=%d\n", tid, tid, workersCurrentS[tid]);
        mutex.unlock();    
    setIsWorkerReady(tid, true);*/

    
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
    printf("[manager] Started. s = %d\n", s.load());
    //printIsWorkersReady();
    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    waitingAllWorkersReady();
    printIsWorkersReady();

    s++;
    while(s.load() <= numSteps)
    {        
        printf("[manager] Step %d starting...\n", s.load());
        setIsWorkerReadyAll(false); printIsWorkersReady();    
        setStartWorking(true);
        waitingAllWorkersReady(); printIsWorkersReady();    
        setStartWorking(false);
        printf("[manager] Step %d completed!\n", s.load());
        s++;
    }       

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