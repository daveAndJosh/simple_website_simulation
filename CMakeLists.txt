cmake_minimum_required(VERSION 3.16)
project(my_serverless_recipe_site_sim)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cadmium_v2/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cadmium_v2/json/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
message(${CMAKE_CURRENT_SOURCE_DIR}/../cadmium_v2/include)
find_package(Boost COMPONENTS filesystem system unit_test_framework REQUIRED)
add_executable(serverless_recipe_site_sim main.cpp include/Packet.h include/ClientGenerator.h include/Internet.h include/Client.h include/ClientDispatcher.h include/Bucket.h include/Executor.h include/LambdaDispatcher.h include/Lambda.h include/Switch.h)
add_executable(generatorTest tests/TestDriver.cpp
        include/Packet.h
        include/ClientGenerator.h
        include/Internet.h
        include/Client.h
        include/ClientDispatcher.h
        include/Bucket.h
        include/Executor.h
        include/LambdaDispatcher.h
        include/Lambda.h
        include/Switch.h
        tests/GeneratorTest.h
        tests/GeneratorTest.h
        tests/CounterTest.h tests/BigIestream.hpp)
target_link_libraries(serverless_recipe_site_sim ${Boost_LIBRARIES})
target_link_libraries(generatorTest ${Boost_LIBRARIES})
