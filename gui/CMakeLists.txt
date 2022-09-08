set(GUI_SRC_LIST
    config_base.cpp
    config.cpp
)
include(${wxWidgets_USE_FILE})
add_library(GUI STATIC ${GUI_SRC_LIST})
target_link_libraries(GUI ${wxWidgets_LIBRARIES})
target_include_directories (GUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
