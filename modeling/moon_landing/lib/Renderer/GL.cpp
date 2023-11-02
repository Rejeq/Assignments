#include "Renderer/GL.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>

#include "Log.h"

using namespace Ren;

Buffer::~Buffer() {
  if (m_buff != 0) {
    LOG_TRACE("Destroying {} Buffer from destructor", m_buff);
    Destroy();
  }
}

void Buffer::Bind(GLenum target) const {
  glBindBuffer(target, m_buff);
}

void Buffer::Destroy() {
  glDeleteBuffers(1, &m_buff);
  m_buff = 0;
  m_size = 0;
  m_capacity = 0;
}

void Buffer::Append(const void* src, size_t count) {
  const size_t newSize = m_size + count;
  assert(newSize <= m_capacity);

  glBindBuffer(GL_ARRAY_BUFFER, m_buff);
  glBufferSubData(GL_ARRAY_BUFFER, ToBytes(m_size), ToBytes(count), src);
  m_size = newSize;
}

void Buffer::Append(const void* src, size_t count,
                    std::function<void(void*)> func) {
  const size_t newSize = m_size + count;
  assert(newSize <= m_capacity);

  glBindBuffer(GL_ARRAY_BUFFER, m_buff);
  uint8_t* dst = (uint8_t*)glMapBufferRange(GL_ARRAY_BUFFER, ToBytes(m_size),
                                            ToBytes(count), GL_MAP_WRITE_BIT);
  if (dst == nullptr)
    LOG_ERROR("Unable to map OpenGL buffer");

  for (size_t i = 0; i < count; i++) {
    const size_t offset = ToBytes(i);
    void* el = dst + offset;

    memcpy(el, ((const uint8_t*)src) + offset, ToBytes(1));
    func(el);
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);

  m_size = newSize;
}

void Buffer::StretchAppend(const void* src, size_t count, size_t stretchCount) {
  const size_t newSize = m_size + count * stretchCount;
  assert(newSize <= m_capacity);

  glBindBuffer(GL_ARRAY_BUFFER, m_buff);

  uint8_t* dst = (uint8_t*)glMapBufferRange(GL_ARRAY_BUFFER, ToBytes(m_size),
                                            ToBytes(count * stretchCount),
                                            GL_MAP_WRITE_BIT);
  if (dst == nullptr)
    LOG_ERROR("Unable to map OpenGL buffer");

  for (size_t i = 0; i < stretchCount; i++) {
    const size_t offset = ToBytes(i * count);
    memcpy(dst + offset, src, ToBytes(count));
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);
  m_size = newSize;
}

bool Buffer::Reserve(size_t newCapacity) {
  if (m_capacity >= newCapacity)
    return false;

  GLuint newBuff = 0;
  glGenBuffers(1, &newBuff);

  LOG_DEBUG("Reserving memory for '{}' buffer id with {} bytes", newBuff,
            ToBytes(newCapacity));

  // TODO: Allow to change buffer usage
  glBindBuffer(GL_COPY_WRITE_BUFFER, newBuff);
  glBufferData(GL_COPY_WRITE_BUFFER, ToBytes(newCapacity), nullptr,
               GL_DYNAMIC_DRAW);

  if (m_buff != 0) {
    size_t minCap = std::min(newCapacity, m_capacity);

    glBindBuffer(GL_COPY_READ_BUFFER, m_buff);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                        ToBytes(minCap));
    glDeleteBuffers(1, &m_buff);
  }

  m_buff = newBuff;
  m_capacity = newCapacity;

  return true;
}
