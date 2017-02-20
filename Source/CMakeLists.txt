
add_subdirectory(Animation)
add_subdirectory(Framework)
add_subdirectory(GameObject)
add_subdirectory(Multiplayer)
add_subdirectory(UserInterface)
add_subdirectory(Test)

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/Collision.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Serializable.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/StandardCursor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp
        PARENT_SCOPE)

set(TEST_FILES ${TEST_FILES} PARENT_SCOPE)
