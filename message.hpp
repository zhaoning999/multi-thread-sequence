#ifndef __MESSAGE__HPP__
#define __MESSAGE__HPP__
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <condition_variable>


struct InputMessage
{
    int t_;
    int id_;
    InputMessage(int id, int t):id_(id), t_(t) {}
    void sleep() {
        ::sleep(t_);
    }
    void print() {
        std::cout << "intput message id " << id_ << " sleep time " << t_ << "\n";
    }
};

struct OutputMessage
{
    InputMessage* inputMessage_;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool is_finish = false;
    void print() {
        // {
        //     std::unique_lock<std::mutex> lock(this->queue_mutex);
        //     this->condition.wait(lock,
        //         [this]{ return this->is_finish;});
        // }
        inputMessage_->print();
    }
    void doSomething() {
        inputMessage_->sleep();
        // {
        //     std::unique_lock<std::mutex> lock(this->queue_mutex);
        //     is_finish = true;
        // }
        // condition.notify_one();
    }
    OutputMessage(InputMessage* inputMessage): inputMessage_(inputMessage) {};
};

#endif