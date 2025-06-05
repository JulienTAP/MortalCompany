#include "buffers/vao_buffer.hpp"

#include <cassert>
#include <iostream>

#include "buffers/general_buffer.hpp"
#include "GL/glew.h"

VAOBuffer::VAOBuffer() { glCreateVertexArrays(1, &m_vao); }

void VAOBuffer::quickBind() const { glBindVertexArray(m_vao); }

void VAOBuffer::fullBind() const {
  quickBind();

  for (auto &element : m_register)
    if (element.get() != nullptr) element->bind();
}

void VAOBuffer::unbind() { glBindVertexArray(0); }

size_t VAOBuffer::validateSlot(VAOBuffer const &buffer, VAOSlot slot) {
  assert(slot != VAOSlot::SLOTS);

  auto const index = static_cast<size_t>(slot);
  assert(buffer.m_register.at(index).get() != nullptr);

  return index;
}

void VAOBuffer::activateSlot(VAOSlot slot) const {
  const size_t index = validateSlot(*this, slot);

  glEnableVertexAttribArray(index);
}

void VAOBuffer::deactivateSlot(VAOSlot slot) const {
  const size_t index = validateSlot(*this, slot);

  glDisableVertexAttribArray(index);
}

void VAOBuffer::bindSlot(VAOSlot slot,
                         const std::shared_ptr<GeneralBuffer> &buffer,
                         size_t dataIndex) {
  assert(slot != VAOSlot::SLOTS);
  const auto index = static_cast<size_t>(slot);
  assert(buffer.get() != nullptr);

  if (m_register.at(index) != nullptr) m_register.at(index).reset();
  m_register.at(index) = buffer;

  buffer->bind();

  auto [type, size, offset] = buffer->getDataMapAt(dataIndex);

  if (size == ComponentSize::kZero) return;

  glVertexAttribPointer(
      index, static_cast<GLint>(size), type, GL_FALSE, buffer->getStride(),
      reinterpret_cast<void *>(static_cast<uintptr_t>(offset)));
}

std::weak_ptr<GeneralBuffer> VAOBuffer::getBufferAt(VAOSlot slot) const {
  assert(slot != VAOSlot::SLOTS);
  const auto index = static_cast<size_t>(slot);

  auto buffer = m_register.at(index);
  return {buffer};
}

void VAOBuffer::unbindSlot(VAOSlot slot) {
  const size_t index = validateSlot(*this, slot);
  deactivateSlot(slot);

  m_register.at(index)->unbind();
  m_register.at(index).reset();
}

// Move constructor
VAOBuffer::VAOBuffer(VAOBuffer&& other) noexcept {
    m_vao = other.m_vao;
    m_register = std::move(other.m_register);
    other.m_vao = 0;
    // Optionally clear other's register, but std::move is enough for shared_ptr
}

// Move assignment operator
VAOBuffer& VAOBuffer::operator=(VAOBuffer&& other) noexcept {
    if (this != &other) {
        // Release current VAO if valid
        if (m_vao != 0) {
            glDeleteVertexArrays(1, &m_vao);
        }
        m_vao = other.m_vao;
        m_register = std::move(other.m_register);
        other.m_vao = 0;
        // Optionally clear other's register
    }
    return *this;
}

VAOBuffer::~VAOBuffer() {
  if (m_vao == 0) return;
  glDeleteVertexArrays(1, &m_vao);
  std::cout << "VAOBuffer deleted" << std::endl;
}

