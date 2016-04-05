
FILE(GLOB COMMON_SRC "common/*.cc")
FILE(GLOB COMMON_HEADER "common/*.h")

FILE(GLOB EVENT_SRC "event/*.cc")
FILE(GLOB EVENT_HEADER "event/*.h")
add_library(drillstatic STATIC ${COMMON_SRC} ${EVENT_SRC})
set_target_properties(drillstatic PROPERTIES OUTPUT_NAME drill)

install(TARGETS drillstatic DESTINATION lib)
install (FILES ${COMMON_HEADER} DESTINATION include/drill/common)
install (FILES ${EVENT_HEADER} DESTINATION include/drill/event)
install (FILES "${PROJECT_BINARY_DIR}/drill/config/config.h" DESTINATION include/drill/config)