cmake_minimum_required(VERSION 3.15)
include(AddGoogleTest) # Could be in /tests/CMakeLists.txt

add_executable(test_ltc 
	test_vmap.cpp
	test_vset.cpp
	test_range.cpp
	test_avector.cpp
	test_amap.cpp
)

target_link_libraries(test_ltc gtest gtest_main libltc)
install(TARGETS test_ltc RUNTIME DESTINATION bin)

add_test(
	NAME test_ltc
	COMMAND test_vmap
)

target_compile_features(test_ltc PRIVATE cxx_std_14)
