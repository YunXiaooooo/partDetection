#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
    ThreadPool(size_t);
    //模板函数，此处使用class和typename效果一致，...表示可选参数，可以传入不定数量的参数
    template<class F, class... Args>    
    auto enqueue(F&& f, Args&&... args)                             //&&右值引用
        -> std::future<typename std::result_of<F(Args...)>::type>;  //auto占位作用，实际函数返回类型由->决定，typename 用于对编译器声明后面是嵌套类型，而非数据
    ~ThreadPool();                                                  //result_of::type用于在编译时推导出一个可调用对象的类型
private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // the task queue
    std::queue< std::function<void()> > tasks;
    
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};
 
// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
    :   stop(false)
{
    for(size_t i = 0;i<threads;++i)
        workers.emplace_back(
            [this]                                                              //使lambda表达式函数按值捕获当前对象的this指针，lambda函数拥有与类函数一样的访问权限
            {
                for(;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);   //在wait阻塞期间会自动释放lock,在wait被唤醒以后确认lambda函数给出的谓词为真时重新取得锁
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });//若第一次运行到此处，谓词为真，不会阻塞直接继续运行
                        if(this->stop && this->tasks.empty())//如果线程池stop唤醒的条件变量，结束线程
                            return;
                        task = std::move(this->tasks.front());//move移动为右值
                        this->tasks.pop();
                    }

                    task();
                }
            }
        );
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;//using 使用return_type替换返回类型
    // make_shared函数的主要功能是在动态内存中分配一个对象并使用（）内的初始化它，返回指向此对象的shared_ptr;
    //由于是通过shared_ptr管理内存，因此一种安全分配和使用动态内存的方法
    // std::packaged_task 包装一个可调用的对象，并且允许异步获取该可调用对象产生的结果。这里的打包类型是return_type
    //forward使()转化为<>相同类型的左值或右值引用,bind绑定，使std::forward<Args>(args)成为std::forward<F>(f)的参数
    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();// 与模板函数的返回类型一致，是函数异步的执行结果。
    {
        std::unique_lock<std::mutex> lock(queue_mutex);//取得锁，若当前由线程正在从队列中取任务会被阻塞部分时刻

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();//从wait阻塞等待的线程内随机唤醒一个，如果所有线程都没有wait阻塞，那第一个结束当前task的线程会因为队列非空直接开始取下一个task
    return res;//返回可以使用feature.get()获取结果的变量
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}

#endif
