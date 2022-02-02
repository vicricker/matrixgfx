
cmake_minimum_required(VERSION 3.16)


project(matrixBS
    VERSION 1.0
    DESCRIPTION "RGB Matrix Stuff"
    LANGUAGES C)

    file(GLOB CLOCK_SOURCES ${CMAKE_SOURCE_DIR}/clock/src/*.c)
    file(GLOB FIRE_SOURCES ${CMAKE_SOURCE_DIR}/examples/fire/*.c)
    file(GLOB PLASMA_SOURCES ${CMAKE_SOURCE_DIR}/examples/plasma/*.c)
    file(GLOB PLASMA_SOURCES ${CMAKE_SOURCE_DIR}/examples/scroller/*.c)

    add_executable(clock ${CLOCK_SOURCES})
    add_executable(fire ${FIRE_SOURCES})
    add_executable(plasma ${PLASMA_SOURCES})
    add_executable(scroller ${PLASMA_SOURCES})

    target_include_directories(clock PUBLIC ${CMAKE_SOURCE_DIR}/clock/src)
    target_include_directories(fire PUBLIC ${CMAKE_SOURCE_DIR}/fire)
    target_include_directories(plasma PUBLIC ${CMAKE_SOURCE_DIR}/plasma)
    target_include_directories(scroller PUBLIC ${CMAKE_SOURCE_DIR}/scroller)


    target_compile_options(clock PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fire PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(plasma PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(scroller PRIVATE -Wall -Wextra -Wpedantic)
    
    find_library(LIBRARY_RGBMATRIX rgbmatrix)

    target_link_libraries(clock PRIVATE ${LIBRARY_RGBMATRIX} m gif jpeg matrixgfx)
    target_link_libraries(fire PRIVATE ${LIBRARY_RGBMATRIX} gif jpeg matrixgfx)
    target_link_libraries(plasma PRIVATE ${LIBRARY_RGBMATRIX} m gif jpeg matrixgfx)
    target_link_libraries(scroller PRIVATE ${LIBRARY_RGBMATRIX} m gif jpeg matrixgfx)


    file(GLOB LIB_SOURCES matrixgfx/src/*.c)
    add_library(matrixgfx SHARED ${LIB_SOURCES})
    target_include_directories(matrixgfx PUBLIC matrixgfx/include)
