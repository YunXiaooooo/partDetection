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
    //ģ�庯�����˴�ʹ��class��typenameЧ��һ�£�...��ʾ��ѡ���������Դ��벻�������Ĳ���
    template<class F, class... Args>    
    auto enqueue(F&& f, Args&&... args)                             //&&��ֵ����
        -> std::future<typename std::result_of<F(Args...)>::type>;  //autoռλ���ã�ʵ�ʺ�������������->������typename ���ڶԱ���������������Ƕ�����ͣ���������
    ~ThreadPool();                                                  //result_of::type�����ڱ���ʱ�Ƶ���һ���ɵ��ö��������
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
            [this]                                                              //ʹlambda���ʽ������ֵ����ǰ�����thisָ�룬lambda����ӵ�����ຯ��һ���ķ���Ȩ��
            {
                for(;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);   //��wait�����ڼ���Զ��ͷ�lock,��wait�������Ժ�ȷ��lambda����������ν��Ϊ��ʱ����ȡ����
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });//����һ�����е��˴���ν��Ϊ�棬��������ֱ�Ӽ�������
                        if(this->stop && this->tasks.empty())//����̳߳�stop���ѵ����������������߳�
                            return;
                        task = std::move(this->tasks.front());//move�ƶ�Ϊ��ֵ
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
    using return_type = typename std::result_of<F(Args...)>::type;//using ʹ��return_type�滻��������
    // make_shared��������Ҫ�������ڶ�̬�ڴ��з���һ������ʹ�ã����ڵĳ�ʼ����������ָ��˶����shared_ptr;
    //������ͨ��shared_ptr�����ڴ棬���һ�ְ�ȫ�����ʹ�ö�̬�ڴ�ķ���
    // std::packaged_task ��װһ���ɵ��õĶ��󣬲��������첽��ȡ�ÿɵ��ö�������Ľ��������Ĵ��������return_type
    //forwardʹ()ת��Ϊ<>��ͬ���͵���ֵ����ֵ����,bind�󶨣�ʹstd::forward<Args>(args)��Ϊstd::forward<F>(f)�Ĳ���
    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();// ��ģ�庯���ķ�������һ�£��Ǻ����첽��ִ�н����
    {
        std::unique_lock<std::mutex> lock(queue_mutex);//ȡ����������ǰ���߳����ڴӶ�����ȡ����ᱻ��������ʱ��

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();//��wait�����ȴ����߳����������һ������������̶߳�û��wait�������ǵ�һ��������ǰtask���̻߳���Ϊ���зǿ�ֱ�ӿ�ʼȡ��һ��task
    return res;//���ؿ���ʹ��feature.get()��ȡ����ı���
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
