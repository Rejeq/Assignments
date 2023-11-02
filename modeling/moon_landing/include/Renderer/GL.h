#pragma once

#include <cstdint>
#include <functional>

#include <gl/glew.h>

namespace Ren {

class Buffer {
 public:
  Buffer(size_t elementSizeInBytes) : m_elementSize(elementSizeInBytes) {}
  ~Buffer();

  void Bind(GLenum target) const;
  void Destroy();

  void Append(const void* src, size_t count);
  void Append(const void* src, size_t count, std::function<void(void*)> func);
  void StretchAppend(const void* src, size_t count, size_t stretchCount);

  bool Reserve(size_t newCapacity);
  size_t GetCapacity() const { return m_capacity; }

  size_t GetSize() const { return m_size; }

  // Returns size of one element in bytes
  size_t GetElementSize() const { return m_elementSize; }

 private:
  size_t ToBytes(size_t size) const { return size * m_elementSize; }

  const size_t m_elementSize;
  size_t m_size = 0;
  size_t m_capacity = 0;
  GLuint m_buff = 0;
};

}  // namespace Ren
