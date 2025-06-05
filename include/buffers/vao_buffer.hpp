#ifndef VAO_BUFFER_H
#define VAO_BUFFER_H

#include <array>
#include <cassert>
#include <buffers/general_buffer.hpp>
#include <memory>

#include "GL/glew.h"

class GeneralBuffer;

/**
 * @enum VAOSlot
 * @brief Enum representing the different slots available in a VAO (Vertex Array
 * Object).
 */
enum class VAOSlot : uint8_t {
  kPositions = 0, /**< Slot for position data. */
  kColors,        /**< Slot for color data. */
  kNormals,       /**< Slot for normal vector data. */
  kTextures,      /**< Slot for texture coordinate data. */
  kIndices,       /**< Slot for index data. */
  SLOTS,          /**< Total number of slots. */
};

/**
 * @brief The total number of VAO slots as a constant size value.
 */
constexpr size_t kVAOSlotSize = static_cast<size_t>(VAOSlot::SLOTS);

/**
 * @class VAOBuffer
 * @brief Represents a Vertex Array Object (VAO) buffer for managing OpenGL
 * buffer bindings.
 *
 * This class provides functionality to bind, unbind, and manage buffers in
 * specific VAO slots. It ensures proper resource management and prevents
 * copying or moving of the VAOBuffer object.
 */
class VAOBuffer {
 public:
  static std::unique_ptr<VAOBuffer> create() {
    return std::make_unique<VAOBuffer>();
  }

  /**
   * @brief Quickly binds the VAO for use in the OpenGL context.
   */
  void quickBind() const;

  /**
   * @brief Fully binds the VAO, ensuring all associated buffers are bound.
   *
   * This method isn't meant to be used, as quickBind() is sufficient for most
   * cases. Basically binds the VAO and will force all the slots to be bound.
   */
  void fullBind() const;

  /**
   * @brief Unbinds the VAO from the OpenGL context.
   */
  static void unbind();

  /**
   * @brief Activates a specific slot in the VAO.
   * @param slot The slot to activate.
   */
  void activateSlot(VAOSlot slot) const;

  /**
   * @brief Deactivates a specific slot in the VAO.
   * @param slot The slot to deactivate.
   */
  void deactivateSlot(VAOSlot slot) const;

  /**
   * @brief Binds a buffer to a specific slot in the VAO.
   * @param slot The slot to bind the buffer to.
   * @param buffer The buffer to bind.
   * @param dataIndex The index of the data in the buffer.
   */
  void bindSlot(VAOSlot slot, const std::shared_ptr<GeneralBuffer>& buffer,
                size_t dataIndex);

  /**
   * @brief Unbinds the buffer from a specific slot in the VAO.
   * @param slot The slot to unbind the buffer from.
   */
  void unbindSlot(VAOSlot slot);

  /**
   * @brief Retrieves the buffer bound to a specific slot.
   * @param slot The slot to retrieve the buffer from.
   * @return A weak pointer to the buffer at the specified slot.
   */
  [[nodiscard]] std::weak_ptr<GeneralBuffer> getBufferAt(VAOSlot slot) const;

  // disable copying
  VAOBuffer(VAOBuffer const &buffer) = delete;
  VAOBuffer &operator=(VAOBuffer const &buffer) = delete;

  // disable moving
  VAOBuffer(VAOBuffer &&buffer) noexcept ;
  VAOBuffer &operator=(VAOBuffer &&buffer) noexcept;

  ~VAOBuffer();

  VAOBuffer();
 private:

  /**
  * @brief Validates the specified slot for the given VAOBuffer.
  * @param buffer The VAOBuffer to validate.
  * @param slot The slot to validate.
  * @return The validated slot index.
  */
  static size_t validateSlot(VAOBuffer const &buffer, VAOSlot slot);

  GLuint m_vao{}; /**< OpenGL ID for the Vertex Array Object. */
  std::array<std::shared_ptr<GeneralBuffer>, kVAOSlotSize> m_register; /**< Array of buffers bound to VAO slots. */
};

#endif  // VAO_BUFFER_H