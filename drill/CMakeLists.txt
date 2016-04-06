
FILE(GLOB COMMON_SRC "common/*.cc")
FILE(GLOB COMMON_HEADER "common/*.h")

FILE(GLOB EVENT_SRC "event/*.cc")
FILE(GLOB EVENT_HEADER "event/*.h")

FILE(GLOB NET_SRC "net/*.cc")
FILE(GLOB NET_HEADER "net/*.h")

add_library(drillstatic STATIC ${COMMON_SRC} ${EVENT_SRC} ${NET_SRC})
set_target_properties(drillstatic PROPERTIES OUTPUT_NAME drill)

install(TARGETS drillstatic DESTINATION lib)
install (FILES ${COMMON_HEADER} DESTINATION include/drill/common)
install (FILES ${EVENT_HEADER} DESTINATION include/drill/event)
install (FILES ${NET_HEADER} DESTINATION include/drill/net)
install (FILES "${PROJECT_BINARY_DIR}/drill/config/config.h" DESTINATION include/drill/config)