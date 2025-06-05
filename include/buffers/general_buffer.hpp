#ifndef GENERAL_BUFFER_H
#define GENERAL_BUFFER_H

#include <GL/glew.h>

#include <managers/buffer_manager.hpp>
#include <cstddef>
#include <vector>
#include <memory>

/**
 * @enum ComponentSize
 * @brief Represents the number of components in a data element.
 */
enum class ComponentSize : uint8_t {
  kZero = 0,
  kOne = 1,
  kTwo = 2,
  kThree = 3,
  kFour = 4
};

/**
 * @enum Stride
 * @brief Represents the stride (byte offset between consecutive elements).
 */
enum Stride : size_t {};

/**
 * @enum Offset
 * @brief Represents the offset (byte position within a buffer).
 */
enum Offset : size_t {};

/**
 * @struct DataBufferMapping
 * @brief Represents a mapping of data in a buffer.
 *
 * This structure defines the type and size in a buffer.
 */
struct DataBufferMapping {
  GLenum type;        /**< OpenGL data type (e.g., GL_FLOAT). */
  ComponentSize size; /**< Number of components in the data element. */
};

/**
 * @struct MappedDataBuffer
 * @brief Represents a mapping of data in a buffer.
 *
 * This structure defines the type, size, and offset of data in a buffer.
 */
struct MappedDataBuffer {
  GLenum type;        /**< OpenGL data type (e.g., GL_FLOAT). */
  ComponentSize size; /**< Number of components in the data element. */
  Offset offset;      /**< Byte offset of the data element in the buffer. */
};

/**
 * @struct BufferDataSource
 * @brief Represents the source data for a buffer.
 */
struct BufferDataSource {
  void const* ptr;                        /**< Pointer to the data source. */
  size_t size;                            /**< Size of the data in bytes. */
  std::vector<DataBufferMapping> mapping; /**< Mapping of data elements. */
};

/**
 * @class GeneralBuffer
 * @brief Represents a general-purpose OpenGL buffer.
 *
 * This class provides functionality for managing OpenGL buffers,
 * including binding, unbinding, and accessing buffer properties.
 */
class GeneralBuffer {
 public:
  /**
   * @brief Gets the OpenGL buffer ID.
   * @return The buffer ID.
   */
  [[nodiscard]] GLuint getId() const { return m_buffer; }

  /**
   * @brief Gets the OpenGL buffer type.
   * @return The buffer type (e.g., GL_ARRAY_BUFFER).
   */
  [[nodiscard]] GLenum getType() const { return m_bufferType; }

  /**
   * @brief Gets the OpenGL buffer type.
   * @return The buffer type (e.g., GL_ARRAY_BUFFER).
   */
  [[nodiscard]] MappedDataBuffer const& getDataMapAt(size_t index) const;

  /**
   * @brief Gets the size of the buffer.
   * @return The size of the buffer in bytes.
   */
  [[nodiscard]] std::size_t getSize() const { return m_size; };

  /**
   * @brief Gets the stride of the buffer.
   * @return The stride of the buffer.
   */
  [[nodiscard]] Stride getStride() const { return m_stride; };

  /**
   * @brief Binds the buffer to the current OpenGL context.
   */
  void bind() const;

  /**
   * @brief Unbinds the buffer from the current OpenGL context.
   */
  void unbind() const;

  ~GeneralBuffer();

  GeneralBuffer(const BufferDataSource& source, GLenum m_bufferType,
                GLenum usage);

  /**
   * @brief Creates an Element Buffer Object (EBO).
   * @param src The data source for the buffer.
   * @param usage The OpenGL usage pattern (e.g., GL_STATIC_DRAW).
   * @return A GeneralBuffer representing the EBO.
   */
  static std::shared_ptr<GeneralBuffer> createEBO(const BufferDataSource& src, GLenum usage);

  /**
   * @brief Creates a Vertex Buffer Object (VBO).
   * @param src The data source for the buffer.
   * @param usage The OpenGL usage pattern (e.g., GL_STATIC_DRAW).
   * @return A GeneralBuffer representing the VBO.
   */
  static std::shared_ptr<GeneralBuffer> createVBO(const BufferDataSource& src, GLenum usage);

private:
  GLuint m_buffer{};                                    /**< OpenGL buffer ID. */
  GLenum m_bufferType;                                /**< OpenGL buffer type. */
  std::vector<MappedDataBuffer> m_datatypes;   /**< Data type mappings. */
  GLenum m_usage;                                     /**< OpenGL usage pattern. */
  std::size_t const m_size;                           /**< Size of the buffer in bytes. */
  Stride m_stride;                                    /**< Stride of the buffer. */

  friend class BufferManager;
};

#endif  // GENERAL_BUFFER_H