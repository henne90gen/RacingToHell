
add_subdirectory(Menu)
add_subdirectory(Screen)

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/GameScreenFactory.cpp
        PARENT_SCOPE)

