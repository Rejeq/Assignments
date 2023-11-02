# PLOT_DIR - root of library source
if (NOT PLOT_DIR AND MOON_FETCHING)
  set(PLOT_URL "https://www.qcustomplot.com/release/2.1.1/QCustomPlot-source.tar.gz")
  message(STATUS "Loading QCustomPlot from: ${PLOT_URL}")

  include(FetchContent)
  FetchContent_Declare(plot
    URL ${PLOT_URL}
  )

  FetchContent_GetProperties(plot)
  if(NOT plot_POPULATED)
    FetchContent_Populate(plot)
    set(PLOT_DIR "${plot_SOURCE_DIR}")
  endif()
endif()

find_package(Qt5 COMPONENTS Core PrintSupport REQUIRED)

add_library(QCustomPlot STATIC
  ${PLOT_DIR}/qcustomplot.h
  ${PLOT_DIR}/qcustomplot.cpp
)

target_include_directories(QCustomPlot PUBLIC ${PLOT_DIR})

target_link_libraries(QCustomPlot PUBLIC
  Qt5::PrintSupport
  Qt5::Core
)

message(STATUS "QCustomPlot configured successfully")
