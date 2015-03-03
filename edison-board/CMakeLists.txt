include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(server)
add_subdirectory(dataglutton)

add_executable(snow-edison edison.cpp service_manager.cpp)

target_link_libraries(snow-edison snow-server snow-dataglutton ${CMAKE_THREAD_LIBS_INIT})