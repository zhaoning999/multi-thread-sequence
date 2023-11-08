#ifndef __PROCESSOR__HPP__
#define __PROCESSOR__HPP__

#include <queue>
#include "ThreadPool.h"
#include "message.hpp"

struct Processor
{
    ThreadPool pool_;
    std::mutex queue_in;
    std::mutex queue_out;
    std::condition_variable condition_in;
    std::condition_variable condition_out;
    std::queue< InputMessage* > in_queue;
    std::queue< OutputMessage* > out_queue;
    std::thread* t_in;
    std::thread* t_out;
    Processor():pool_(3){
        t_in = new std::thread([this]
            {
                InputMessage* task;
                for(;;)
                {
                    {
                        std::unique_lock<std::mutex> lock(this->queue_in);
                        this->condition_in.wait(lock,
                            [this]{ return !this->in_queue.empty(); });
                        task = std::move(this->in_queue.front());
                        this->in_queue.pop();
                    }
                    OutputMessage* outmessage = new OutputMessage(task);
                    this->enqueue_out(outmessage);
                    pool_.enqueue([this, outmessage]{
                        outmessage->doSomething();
                    });
                }
            });
        t_out = new std::thread([this]
            {
                OutputMessage* task;
                for(;;)
                {
                    {
                        std::unique_lock<std::mutex> lock(this->queue_out);
                        this->condition_out.wait(lock,
                            [this]{ return !this->out_queue.empty(); });
                        task = std::move(this->out_queue.front());
                        this->out_queue.pop();
                    }
                    task->print();
                }
            });
    };
    void enqueue_in( InputMessage* inmessage) {
        {
            std::unique_lock<std::mutex> lock(queue_in);
            in_queue.emplace(inmessage);
        }
        condition_in.notify_one();
    }
    void enqueue_out( OutputMessage* outmessage) {
        {
            std::unique_lock<std::mutex> lock(queue_out);
            out_queue.emplace(outmessage);
        }
        condition_out.notify_one();
    }
};


#endif