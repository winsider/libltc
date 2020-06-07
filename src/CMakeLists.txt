cmake_minimum_required(VERSION 3.15)

add_library(libltc STATIC 
    ltc.cpp
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/ltc/vmap.hpp>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/ltc/vset.hpp>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/ltc/range.hpp>
)

target_include_directories(libltc
PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_features(libltc PRIVATE cxx_std_14)
