project(flock_core)

add_library(
        ${PROJECT_NAME} SHARED
        src/common.hpp
        src/common.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE FLK_SHARED_LIB)
