option(MOON_QT_PREFIX_DIR "Prefix to the qt compiler toolchain")
option(MOON_QT_BIN_DIR "Directory that contains qt binaries")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (NOT MOON_QT_PREFIX_DIR)
  message(STATUS "MOON_QT_PREFIX is not specefied, assuming that qt stuff already in PATH")
else()
  if (NOT MOON_QT_BIN_DIR)
    set(MOON_QT_BIN_DIR "${MOON_QT_PREFIX_DIR}/bin")
  endif()

  message(STATUS "Qt prefix dir: ${MOON_QT_PREFIX_DIR}")
  message(STATUS "Qt binary dir: ${MOON_QT_BIN_DIR}")
endif()

find_package(Qt5 COMPONENTS Widgets Svg REQUIRED)
find_package(AdvanceDockingSystem)
find_package(QCustomPlot)

add_executable(MoonLanding
  ./src/Main.cpp
  ./src/MainWindow.cpp
  ./src/MainWindow.h
  ./src/MainWindow.ui

  ./src/CurrentDataView.cpp
  ./src/CurrentDataView.h
  ./src/CurrentDataView.ui

  ./src/PlotView.cpp
  ./src/PlotView.h

  ./src/RocketBuilder.cpp
  ./src/RocketBuilder.h

  ./src/RocketController.cpp
  ./src/RocketController.h

  ./src/SceneView.cpp
  ./src/SceneView.h

  ./src/SetupView.cpp
  ./src/SetupView.h
  ./src/SetupView.ui

  ./src/WorldController.cpp
  ./src/WorldController.h

  ./src/LogGl.cpp
  ./Resources.qrc
  ./MoonLanding.rc
)

target_link_libraries(MoonLanding PRIVATE
  Moon::Physics
  Moon::Renderer
  AdvanceDockingSystem
  QCustomPlot
  Qt5::Widgets
  Qt5::Svg # This is required also for ADS to load icons correctly
)

if(COMMAND target_precompile_headers AND MOON_PCH)
  configure_file(${MOON_DIR}/cmake/GuiPch.h.in ${CMAKE_CURRENT_BINARY_DIR}/GuiPch.h @ONLY)
  target_precompile_headers(MoonLanding PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/GuiPch.h)
  message(STATUS "Precompiled header enabled")
endif()

include(DeployQt)
