FILE(GLOB LIVE_ASYNC_SRC "*.cc")
add_executable(live_service_async ${LIVE_ASYNC_SRC})
target_link_libraries(live_service_async  drillstatic pthread rt)