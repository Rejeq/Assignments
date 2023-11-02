option(MOON_FIND_GLEW "Find glew package or fetch it" ON)

if (MOON_FIND_GLEW)
  list(REMOVE_ITEM CMAKE_MODULE_PATH "${MOON_DIR}/cmake")
  find_package(GLEW)
  list(APPEND CMAKE_MODULE_PATH "${MOON_DIR}/cmake")
endif()

if (NOT GLEW_FOUND AND MOON_FETCHING)
    set(GLEW_URL "https://github.com/Perlmint/glew-cmake/archive/refs/tags/glew-cmake-2.2.0.tar.gz")
    message(STATUS "Loading GLEW-cmake from ${GLEW_URL}")

  include(FetchContent)
  FetchContent_Declare(glew
    URL ${GLEW_URL}
  )

  FetchContent_GetProperties(glew)
  if(NOT glew_POPULATED)
    FetchContent_Populate(glew)

    add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()

  add_library(GLEW::GLEW ALIAS libglew_shared)

  message(STATUS "GLEW-cmake configured successfully")
endif()

