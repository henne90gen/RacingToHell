
add_subdirectory(MenuItem)

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/AboutMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Countdown.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameOverMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/HighscoreMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Menu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MultiplayerLobby.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MultiplayerMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OptionsMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PauseMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PauseMultiplayerMenu.cpp
        PARENT_SCOPE)

