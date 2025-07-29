//
// Created by kirsche on 7/30/25.
//

#ifndef EXPONENTIALMOVINGAVERAGE_H
#define EXPONENTIALMOVINGAVERAGE_H
#include <deque>
#include <cstddef>


class ExponentialMovingAverage {
    std::deque<double> prices;
    double sum = 0.0;
    size_t window;
    double alpha;
    bool initialized = false;
    double value = 0.0;

public:
    explicit ExponentialMovingAverage(size_t window);

    double add_price_point(double price);

    [[nodiscard]] bool isReady() const;
    [[nodiscard]] double get() const;
};



#endif //EXPONENTIALMOVINGAVERAGE_H
