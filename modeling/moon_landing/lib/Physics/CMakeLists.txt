add_library(MoonPhysics STATIC
  ./Rocket.cpp
  ./World.cpp
)
add_library(Moon::Physics ALIAS MoonPhysics)

target_link_libraries(MoonPhysics PUBLIC
  MoonBase
)
