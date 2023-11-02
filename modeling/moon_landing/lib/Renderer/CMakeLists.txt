find_package(GLEW)

add_library(MoonRenderer STATIC
  ./Camera3d.cpp
  ./CameraBase.cpp
  ./GL.cpp
  ./ShaderDrawer.cpp
  ./Shader.cpp
  ./SurfaceDrawer.cpp
)
add_library(Moon::Renderer ALIAS MoonRenderer)

target_link_libraries(MoonRenderer PUBLIC
  MoonBase
  GLEW::GLEW
)
