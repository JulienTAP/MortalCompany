#include "buffers/general_buffer.hpp"

GeneralBuffer::GeneralBuffer(const BufferDataSource& source, GLenum bufferType, GLenum usage)
    : m_bufferType(bufferType), m_datatypes(), m_usage(usage), m_size(source.size), m_stride(Stride{0}) {
  glGenBuffers(1, &m_buffer);
  
  bind();

  glBufferData(m_bufferType, source.size, source.ptr, usage);

  unbind();

  size_t stride = 0;
  for (auto &element : source.mapping) {
    m_datatypes.emplace_back(
        MappedDataBuffer{element.type, element.size, Offset{stride}});
    stride += static_cast<size_t>(element.size) * sizeof(element.type);
  }

  m_stride = Stride{stride};
}

std::shared_ptr<GeneralBuffer> GeneralBuffer::createVBO(const BufferDataSource& src, GLenum usage) {
  return std::make_shared<GeneralBuffer>(src, GL_ARRAY_BUFFER, usage);
}

std::shared_ptr<GeneralBuffer> GeneralBuffer::createEBO(const BufferDataSource& src, GLenum usage) {
  return std::make_shared<GeneralBuffer>(src, GL_ELEMENT_ARRAY_BUFFER, usage);
}

MappedDataBuffer const &GeneralBuffer::getDataMapAt(size_t index) const {
  return m_datatypes.at(index);
}

void GeneralBuffer::bind() const { glBindBuffer(m_bufferType, m_buffer); }

void GeneralBuffer::unbind() const { glBindBuffer(m_bufferType, 0); }

GeneralBuffer::~GeneralBuffer() {
  GLint current;

  glGetIntegerv(m_bufferType, &current);
  if (static_cast<GLuint>(current) == m_buffer) unbind();

  glDeleteBuffers(1, &m_buffer);
}
