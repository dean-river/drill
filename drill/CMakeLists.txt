
FILE(GLOB COMMON_SRC "common/*.cc")
FILE(GLOB COMMON_HEADER "common/*.h")

FILE(GLOB EVENT_SRC "event/*.cc")
FILE(GLOB EVENT_HEADER "event/*.h")

FILE(GLOB NET_SRC "net/*.cc")
FILE(GLOB NET_HEADER "net/*.h")

FILE(GLOB SERVICE_SRC "service/*.cc")
FILE(GLOB SERVICE_HEADER "service/*.h")

FILE(GLOB HTTP_SRC "http/*.cc")
FILE(GLOB HTTP_HEADER "http/*.h")


FILE(GLOB MONITOR_SRC "monitor/*.cc")
FILE(GLOB MONITOR_HEADER "monitor/*.h")

SET(ALL_SRC
	${COMMON_SRC}
	${EVENT_SRC}
	${NET_SRC}
	${SERVICE_SRC}
	${HTTP_SRC}
	#${MONITOR_SRC}
	)

add_library(drillstatic STATIC ${ALL_SRC} )
set_target_properties(drillstatic PROPERTIES OUTPUT_NAME drill)

if(enable_dynamic)
	add_library(drillshared SHARED ${ALL_SRC})
	set_target_properties(drillshared PROPERTIES OUTPUT_NAME drill)
endif()

install(TARGETS drillstatic drillshared DESTINATION lib)

install (FILES ${COMMON_HEADER} DESTINATION include/drill/common)
install (FILES ${EVENT_HEADER} DESTINATION include/drill/event)
install (FILES ${NET_HEADER} DESTINATION include/drill/net)
install (FILES ${SERVICE_HEADER} DESTINATION include/drill/service)
install (FILES ${HTTP_HEADER} DESTINATION include/drill/http)
#install (FILES ${MONITOR_HEADER} DESTINATION include/drill/monitor)

install (FILES "${PROJECT_BINARY_DIR}/drill/config/config.h" DESTINATION include/drill/config)
