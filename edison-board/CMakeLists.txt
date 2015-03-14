include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(server)
add_subdirectory(dataglutton)
add_subdirectory(wifi_controller)

add_executable(snow-edison edison.cpp service_manager.cpp)

target_link_libraries(snow-edison snow-server snow-dataglutton snow-wifi-controller ${CMAKE_THREAD_LIBS_INIT})