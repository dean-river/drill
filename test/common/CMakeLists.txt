add_executable(string_test string_test.cc)
target_link_libraries(string_test gtest gtest_main drillstatic pthread)

add_executable(math_test math_test.cc)
target_link_libraries(math_test gtest gtest_main drillstatic pthread)

add_executable(properties_test properties_test.cc)
target_link_libraries(properties_test  drillstatic )

add_executable(cmdline_test cmdline_test.cc)
target_link_libraries(cmdline_test  drillstatic )