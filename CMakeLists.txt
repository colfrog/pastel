cmake_minimum_required(VERSION 3.16)

project(pastel VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

qt_add_executable(pastel
  piece.cpp
  board.cpp
  BoardView.cpp
  main.cpp
)

target_link_libraries(pastel PRIVATE Qt6::Widgets)
set_target_properties(pastel PROPERTIES
  WIN32_EXECUTABLE ON
  MACOSX_BUNDLE ON
)
