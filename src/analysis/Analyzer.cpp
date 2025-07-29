//
// Created by kirsche on 7/30/25.
//

#include "Analyzer.h"

#include <iostream>
#include <optional>

Analyzer::Analyzer(): sma(5), ema(5) {} ;

void Analyzer::process_price(double price) {
    sma.add_price_point(price);
    ema.add_price_point(price);
    counter++;
}

