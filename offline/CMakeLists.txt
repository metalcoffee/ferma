cmake_minimum_required(VERSION 3.20)

project(ferma)
set(CMAKE_CXX_STANDARD 17)

add_executable(ferma main.cpp BaseElem.cpp Menu.cpp Map.cpp Shop.cpp Chat.cpp TextElem.cpp)
#add_executable(ferma server.cpp client.cpp)
#add_executable(ferma MultiplayerProject_Client.cpp MultiplayerProject_Server.cpp NetworkClient.cpp NetworkServer.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/msys64/mingw64/lib/cmake/SFML)
set(SOCI_DIR C:/msys64/mingw64/lib/cmake/SOCI)

include_directories(C:/msys64/mingw64/include)

find_package(SFML 2.5 COMPONENTS graphics network window audio system REQUIRED)
find_package(SOCI REQUIRED)

target_link_libraries(ferma sfml-system sfml-window sfml-graphics sfml-audio sfml-network soci_core_4_0 soci_postgresql_4_0)
