find_package(spdlog 1.12.0 CONFIG)

if (NOT spdlog_FOUND AND MOON_FETCHING)
  set(SPDLOG_URL "https://github.com/gabime/spdlog/archive/refs/tags/v1.12.0.tar.gz")
  message(STATUS "Loading spdlog from ${SPDLOG_URL}")

  include(FetchContent)
  FetchContent_Declare(spdlog
    URL ${SPDLOG_URL}
  )
  FetchContent_MakeAvailable(spdlog)

  message(STATUS "spdlog configured successfully")
endif()
