//
// Created by kirsche on 7/30/25.
//

#ifndef ANALYZER_H
#define ANALYZER_H

#include <iostream>

#include "SimpleMovingAverage.h"
#include "ExponentialMovingAverage.h"

class Analyzer {
    SimpleMovingAverage sma;
    ExponentialMovingAverage ema;


public:
    uint_fast64_t counter = 0;
    Analyzer();

    void process_price(double price);

    void print_stats() const {
        if (sma.isReady() && ema.isReady()) {
            std::cout << "SMA: " << sma.get() << " EMA: " << ema.get() << '\n';
        }
    }

    void print_counter() {
        std::cout << "Counter: " << counter << '\n';
    }
};



#endif //ANALYZER_H
