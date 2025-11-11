cmake_minimum_required(VERSION 3.0)

set(PROJ_NAME "xmlinterp4config")
project(${PROJ_NAME} LANGUAGES CXX)


find_package(XercesC 3.2.0 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/inc)
add_executable(${PROJECT_NAME} src/main.cpp src/xmlinterp.cpp)
target_link_libraries(${PROJECT_NAME} xerces-c)

add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/config config
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo " Utworzony zostal program:  ${PROJ_NAME}"
    COMMAND ${CMAKE_COMMAND} -E echo ""
  )
