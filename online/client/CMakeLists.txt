cmake_minimum_required(VERSION 3.22)
project(source)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(source
        CoFarmer.cpp
        CoFarmer.h
        Game.cpp
        Game.h
        main.cpp
        Network.cpp
        Network.h
        Player.cpp
        Player.h
        BaseElem.cpp Menu.cpp Map.cpp Shop.cpp Chat.cpp TextElem.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/msys64/mingw64/lib/cmake/SFML)
set(SOCI_DIR C:/msys64/mingw64/lib/cmake/SOCI)

include_directories(C:/msys64/mingw64/include)

find_package(SFML 2.5 COMPONENTS graphics network window audio system REQUIRED)
find_package(SOCI REQUIRED)

target_link_libraries(source sfml-system sfml-window sfml-graphics sfml-audio sfml-network soci_core_4_0 soci_postgresql_4_0)
