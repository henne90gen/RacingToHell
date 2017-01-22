
add_subdirectory(Boss)

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/AICar.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Bullet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Car.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObjectFactory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PlayerCar.cpp
        PARENT_SCOPE)

