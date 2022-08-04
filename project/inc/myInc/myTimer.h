#ifndef _MYTIMER_H_
#define _MYTIMER_H_

#include <iostream>
#include <chrono>
#include <mutex>
#include <future>
#include <condition_variable>
#include "communication.h"
#include <atomic>

class myTimer
{
public:
    myTimer()
    {
    }

    myTimer(const myTimer&) = delete;
    myTimer& operator=(const myTimer&) = delete;
    ~myTimer()
    {
        printf("myTimer���� \n");
        stop();
    }

    template<class F, class... Args>
    void start(const int interval, F f, Args... args)
    {
        if (!m_stop)
            return;
        m_stop = false;//����ʱ����m_stop��Ϊfalse

        auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        m_future = std::async(std::launch::async, [this, interval, task]()
            {
                auto start = std::chrono::high_resolution_clock::now(); //��ǰʱ��
                auto end = start + std::chrono::milliseconds(interval); //����ʱ��
                while (!m_stop)
                {

                    std::unique_lock<std::mutex> lock(m_mutex);
                    const auto status = m_cv.wait_until(lock, end);
                    lock.unlock();//�����ͷ���������modbusͨ�ų�ʱʹ������ʱ�䱻����
                    //const auto status = m_cv.wait_for(lock, std::chrono::milliseconds(interval));
                    if (status == std::cv_status::timeout)
                    {
                        start = std::chrono::high_resolution_clock::now(); //��ǰʱ��
                        end = start + std::chrono::milliseconds(interval); //����ʱ��
                        task();
                    }
                }
            });
    }

    void stop()
    {
        if (m_stop)
            return;
        m_stop = true;
        m_cv.notify_one();
        m_future.wait();
    }

private:
    std::atomic_bool m_stop{ true };

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::future<void> m_future;
};

void myTimerTask(std::shared_ptr<communicationToolProxy> toolProxy);
#endif 
