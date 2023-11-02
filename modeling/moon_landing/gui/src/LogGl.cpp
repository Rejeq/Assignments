#include "LogGl.h"

#include <string_view>

#include "Renderer/GL.h"
#include "Log.h"

static const char* GlDebugSourceToString(GLenum source) {
  switch (source) {
    case GL_DEBUG_SOURCE_API: return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW_SYSTEM";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
    case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD_PARTY";
    case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
    case GL_DEBUG_SOURCE_OTHER: return "OTHER";
  }
  return "Undefined";
}

static const char* GlDebugTypeToString(GLenum type) {
  switch (type) {
    case GL_DEBUG_TYPE_ERROR: return "GL_ERROR";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
    case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
    case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
    case GL_DEBUG_TYPE_MARKER: return "MARKER";
    case GL_DEBUG_TYPE_PUSH_GROUP: return "PUSH_GROUP";
    case GL_DEBUG_TYPE_POP_GROUP: return "POP_GROUP";
    case GL_DEBUG_TYPE_OTHER: return "OTHER";
  }
  return "Undefined";
}

static void GlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                       GLsizei /*length*/, const GLchar* message,
                       const void* /*userParam*/) {
  // Since the shader error handled in the Shader class
  if (source == GL_DEBUG_SOURCE_SHADER_COMPILER)
    return;

  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    return;

  constexpr std::string_view msg = "{}: {}: [{}]:\n{}\n";
#define MESSAGE_ARGS \
  GlDebugSourceToString(source), GlDebugTypeToString(type), id, message

  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: LOG_ERROR(msg, MESSAGE_ARGS); break;
    case GL_DEBUG_SEVERITY_MEDIUM: LOG_WARN(msg, MESSAGE_ARGS); break;
    case GL_DEBUG_SEVERITY_LOW: LOG_INFO(msg, MESSAGE_ARGS); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_TRACE(msg, MESSAGE_ARGS); break;
  }
#undef MESSAGE_ARGS
}

void InitGlLogging() {
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(GlMessageCallback, nullptr);
}
