
add_executable(http_client http_client.cc)
target_link_libraries(http_client  drillstatic pthread rt)