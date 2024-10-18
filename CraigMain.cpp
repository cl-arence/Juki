// Adapted and modernized by Craig Scratchley from Listing 4.1
#include <thread>
#include <iostream>
#include "threadsafe_queue.hpp"
#include "datachunk.hpp"

int counter{0};

bool more_data_to_prepare()
{
    return 3 > counter;
    //return true; // false;
}

//struct data_chunk
//{};

data_chunk prepare_data()
{
    ++ counter;
    return data_chunk(counter); // supply counter to data_chunk
}

void process(data_chunk&)
{}

bool is_last_chunk(data_chunk& d)
{
    return 3 <= d.i;
//    return false; // true;
}

threadsafe_queue<data_chunk> ts_queue; // renamed from data_queue

void data_preparation_thread()
{
    std::cout << "Preparation thread starting\n";
    while(more_data_to_prepare())
    {
        data_chunk const data{prepare_data()};
        ts_queue.push(data);
        ts_queue.drain();
    }
    std::cout << "Preparation thread ending\n";
}

void data_processing_thread()
{
    std::cout << "Processing thread starting\n";
    while(true)
    {
        data_chunk data{*ts_queue.wait_and_pop()};
        process(data);
        if(is_last_chunk(data))
            break;
    }
    std::cout << "Processing thread ending\n";
}

int main()
{
    // use jthread instead of thread
    std::jthread t2(data_processing_thread); // re-ordered thread creation
    std::cout << "Between thread creation\n";
    std::jthread t1(data_preparation_thread);
    std::cout << "Both threads created\n";
}
