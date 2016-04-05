add_executable(string_test string_test.cc)
target_link_libraries(string_test gtest gtest_main drillstatic pthread)

add_executable(math_test math_test.cc)
target_link_libraries(math_test gtest gtest_main drillstatic pthread)

add_executable(properties_test properties_test.cc)
target_link_libraries(properties_test  drillstatic )

add_executable(cmdline_test cmdline_test.cc)
target_link_libraries(cmdline_test  drillstatic )

add_executable(matrix_test matrix_test.cc)
target_link_libraries(matrix_test gtest gtest_main drillstatic pthread)

add_executable(lru_cache_test lru_cache_test.cc)
target_link_libraries(lru_cache_test gtest gtest_main drillstatic pthread)