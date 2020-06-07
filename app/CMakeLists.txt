cmake_minimum_required(VERSION 3.15)

add_executable(ltc main.cpp)

target_link_libraries(ltc PRIVATE libltc)
target_compile_features(ltc PRIVATE cxx_std_14)
