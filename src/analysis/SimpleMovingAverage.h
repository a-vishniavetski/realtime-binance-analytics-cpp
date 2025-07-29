//
// Created by kirsche on 7/30/25.
//

#ifndef SIMPLEMOVINGAVERAGE_H
#define SIMPLEMOVINGAVERAGE_H
#include <deque>
#include <cstddef>

class SimpleMovingAverage {
    std::deque<double> prices;
    double sum = 0.0;
    size_t window;

public:
    explicit SimpleMovingAverage(size_t window);

    double add_price_point(double price);

    [[nodiscard]] bool isReady() const;
    [[nodiscard]] double get() const;
};

#endif //SIMPLEMOVINGAVERAGE_H
