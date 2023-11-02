#shader vertex
#if GL_ES
precision mediump float;
#endif

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 fin_color;

uniform mat4 u_projView;

void main() {
  fin_color = color;
  gl_Position = u_projView * vec4(position, 1.0f);
}

#shader fragment
#if GL_ES
precision mediump float;
#endif

out vec4 fout_color;
in vec4 fin_color;

void main() {
  fout_color = fin_color;
}

