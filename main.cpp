#include "processor.hpp"
int main() {
    Processor processor{};
    for (size_t i = 0; i < 100; i++)
    {
        int r = 1 + ( std::rand() % ( 10 - 1 + 1 ) );
        InputMessage* message = new InputMessage(i, r);
        processor.enqueue_in(message);
    }
    sleep(100);
}