FILE(GLOB LIVE_SRC "*.cc")
add_executable(live_service ${LIVE_SRC})
target_link_libraries(live_service  drillstatic pthread rt)