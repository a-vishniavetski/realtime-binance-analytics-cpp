//
// Created by kirsche on 7/30/25.
//

#include "ExponentialMovingAverage.h"

ExponentialMovingAverage::ExponentialMovingAverage(const size_t window) : window(window), alpha(2.0 / (window + 1)) {};


double ExponentialMovingAverage::add_price_point(double price) {
    if (!initialized) {
        value = price;
        initialized = true;
    } else {
        value = alpha * price + (1.0 - alpha) * value;
    }
    return value;
}

[[nodiscard]] bool ExponentialMovingAverage::isReady() const { return initialized; };
[[nodiscard]] double ExponentialMovingAverage::get() const { return value; };
