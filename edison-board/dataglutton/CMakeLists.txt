add_library(snow-dataglutton data_writer.cpp uart_device.cpp fake_data_provider.cpp log_manager.cpp)

target_link_libraries(snow-dataglutton snow-protocol)
