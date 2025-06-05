#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include <buffers/vao_buffer.hpp>
#include <map>
#include <memory>

struct BufferDataSource;
class GeneralBuffer;
class VAOBuffer;

/**
 * @class BufferManager
 * @brief Manages the creation, registration, and retrieval of buffers and VAOs.
 *
 * This class provides functionality to manage Vertex Buffer Objects (VBOs),
 * Element Buffer Objects (EBOs), and Vertex Array Objects (VAOs). It allows
 * for the creation, registration, retrieval, and cleanup of these resources.
 */
class BufferManager {
 public:
  BufferManager() : m_buffers() {};

  /**
   * @brief Releases a buffer associated with the given key.
   * @param key The key identifying the buffer to be released.
   */
  void releaseBuffer(std::string const& key);

  /**
   * @brief Creates a Vertex Buffer Object (VBO) and registers it with the given
   * key.
   * @param key The key to associate with the created VBO.
   * @param source The data source for the buffer.
   * @param usage The OpenGL usage hint for the buffer (e.g., GL_STATIC_DRAW).
   * @return A shared pointer to the created GeneralBuffer.
   */
  std::shared_ptr<GeneralBuffer> createVBO(std::string const& key,
                                           const BufferDataSource& source,
                                           GLenum usage);
  /**
   * @brief Creates an Element Buffer Object (EBO) and registers it with the
   * given key.
   * @param key The key to associate with the created EBO.
   * @param source The data source for the buffer.
   * @param usage The OpenGL usage hint for the buffer (e.g., GL_STATIC_DRAW).
   * @return A shared pointer to the created GeneralBuffer.
   */
  std::shared_ptr<GeneralBuffer> createEBO(std::string const& key,
                                           const BufferDataSource& source,
                                           GLenum usage);
  /**
   * @brief Checks if a buffer is registered with the given key.
   * @param key The key to check for buffer registration.
   * @return True if the buffer is registered, false otherwise.
   */
  bool isBufferRegistered(std::string const& key) const;

  /**
   * @brief Retrieves a buffer associated with the given key.
   * @param key The key identifying the buffer to retrieve.
   * @return A shared pointer to the requested GeneralBuffer, or nullptr if not
   * found.
   */
  std::shared_ptr<GeneralBuffer> getBuffer(std::string const& key);

  /**
   * @brief Registers a Vertex Array Object (VAO) with the given key.
   * @param key The key to associate with the VAO.
   * @param vao A unique pointer to the VAO to be registered.
   */
  void registerVAO(std::string const& key, std::unique_ptr<VAOBuffer> vao);

  /**
   * @brief Checks if a VAO is registered with the given key.
   * @param key The key to check for VAO registration.
   * @return True if the VAO is registered, false otherwise.
   */
  bool isVAORegistered(std::string const& key) const;

  /**
   * @brief Retrieves a VAO associated with the given key.
   * @param key The key identifying the VAO to retrieve.
   * @return A shared pointer to the requested VAOBuffer, or nullptr if not
   * found.
   */
  std::shared_ptr<VAOBuffer> getVAO(std::string const& key);

  /**
   * @brief Releases a VAO associated with the given key.
   * @param key The key identifying the VAO to be released.
   */
  void releaseVAO(std::string const& key);

  ~BufferManager() {
    m_buffers.clear();
    m_vaos.clear();
  }

 private:
  /**
   * @brief Cleans up all registered VAOs.
   */
  void cleanupVAOS();

  /**
   * @brief Cleans up all registered buffers.
   */
  void cleanupBuffers();

  std::map<std::string const, std::shared_ptr<GeneralBuffer>> m_buffers; /**< Map of buffer keys to GeneralBuffer shared pointers. */
  std::map<std::string const, std::shared_ptr<VAOBuffer>> m_vaos; /**< Map of VAO keys to VAOBuffer shared pointers. */
};

#endif  // BUFFER_MANAGER_H
