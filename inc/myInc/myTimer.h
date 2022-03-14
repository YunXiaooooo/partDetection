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
        printf("myTimer���� \n");
        stop();
    }

    void start(int interval, std::function<void(modbusRtuMaster*)> task, modbusRtuMaster* mbMaster)
    {
        if (!readM_stop())
            return;
        setM_stop(false);//����ʱ����m_stop��Ϊfalse

        m_future = std::async(std::launch::async, [this, interval,task, mbMaster]()
            {
                auto start = std::chrono::high_resolution_clock::now(); //��ǰʱ��
                auto end = start + std::chrono::milliseconds(interval); //����ʱ��
                while (!readM_stop())
                {
                    
                    std::unique_lock<std::mutex> lock(m_mutex);
                    const auto status = m_cv.wait_until(lock, end);
                    lock.unlock();//�����ͷ���������modbusͨ�ų�ʱʹ������ʱ�䱻�������˳�ʱstop���޷��޸�m_stop״̬
                    //const auto status = m_cv.wait_for(lock, std::chrono::milliseconds(interval));
                    if (status == std::cv_status::timeout)
                    {
                        start = std::chrono::high_resolution_clock::now(); //��ǰʱ��
                        end = start + std::chrono::milliseconds(interval); //����ʱ��
                        task(mbMaster);
                    }
                }
            });
    }

    void stop()
    {
        if (readM_stop())
            return;

        setM_stop(true);
        m_cv.notify_one();
        m_future.wait();
    }

private:
    bool m_stop{ true };
    bool readM_stop()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_stop;
    }
    void setM_stop(bool tf)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop = tf;
    }
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::future<void> m_future;
};

void myTimerTask(modbusRtuMaster* mbMaster);

#endif 
