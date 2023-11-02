find_package(Spdlog)
find_package(Eigen)

add_library(MoonBase STATIC
  ./Log.cpp
  ./LogBase.cpp
)
add_library(Moon::Base ALIAS MoonBase)

target_include_directories(MoonBase PUBLIC
  ${MOON_DIR}/include
)

target_link_libraries(MoonBase PUBLIC
  Eigen3::Eigen
  spdlog::spdlog
)

target_compile_definitions(MoonBase PUBLIC
  _USE_MATH_DEFINES # For M_PI macro
)

target_compile_features(MoonBase PUBLIC cxx_std_17)

target_compile_options(MoonBase PUBLIC
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4244 /wd4267 /wd4127>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if(MOON_LOGGING)
  target_compile_definitions(MoonBase PUBLIC LOGGING_ENABLED)
  message(STATUS "Logging enabled")
endif()

if(MOON_TRACE_LOGGING)
  target_compile_definitions(MoonBase PUBLIC TRACE_LOGGING_ENABLED)
  message(STATUS "Detailed logging enabled")
endif()

add_subdirectory(./Physics)
add_subdirectory(./Renderer)
