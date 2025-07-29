//
// Created by kirsche on 7/30/25.
//

#include "SimpleMovingAverage.h"


SimpleMovingAverage::SimpleMovingAverage(const size_t window) : window(window) {}

double SimpleMovingAverage::add_price_point(const double price) {
    if (prices.size() == window) {
        sum -= prices.front();
        prices.pop_front();
    }

    prices.push_back(price);
    sum += price;

    return get();
}

[[nodiscard]] bool SimpleMovingAverage::isReady() const { return prices.size() == window; }
[[nodiscard]] double SimpleMovingAverage::get() const { return sum / static_cast<double>(prices.size()); }