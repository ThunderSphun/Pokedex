add_subdirectory("glad/cmake" "glad_cmake")
glad_add_library(glad REPRODUCIBLE LOADER API gl:core=4.3)
