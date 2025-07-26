cmake_minimum_required(VERSION 3.31)
project(RealTimeBinanceProcessing)

set(CMAKE_CXX_STANDARD 17)

# Find required packages
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

add_executable(RealTimeBinanceProcessing src/main.cpp)

# Link libraries
target_link_libraries(RealTimeBinanceProcessing
        ${Boost_LIBRARIES}
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
)

# Include directories
target_include_directories(RealTimeBinanceProcessing PRIVATE
        ${Boost_INCLUDE_DIRS}
)
