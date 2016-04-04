set(
	COMMON_SRC
	common/string.cc
	)

set(
	COMMON_HEADERS
	string.hpp
	)

	
add_library(drillstatic STATIC ${COMMON_SRC})



install (FILES "${PROJECT_BINARY_DIR}/walle/config/config.h" DESTINATION include/drill/config)