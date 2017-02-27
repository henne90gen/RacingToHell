
add_subdirectory(Action)

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/BossCar.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Carrier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Jet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Mech.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Tank.cpp
        PARENT_SCOPE)

