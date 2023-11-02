# ADS_DIR - root of library repo
if (ADS_DIR OR NOT MOON_FETCHING)
  include("${ADS_DIR}/CMakeLists.txt")
else()
  set(ADS_URL "https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System/archive/refs/tags/4.1.1.tar.gz")
  message(STATUS "Loading Advanced Docking System from: ${ADS_URL}")

  include(FetchContent)
  FetchContent_Declare(ads
    URL ${ADS_URL}
  )

  set(ADS_VERSION "4.1.1" CACHE STRING "" FORCE)
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(BUILD_STATIC  ON CACHE BOOL "" FORCE)

  # Populates qt${QT_VERSION_MAJOR}advanceddocking target
  FetchContent_MakeAvailable(ads)

  message(STATUS "Advanced Docking System configured successfully")
endif()

add_library(AdvanceDockingSystem ALIAS qt5advanceddocking)
