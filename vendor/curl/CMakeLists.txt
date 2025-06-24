set(CURL_ZLIB OFF CACHE STRING "Build curl with ZLIB support (AUTO, ON or OFF)")
set(CURL_BROTLI OFF CACHE STRING "Build curl with brotli support (AUTO, ON or OFF)")
set(CURL_ZSTD OFF CACHE STRING "Build curl with zstd support (AUTO, ON or OFF)")

set(CURL_USE_LIBPSL OFF CACHE BOOL "Use libpsl")
set(CURL_USE_LIBSSH2 OFF CACHE BOOL "Use libssh2")

set(USE_NGHTTP2 OFF CACHE BOOL "Use nghttp2 library")
set(USE_LIBIDN2 OFF CACHE BOOL "Use libidn2 for IDN support")

set(BUILD_EXAMPLES OFF CACHE BOOL "Build libcurl examples")
set(BUILD_CURL_EXE OFF CACHE BOOL "Build curl executable")
set(BUILD_TESTING OFF CACHE BOOL "Build tests")

add_subdirectory(curl)
