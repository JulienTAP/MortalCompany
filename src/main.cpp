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
const float FOV = 90.0f;
const float nearPlane = 0.1f;
const float farPlane = 100.0f;

// Room and Hall dimensions
const float roomSize = 10.0f;
const float hallWidth = roomSize / 3.0f;
const float hallHeight = roomSize / 3.0f;
const float hallLength = roomSize * 2.0f;


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

    std::vector<Texture> floorTextures = {
        Texture("./textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("./textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };
    std::vector<Texture> wallTextures = {
        Texture("./textures/murSalleSombre.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("./textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };
    std::vector<Texture> ceilingTextures = {
        Texture("./textures/plafondSalleSombre.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("./textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };

    std::vector<Vertex> planeVerts(planeVertices, planeVertices + sizeof(planeVertices) / sizeof(Vertex));
    std::vector<GLuint> planeInd(planeIndices, planeIndices + sizeof(planeIndices) / sizeof(GLuint));

    auto root = std::make_unique<object3D>();
    std::vector<Drawable*> collidableObjects;

    auto room = std::make_unique<object3D>();

    // Floor
    auto floor = createTexturedPlane(planeVerts, planeInd, floorTextures);
    floor->setLocalModelMatrix(
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -roomSize / 2.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize))
    );
    collidableObjects.push_back(floor.get());
    room->addObject(std::move(floor));

    // Ceiling
    auto ceiling = createTexturedPlane(planeVerts, planeInd, ceilingTextures);
    ceiling->setLocalModelMatrix(
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, roomSize / 2.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize))
    );
    collidableObjects.push_back(ceiling.get());
    room->addObject(std::move(ceiling));

    // Back Wall (-Z)
    auto wall_back = createTexturedPlane(planeVerts, planeInd, wallTextures);
    wall_back->setLocalModelMatrix(
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -roomSize / 2.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize))
    );
    collidableObjects.push_back(wall_back.get());
    room->addObject(std::move(wall_back));

    // Front Wall (+Z)
    auto wall_front = createTexturedPlane(planeVerts, planeInd, wallTextures);
    wall_front->setLocalModelMatrix(
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, roomSize / 2.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize))
    );
    collidableObjects.push_back(wall_front.get());
    room->addObject(std::move(wall_front));

    // Left Wall (-X)
    auto wall_left = createTexturedPlane(planeVerts, planeInd, wallTextures);
    wall_left->setLocalModelMatrix(
        glm::translate(glm::mat4(1.0f), glm::vec3(-roomSize / 2.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(roomSize, 1.0f, roomSize))
    );
    collidableObjects.push_back(wall_left.get());
    room->addObject(std::move(wall_left));

    // Right Wall (+X) has been removed to make an opening for the hall

    root->addObject(std::move(room));

    // --- Hall Construction ---
    auto hall = std::make_unique<object3D>();
    // The hall now uses the full roomHeight, but keeps its previous width and length
    const float hallHeight = roomSize;

    // Hall Floor
    auto hall_floor = createTexturedPlane(planeVerts, planeInd, floorTextures);
    hall_floor->setLocalModelMatrix(
        // Move to the floor level of the room, and position it just outside the right wall
        glm::translate(glm::mat4(1.0f), glm::vec3(roomSize / 2.0f + hallLength / 2.0f, -roomSize / 2.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(hallLength, 1.0f, hallWidth))
    );
    collidableObjects.push_back(hall_floor.get());
    hall->addObject(std::move(hall_floor));

    // Hall Ceiling
    auto hall_ceiling = createTexturedPlane(planeVerts, planeInd, ceilingTextures);
    hall_ceiling->setLocalModelMatrix(
        // Move to the ceiling level of the room
        glm::translate(glm::mat4(1.0f), glm::vec3(roomSize / 2.0f + hallLength / 2.0f, roomSize / 2.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(hallLength, 1.0f, hallWidth))
    );
    collidableObjects.push_back(hall_ceiling.get());
    hall->addObject(std::move(hall_ceiling));

    // Hall Left Wall
    auto hall_wall_left = createTexturedPlane(planeVerts, planeInd, wallTextures);
    hall_wall_left->setLocalModelMatrix(
        // Positioned along the length of the hall at the correct Z offset
        glm::translate(glm::mat4(1.0f), glm::vec3(roomSize / 2.0f + hallLength / 2.0f, 0.0f, -hallWidth / 2.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        // Scale to be the correct length and height
        glm::scale(glm::mat4(1.0f), glm::vec3(hallLength, 1.0f, hallHeight))
    );
    collidableObjects.push_back(hall_wall_left.get());
    hall->addObject(std::move(hall_wall_left));

    // Hall Right Wall
    auto hall_wall_right = createTexturedPlane(planeVerts, planeInd, wallTextures);
    hall_wall_right->setLocalModelMatrix(
        glm::translate(glm::mat4(1.0f), glm::vec3(roomSize / 2.0f + hallLength / 2.0f, 0.0f, hallWidth / 2.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(hallLength, 1.0f, hallHeight))
    );
    collidableObjects.push_back(hall_wall_right.get());
    hall->addObject(std::move(hall_wall_right));

    // Hall Back Wall (to "close the box")
    auto hall_wall_back = createTexturedPlane(planeVerts, planeInd, wallTextures);
    hall_wall_back->setLocalModelMatrix(
        // Position at the very end of the hall
        glm::translate(glm::mat4(1.0f), glm::vec3(roomSize / 2.0f + hallLength, 0.0f, 0.0f)) *
        // Rotate it to be a vertical wall facing inwards
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        // Scale it to the height and width of the hall
        glm::scale(glm::mat4(1.0f), glm::vec3(hallWidth, 1.0f, hallHeight))
    );
    collidableObjects.push_back(hall_wall_back.get());
    hall->addObject(std::move(hall_wall_back));

    root->addObject(std::move(hall));


    // --- Light and Camera Setup ---
    LightProperties lightProps;
    lightProps.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    auto lightObject = createTexturedPlane(planeVerts, planeInd, {});
    lightObject->scale(glm::vec3(0.2f, 0.2f, 0.2f));
    lightObject->setCollidable(false);
    Drawable* lightRawPtr = lightObject.get();

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f), FOV, nearPlane, farPlane);

    // --- Main Loop ---
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