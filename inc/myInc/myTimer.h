#ifndef _MYTIMER_H_
#define _MYTIMER_H_

#include <iostream>
#include <chrono>
#include <mutex>
#include <future>
#include <condition_variable>
#include "modbusCommuncation.h"

class myTimer
{
public:
    myTimer()
    {
    }

    myTimer(const myTimer& timer) = delete;

    ~myTimer()
    {
        stop();
    }

    void start(int interval, std::function<void(modbusRtuMaster*)> task, modbusRtuMaster* mbMaster)
    {
        if (!m_stop)
            return;
        m_stop = false;

        m_future = std::async(std::launch::async, [this, interval,task, mbMaster]()
            {
                auto start = std::chrono::high_resolution_clock::now(); //当前时间
                auto end = start + std::chrono::milliseconds(interval); //结束时间
                while (!m_stop)
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    const auto status = m_cv.wait_until(lock, end);
                    //const auto status = m_cv.wait_for(lock, std::chrono::milliseconds(interval));
                    if (status == std::cv_status::timeout)
                    {
                        start = std::chrono::high_resolution_clock::now(); //当前时间
                        end = start + std::chrono::milliseconds(interval); //结束时间
                        task(mbMaster);
                    }
                }
            });
    }

    void stop()
    {
        if (m_stop)
            return;

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_stop = true;
        }
        m_cv.notify_one();
        m_future.wait();
    }

private:
    bool m_stop{ true };
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::future<void> m_future;
};

void myTimerTask(modbusRtuMaster* mbMaster);

#endif 
