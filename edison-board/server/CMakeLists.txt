add_library(snow-server server.cpp client_service.cpp)

target_link_libraries(snow-server ${CMAKE_THREAD_LIBS_INIT})