#shader vertex
#if GL_ES
precision mediump float;
#endif

out vec2 fin_uv;

uniform mat4 u_projView;

vec4 plane[12] = vec4[]
(
  vec4(-1.0f, -1.0f, 0.0f, 0.0f),
  vec4(-1.0f,  1.0f, 0.0f, 0.0f),
  vec4( 0.0f,  0.0f, 0.0f, 1.0f),
  vec4(-1.0f,  1.0f, 0.0f, 0.0f),
  vec4( 1.0f,  1.0f, 0.0f, 0.0f),
  vec4( 0.0f,  0.0f, 0.0f, 1.0f),

  vec4( 1.0f,  1.0f, 0.0f, 0.0f),
  vec4( 1.0f, -1.0f, 0.0f, 0.0f),
  vec4( 0.0f,  0.0f, 0.0f, 1.0f),
  vec4( 1.0f, -1.0f, 0.0f, 0.0f),
  vec4(-1.0f, -1.0f, 0.0f, 0.0f),
  vec4( 0.0f,  0.0f, 0.0f, 1.0f)
);

void main()
{
  vec4 pos = plane[gl_VertexID];

  gl_Position = pos;
}

#shader fragment
// Shader stolen from: https://www.shadertoy.com/view/ltBXDd

#if GL_ES
precision mediump float;
#endif

in vec2 fin_uv;

out vec4 fout_color;

uniform vec2 u_resolution;
uniform float u_height;

#define PI (3.14159265359)

const float g_duration = 8.0f; // Layer advection speed
const float g_speed = 30.0f; // Star speed
const float g_zoom = 3.0f;
const vec3 g_projection = vec3(-1.0f, 0.0f, 1.0f) / 3.0f;
const vec2 g_direction = vec2(-0.5f, -PI);

#define R(uv, d) fract(1e4 * sin((uv) * mat2(1234, -53, 457, -17) + (d)))

float MakeLayer(vec2 uv, float pos) {
  uv = uv / exp2(pos - 8.0f);

  vec2 iuv = ceil(uv * g_duration / (3.0f + pos));
  vec2 star = 0.2f + 0.6f * R(iuv, 0.0f);
  float radius = 18.0f * R(iuv, 1.0f).x;
  return (radius > 1.0f) ? 1.0f : length(star - fract(uv)) * 8.0f / (1.0f + 5.0f * radius);
}

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution + g_direction;

  vec3 pos = fract(u_height / g_duration / g_speed + g_projection + 0.5f) - 0.5f;
  vec3 weight = 0.5f + 0.5f * cos(6.28f * pos);
  pos = pos * g_duration + g_zoom;

  vec3 starLayer;
  starLayer.x = MakeLayer(uv, pos.x);
  starLayer.y = MakeLayer(-uv, pos.y);
  starLayer.z = MakeLayer(uv.yx, pos.z);
  starLayer = 0.03f / (starLayer * 0.7f); // Brightness

  fout_color += dot(weight, starLayer);

  // Enable if you want to separate layer by color
  // fout_color = vec4(weight * starLayer, 1.0f);
}
