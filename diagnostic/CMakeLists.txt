qt5_wrap_ui(UIS_HDRS ui/mainwindow.ui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(snow-diagnostic diagnostic.cpp mainwindow.cpp ${UIS_HDRS})

target_link_libraries (snow-diagnostic snow-protocol)

qt5_use_modules(snow-diagnostic Widgets Network)
