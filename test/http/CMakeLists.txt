#add_executable(uri_test uri_test.cc)
#target_link_libraries(uri_test gtest gtest_main drillstatic pthread)

add_executable(http_server http_server.cc)
target_link_libraries(http_server  drillstatic pthread rt)


add_executable(live_dispatch live_dispatch.cc)
target_link_libraries(live_dispatch  drillstatic pthread rt)