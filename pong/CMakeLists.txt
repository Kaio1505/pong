project(pong)
add_executable(${PROJECT_NAME} main.cpp window.cpp balls.cpp
                               ship.cpp starlayers.cpp)
enable_abcg(${PROJECT_NAME})
