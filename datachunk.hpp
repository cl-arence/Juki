/*
 * datachunk.h
 *
 *  Created on: Oct 3, 2024
 *      Author: Craig Scratchley
 */

#ifndef DATACHUNK_H_
#define DATACHUNK_H_

#include <format>
#include <iostream>

class data_chunk {
public:
    inline static int copyCounter;
    int i;
    data_chunk(int data) : i(data) {
        std::cout << std::format("Constructor: {}\n", i);
        ++ copyCounter;
    };
    data_chunk(const data_chunk &d0) : i(d0.i) {
        std::cout << std::format("Copy Constructor: {}\n", i);
        ++ copyCounter;
    };

    virtual ~data_chunk() {
       std::cout << std::format("Destructor: {}\n", i);
        -- copyCounter;
    };
};

#endif /* DATACHUNK_H_ */
