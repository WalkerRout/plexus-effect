set(EXEC ${CMAKE_PROJECT_NAME})
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.cpp)
set(SOURCES ${SOURCES})

add_executable(${EXEC}_run ${SOURCES})
add_library(${EXEC}_lib STATIC ${SOURCES})

# SDL2
target_link_libraries(${EXEC}_run SDL2 SDL2_image)