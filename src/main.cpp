#include <iostream>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "objects/object3D.h"
#include "renders/triangles_object.h"
#include "buffers/general_buffer.hpp"
#include "buffers/vao_buffer.hpp"
#include "light.h"
#include "camera.h"
#include "shaderClass.h"
#include "texture.h"
#include "VBO.h"

// constants
const unsigned int width = 800;
const unsigned int height = 800;
const float FOV = 45.0f;
const float nearPlane = 0.1f;
const float farPlane = 100.0f;

// Room dimensions
const float roomSize = 10.0f;

Vertex planeVertices[] =
    {
        Vertex{glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}};

GLuint planeIndices[] = {0, 1, 2, 0, 2, 3};

// Helper function to create a textured plane
std::unique_ptr<TrianglesObject> createTexturedPlane(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures)
{
    BufferDataSource vboSource;
    vboSource.ptr = vertices.data();
    vboSource.size = vertices.size() * sizeof(Vertex);
    vboSource.mapping = {
        {GL_FLOAT, ComponentSize::kThree}, {GL_FLOAT, ComponentSize::kThree},
        {GL_FLOAT, ComponentSize::kThree}, {GL_FLOAT, ComponentSize::kTwo}};

    BufferDataSource eboSource;
    eboSource.ptr = indices.data();
    eboSource.size = indices.size() * sizeof(GLuint);
    eboSource.mapping = {{GL_UNSIGNED_INT, ComponentSize::kOne}};

    auto vbo = GeneralBuffer::createVBO(vboSource, GL_STATIC_DRAW);
    auto ebo = GeneralBuffer::createEBO(eboSource, GL_STATIC_DRAW);

    auto vao = VAOBuffer::create();
    vao->quickBind();
    vao->bindSlot(VAOSlot::kPositions, vbo, 0);
    vao->bindSlot(VAOSlot::kColors, vbo, 1);
    vao->bindSlot(VAOSlot::kNormals, vbo, 2);
    vao->bindSlot(VAOSlot::kTextures, vbo, 3);
    vao->bindSlot(VAOSlot::kIndices, ebo, 0);
    vao->activateSlot(VAOSlot::kPositions);
    vao->activateSlot(VAOSlot::kColors);
    vao->activateSlot(VAOSlot::kNormals);
    vao->activateSlot(VAOSlot::kTextures);
    vao->unbind();

    return std::make_unique<TrianglesObject>(std::move(vao), vertices, textures);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, "Mortal Gemini", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    Shader shaderProgram("./shaders/default.vert", "./shaders/default.frag");
    Shader lightShader("./shaders/light.vert", "./shaders/light.frag");

    std::vector<Texture> roomTextures = {
        Texture("./textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("./textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)};

    std::vector<Vertex> planeVerts(planeVertices, planeVertices + sizeof(planeVertices) / sizeof(Vertex));
    std::vector<GLuint> planeInd(planeIndices, planeIndices + sizeof(planeIndices) / sizeof(GLuint));

    auto root = std::make_unique<object3D>();
    std::vector<Drawable*> collidableObjects;

    auto room = std::make_unique<object3D>();

    // *** THIS IS THE CORRECTED SECTION ***

    // Floor
    auto floor = createTexturedPlane(planeVerts, planeInd, roomTextures);
    glm::mat4 floorModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -roomSize / 2.0f, 0.0f)) *
                           glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize));
    floor->setLocalModelMatrix(floorModel);
    collidableObjects.push_back(floor.get());
    room->addObject(std::move(floor));

    // Ceiling
    auto ceiling = createTexturedPlane(planeVerts, planeInd, roomTextures);
    glm::mat4 ceilingModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, roomSize / 2.0f, 0.0f)) *
                             glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
                             glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize));
    ceiling->setLocalModelMatrix(ceilingModel);
    collidableObjects.push_back(ceiling.get());
    room->addObject(std::move(ceiling));

    // Walls
    // Back Wall (-Z) - This one was correct
    auto wall_back = createTexturedPlane(planeVerts, planeInd, roomTextures);
    glm::mat4 wallBackModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -roomSize / 2.0f)) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
                              glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize));
    wall_back->setLocalModelMatrix(wallBackModel);
    collidableObjects.push_back(wall_back.get());
    room->addObject(std::move(wall_back));

    // Front Wall (+Z) - This one was correct
    auto wall_front = createTexturedPlane(planeVerts, planeInd, roomTextures);
    glm::mat4 wallFrontModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, roomSize / 2.0f)) *
                               glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
                               glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize));
    wall_front->setLocalModelMatrix(wallFrontModel);
    collidableObjects.push_back(wall_front.get());
    room->addObject(std::move(wall_front));

    // *** THE FIX IS HERE ***

    auto wall_left = createTexturedPlane(planeVerts, planeInd, roomTextures);
    glm::mat4 wallLeftModel =
        glm::translate(glm::mat4(1.0f), glm::vec3(-roomSize / 2.0f, 0.0f, 0.0f)) * // 4. Move to the left
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * // 3. Rotate to face inward
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * // 2. Stand the plane up
        glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize));                   // 1. Scale it
    wall_left->setLocalModelMatrix(wallLeftModel);
    collidableObjects.push_back(wall_left.get());
    room->addObject(std::move(wall_left));

    // Right Wall (+X)
    auto wall_right = createTexturedPlane(planeVerts, planeInd, roomTextures);
    glm::mat4 wallRightModel =
        glm::translate(glm::mat4(1.0f), glm::vec3(roomSize / 2.0f, 0.0f, 0.0f)) * // 4. Move to the right
        glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * // 3. Rotate to face inward
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * // 2. Stand the plane up
        glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize));                  // 1. Scale it
    wall_right->setLocalModelMatrix(wallRightModel);
    collidableObjects.push_back(wall_right.get());
    room->addObject(std::move(wall_right));

    root->addObject(std::move(room));

    LightProperties lightProps;
    lightProps.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    auto lightObject = createTexturedPlane(planeVerts, planeInd, {});
    lightObject->scale(glm::vec3(0.2f, 0.2f, 0.2f));
    lightObject->setCollidable(false);
    Drawable* lightRawPtr = lightObject.get();

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f), FOV, nearPlane, farPlane);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window, collidableObjects, 1.0f / 60.0f);
        camera.updateMatrix(FOV, nearPlane, farPlane);

        lightProps.position = glm::vec3(2.0f * sin(glfwGetTime()), 1.5f, 2.0f * cos(glfwGetTime()));
        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightProps.position);

        shaderProgram.Activate();
        root->draw(shaderProgram, camera, lightProps, glm::mat4(1.0f));

        lightShader.Activate();
        lightRawPtr->setLocalModelMatrix(lightModel);
        lightRawPtr->draw(lightShader, camera, lightProps, glm::mat4(1.0f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderProgram.Delete();
    lightShader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
