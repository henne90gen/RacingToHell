
include_directories(${PROJECT_SOURCE_DIR}/Headers)

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/Animation/Animation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Animation/Explosion.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Animation/MechLegs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Animation/MechTop.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/Boss/BossCar.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/Boss/Carrier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/Boss/Jet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/Boss/Mech.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/Boss/Tank.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/AICar.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/Bullet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/Car.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/GameObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/GameObjectFactory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GameObject/PlayerCar.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MenuItem/ComboBox.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MenuItem/HighscoreList.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MenuItem/MenuButton.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MenuItem/MenuItem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MenuItem/PlayerTable.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MenuItem/Slider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MenuItem/StatBox.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MenuItem/Textbox.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/AboutMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/Countdown.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/GameOverMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/HighscoreMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MainMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/Menu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MultiplayerLobby.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/MultiplayerMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/OptionsMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/PauseMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/Menu/PauseMultiplayerMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/LevelUpScreen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/LoadingScreen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/LevelScreen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/GameScreen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/GameObjectScreen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/GameScreenFactory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/EmptyScreen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Screen/ExitScreen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Multiplayer/MPGameObjectContainer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Multiplayer/NetworkHandle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Collision.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Framework.cpp
        Screen/HUDScreen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RacingToHell.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Serializable.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/StandardCursor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Framework/OptionsManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Framework/LevelManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Framework/GameObjectManager.cpp)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} pthread)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif ()

