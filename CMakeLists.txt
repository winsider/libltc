cmake_minimum_required(VERSION 3.15)
project (ltc VERSION 1.0.0 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
add_subdirectory(src)
add_subdirectory(app)

# Testing
enable_testing()
add_subdirectory(test)

