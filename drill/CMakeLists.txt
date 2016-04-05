set(
	COMMON_SRC
	common/string.cc
	common/md5.cc
	common/properties.cc
	common/time.cc
	common/local_thread.cc
	common/log_stream.cc
	common/logger.cc
	common/thread.cc
	common/thread_pool.cc
	common/cond.cc
	common/mutex.cc
	common/buffer.cc
	common/histogram.cc
	common/filesystem.cc
	)

set(
	COMMON_HEADERS
	common/string.h
	common/md5.h
	common/math.h
	common/properties.h
	common/random.h
	common/cmdline.h
	common/time.h
	common/ip_filter.h
	common/atmoic.h
	common/noncopyable.h
	common/fixed_buffer.h
	common/base64.h
	common/slice.h
	common/any.h
	common/local_thread.h
	common/log_stream.h
	common/logger.h
	common/thread.h
	common/thread_pool.h
	common/cond.h
	common/mutex.h
	common/scoped_lock.h
	common/buffer.h
	common/types.h
	common/weak_call.h
	common/encode.h
	common/histogram.h
	common/filesystem.h
	
	)

	
add_library(drillstatic STATIC ${COMMON_SRC})



install (FILES "${PROJECT_BINARY_DIR}/walle/config/config.h" DESTINATION include/drill/config)