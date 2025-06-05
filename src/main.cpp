#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "camera.h"
#include "texture.h"

/// constants for the camera
const float FOV = 45.0f;
const float nearPlane = 0.1f;
const float farPlane = 100.0f;

const unsigned int width = 800;
const unsigned int height = 800;

const float faces = 32.0f;
const float radius = 1.0f;
const float lightFaces = 32.0f;
const float lightRadius = 0.25f;

// use left mouse button to interact with the camera
// use z, q, d, d to move the camera
// use left control to move down
// use space to move up

// Vertices coordinates
GLfloat vertices[32 * 32 * 11]; 
GLfloat lightVertices[32 * 32 * 11]; 

// Indices for vertices order
GLuint indices[32 * 32 * 6]; 
GLuint lightIndices[32 * 32 * 6];

void generateSphere(float radius, int faces, GLfloat* vertices, GLuint* indices) {
    int vertexIndex = 0;
    int indexIndex = 0;

    for (int i = 0; i <= faces; ++i) {
        float theta = i * M_PI / faces; // Latitude angle
        for (int j = 0; j <= faces; ++j) {
            float phi = j * 2 * M_PI / faces; // Longitude angle

            // Calculate vertex position
            float x = radius * sin(theta) * cos(phi);
            float y = radius * cos(theta);
            float z = radius * sin(theta) * sin(phi);

            // Add position
            vertices[vertexIndex++] = x;
            vertices[vertexIndex++] = y;
            vertices[vertexIndex++] = z;

            // Add color (example: white)
            vertices[vertexIndex++] = 1.0f;
            vertices[vertexIndex++] = 1.0f;
            vertices[vertexIndex++] = 1.0f;

            // Add texture coordinates
            vertices[vertexIndex++] = (float)j / faces;
            vertices[vertexIndex++] = (float)i / faces;

            // Add normals (same as position for a sphere)
            vertices[vertexIndex++] = x / radius;
            vertices[vertexIndex++] = y / radius;
            vertices[vertexIndex++] = z / radius;
        }
    }

    for (int i = 0; i < faces; ++i) {
        for (int j = 0; j < faces; ++j) {
            int first = i * (faces + 1) + j;
            int second = first + faces + 1;

            // First triangle
            indices[indexIndex++] = first;
            indices[indexIndex++] = second;
            indices[indexIndex++] = first + 1;

            // Second triangle
            indices[indexIndex++] = second;
            indices[indexIndex++] = second + 1;
            indices[indexIndex++] = first + 1;
        }
    }
}

int main(){

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    

    GLFWwindow* window = glfwCreateWindow(width,height,"TP4 - Julien TAP",NULL,NULL);
    if(window==NULL){
        std::cout<<"failed creating the window"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!glewInit()){
        perror("failed init");
    }

    glViewport(0,0,width,height);

    // Generate sphere vertices and indices
    generateSphere(radius, faces, vertices, indices);
	generateSphere(lightRadius, lightFaces, lightVertices, lightIndices);
	
	// Vertices for a textured cube (room)
float roomVertices[] = {
    //  Position           Color         TexCoords   Normals
    // Face arrière
    -3.5f, -1.0f, -3.5f,  1,1,1,   0,0,   0,0,-1,
     3.5f, -1.0f, -3.5f,  1,1,1,   1,0,   0,0,-1,
     3.5f,  1.0f, -3.5f,  1,1,1,   1,1,   0,0,-1,
    -3.5f,  1.0f, -3.5f,  1,1,1,   0,1,   0,0,-1,
    // Face avant
    -3.5f, -1.0f,  3.5f,  1,1,1,   0,0,   0,0,1,
     3.5f, -1.0f,  3.5f,  1,1,1,   1,0,   0,0,1,
     3.5f,  1.0f,  3.5f,  1,1,1,   1,1,   0,0,1,
    -3.5f,  1.0f,  3.5f,  1,1,1,   0,1,   0,0,1,
    // Face gauche
    -3.5f, -1.0f, -3.5f,  1,1,1,   0,0,   -1,0,0,
    -3.5f,  1.0f, -3.5f,  1,1,1,   1,0,   -1,0,0,
    -3.5f,  1.0f,  3.5f,  1,1,1,   1,1,   -1,0,0,
    -3.5f, -1.0f,  3.5f,  1,1,1,   0,1,   -1,0,0,
    // Face droite
     3.5f, -1.0f, -3.5f,  1,1,1,   0,0,   1,0,0,
     3.5f,  1.0f, -3.5f,  1,1,1,   1,0,   1,0,0,
     3.5f,  1.0f,  3.5f,  1,1,1,   1,1,   1,0,0,
     3.5f, -1.0f,  3.5f,  1,1,1,   0,1,   1,0,0,
    // Face bas
    -3.5f, -1.0f, -3.5f,  1,1,1,   0,0,   0,-1,0,
     3.5f, -1.0f, -3.5f,  1,1,1,   1,0,   0,-1,0,
     3.5f, -1.0f,  3.5f,  1,1,1,   1,1,   0,-1,0,
    -3.5f, -1.0f,  3.5f,  1,1,1,   0,1,   0,-1,0,
    // Face haut
    -3.5f,  1.0f, -3.5f,  1,1,1,   0,0,   0,1,0,
     3.5f,  1.0f, -3.5f,  1,1,1,   1,0,   0,1,0,
     3.5f,  1.0f,  3.5f,  1,1,1,   1,1,   0,1,0,
    -3.5f,  1.0f,  3.5f,  1,1,1,   0,1,   0,1,0,
};
float roomFrontCapVertices[] = {
    // Bouchon gauche
    -3.5f, -1.0f, 3.5f,  1,1,1,  0,0,  0,0,1,
    -0.5f, -1.0f, 3.5f,  1,1,1,  1,0,  0,0,1,
    -0.5f,  1.0f, 3.5f,  1,1,1,  1,1,  0,0,1,
    -3.5f,  1.0f, 3.5f,  1,1,1,  0,1,  0,0,1,
    // Bouchon droite
     0.5f, -1.0f, 3.5f,  1,1,1,  0,0,  0,0,1,
     3.5f, -1.0f, 3.5f,  1,1,1,  1,0,  0,0,1,
     3.5f,  1.0f, 3.5f,  1,1,1,  1,1,  0,0,1,
     0.5f,  1.0f, 3.5f,  1,1,1,  0,1,  0,0,1,
};
unsigned int roomFrontCapIndices[] = {
    0,1,2, 2,3,0, // bouchon gauche
    4,5,6, 6,7,4  // bouchon droite
};
unsigned int roomIndices[] = {
    0,1,2,  2,3,0,      // back
    //4,5,6,  6,7,4,      // front
    8,9,10, 10,11,8,    // left
    12,13,14, 14,15,12, // right
    16,17,18, 18,19,16, // bottom
    20,21,22, 22,23,20  // top
};

// Couloir (par exemple, largeur 1, hauteur 2, profondeur 4, partant du centre de la face avant de la salle)
float corridorVertices[] = {
    //  Position           Color         TexCoords   Normals
    // Face arrière (côté salle carrée, z = 3.5)
    -0.5f, -1.0f,  3.5f,  1,1,1,   0,0,   0,0,-1,
     0.5f, -1.0f,  3.5f,  1,1,1,   1,0,   0,0,-1,
     0.5f,  1.0f,  3.5f,  1,1,1,   1,1,   0,0,-1,
    -0.5f,  1.0f,  3.5f,  1,1,1,   0,1,   0,0,-1,
    // Face avant (côté salle rectangulaire, z = 6.0)
    -0.5f, -1.0f,  6.0f,  1,1,1,   0,0,   0,0,1,
     0.5f, -1.0f,  6.0f,  1,1,1,   1,0,   0,0,1,
     0.5f,  1.0f,  6.0f,  1,1,1,   1,1,   0,0,1,
    -0.5f,  1.0f,  6.0f,  1,1,1,   0,1,   0,0,1,
    // Face gauche
    -0.5f, -1.0f,  3.5f,  1,1,1,   0,0,   -1,0,0,
    -0.5f,  1.0f,  3.5f,  1,1,1,   1,0,   -1,0,0,
    -0.5f,  1.0f,  6.0f,  1,1,1,   1,1,   -1,0,0,
    -0.5f, -1.0f,  6.0f,  1,1,1,   0,1,   -1,0,0,
    // Face droite
     0.5f, -1.0f,  3.5f,  1,1,1,   0,0,   1,0,0,
     0.5f,  1.0f,  3.5f,  1,1,1,   1,0,   1,0,0,
     0.5f,  1.0f,  6.0f,  1,1,1,   1,1,   1,0,0,
     0.5f, -1.0f,  6.0f,  1,1,1,   0,1,   1,0,0,
    // Face bas
    -0.5f, -1.0f,  3.5f,  1,1,1,   0,0,   0,-1,0,
     0.5f, -1.0f,  3.5f,  1,1,1,   1,0,   0,-1,0,
     0.5f, -1.0f,  6.0f,  1,1,1,   1,1,   0,-1,0,
    -0.5f, -1.0f,  6.0f,  1,1,1,   0,1,   0,-1,0,
    // Face haut
    -0.5f,  1.0f,  3.5f,  1,1,1,   0,0,   0,1,0,
     0.5f,  1.0f,  3.5f,  1,1,1,   1,0,   0,1,0,
     0.5f,  1.0f,  6.0f,  1,1,1,   1,1,   0,1,0,
    -0.5f,  1.0f,  6.0f,  1,1,1,   0,1,   0,1,0,
};

unsigned int corridorIndices[] = {
    //0,1,2,  2,3,0,      // back
    //4,5,6,  6,7,4,      // front
    8,9,10, 10,11,8,    // left
    12,13,14, 14,15,12, // right
    16,17,18, 18,19,16, // bottom
    20,21,22, 22,23,20  // top
};

// Salle rectangulaire (par exemple, largeur 3, hauteur 2, profondeur 4, centrée en z=8)
float room2Vertices[] = {
    //  Position           Color         TexCoords   Normals
    // Face arrière
    -3.0f, -1.0f,  6.0f,  1,1,1,   0,0,   0,0,-1,
     3.0f, -1.0f,  6.0f,  1,1,1,   1,0,   0,0,-1,
     3.0f,  1.0f,  6.0f,  1,1,1,   1,1,   0,0,-1,
    -3.0f,  1.0f,  6.0f,  1,1,1,   0,1,   0,0,-1,
    // Face avant
    -3.0f, -1.0f, 10.0f,  1,1,1,   0,0,   0,0,1,
     3.0f, -1.0f, 10.0f,  1,1,1,   1,0,   0,0,1,
     3.0f,  1.0f, 10.0f,  1,1,1,   1,1,   0,0,1,
    -3.0f,  1.0f, 10.0f,  1,1,1,   0,1,   0,0,1,
    // Face gauche
    -3.0f, -1.0f,  6.0f,  1,1,1,   0,0,   -1,0,0,
    -3.0f,  1.0f,  6.0f,  1,1,1,   1,0,   -1,0,0,
    -3.0f,  1.0f, 10.0f,  1,1,1,   1,1,   -1,0,0,
    -3.0f, -1.0f, 10.0f,  1,1,1,   0,1,   -1,0,0,
    // Face droite
     3.0f, -1.0f,  6.0f,  1,1,1,   0,0,   1,0,0,
     3.0f,  1.0f,  6.0f,  1,1,1,   1,0,   1,0,0,
     3.0f,  1.0f, 10.0f,  1,1,1,   1,1,   1,0,0,
     3.0f, -1.0f, 10.0f,  1,1,1,   0,1,   1,0,0,
    // Face bas
    -3.0f, -1.0f,  6.0f,  1,1,1,   0,0,   0,-1,0,
     3.0f, -1.0f,  6.0f,  1,1,1,   1,0,   0,-1,0,
     3.0f, -1.0f, 10.0f,  1,1,1,   1,1,   0,-1,0,
    -3.0f, -1.0f, 10.0f,  1,1,1,   0,1,   0,-1,0,
    // Face haut
    -3.0f,  1.0f,  6.0f,  1,1,1,   0,0,   0,1,0,
     3.0f,  1.0f,  6.0f,  1,1,1,   1,0,   0,1,0,
     3.0f,  1.0f, 10.0f,  1,1,1,   1,1,   0,1,0,
    -3.0f,  1.0f, 10.0f,  1,1,1,   0,1,   0,1,0,
};
float room2BackCapVertices[] = {
    // Bouchon gauche
    -3.0f, -1.0f, 6.0f,  1,1,1,  0,0,  0,0,-1,
    -0.5f, -1.0f, 6.0f,  1,1,1,  1,0,  0,0,-1,
    -0.5f,  1.0f, 6.0f,  1,1,1,  1,1,  0,0,-1,
    -3.0f,  1.0f, 6.0f,  1,1,1,  0,1,  0,0,-1,
    // Bouchon droite
     0.5f, -1.0f, 6.0f,  1,1,1,  0,0,  0,0,-1,
     3.0f, -1.0f, 6.0f,  1,1,1,  1,0,  0,0,-1,
     3.0f,  1.0f, 6.0f,  1,1,1,  1,1,  0,0,-1,
     0.5f,  1.0f, 6.0f,  1,1,1,  0,1,  0,0,-1,
};
unsigned int room2BackCapIndices[] = {
    0,1,2, 2,3,0, // bouchon gauche
    4,5,6, 6,7,4  // bouchon droite
};
unsigned int room2Indices[] = {
    //0,1,2,  2,3,0,      // back
    4,5,6,  6,7,4,      // front
    8,9,10, 10,11,8,    // left
    12,13,14, 14,15,12, // right
    16,17,18, 18,19,16, // bottom
    20,21,22, 22,23,20  // top
};

    // Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("./shaders/default.vert.txt", "./shaders/default.frag.txt");
	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));
	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	VAO corridorVAO;
corridorVAO.Bind();
VBO corridorVBO(corridorVertices, sizeof(corridorVertices));
EBO corridorEBO(corridorIndices, sizeof(corridorIndices));
corridorVAO.LinkAttrib(corridorVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
corridorVAO.LinkAttrib(corridorVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
corridorVAO.LinkAttrib(corridorVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
corridorVAO.LinkAttrib(corridorVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
corridorVAO.Unbind();
corridorVBO.Unbind();
corridorEBO.Unbind();

VAO roomFrontCapVAO;
roomFrontCapVAO.Bind();
VBO roomFrontCapVBO(roomFrontCapVertices, sizeof(roomFrontCapVertices));
EBO roomFrontCapEBO(roomFrontCapIndices, sizeof(roomFrontCapIndices));
roomFrontCapVAO.LinkAttrib(roomFrontCapVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
roomFrontCapVAO.LinkAttrib(roomFrontCapVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
roomFrontCapVAO.LinkAttrib(roomFrontCapVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
roomFrontCapVAO.LinkAttrib(roomFrontCapVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
roomFrontCapVAO.Unbind();
roomFrontCapVBO.Unbind();
roomFrontCapEBO.Unbind();

VAO room2VAO;
room2VAO.Bind();
VBO room2VBO(room2Vertices, sizeof(room2Vertices));
EBO room2EBO(room2Indices, sizeof(room2Indices));
room2VAO.LinkAttrib(room2VBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
room2VAO.LinkAttrib(room2VBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
room2VAO.LinkAttrib(room2VBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
room2VAO.LinkAttrib(room2VBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
room2VAO.Unbind();
room2VBO.Unbind();
room2EBO.Unbind();

VAO room2BackCapVAO;
room2BackCapVAO.Bind();
VBO room2BackCapVBO(room2BackCapVertices, sizeof(room2BackCapVertices));
EBO room2BackCapEBO(room2BackCapIndices, sizeof(room2BackCapIndices));
room2BackCapVAO.LinkAttrib(room2BackCapVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
room2BackCapVAO.LinkAttrib(room2BackCapVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
room2BackCapVAO.LinkAttrib(room2BackCapVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
room2BackCapVAO.LinkAttrib(room2BackCapVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
room2BackCapVAO.Unbind();
room2BackCapVBO.Unbind();
room2BackCapEBO.Unbind();
	// Génère VAO/VBO/EBO pour la salle
VAO roomVAO;
roomVAO.Bind();
VBO roomVBO(roomVertices, sizeof(roomVertices));
EBO roomEBO(roomIndices, sizeof(roomIndices));
roomVAO.LinkAttrib(roomVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);              // position
roomVAO.LinkAttrib(roomVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float))); // couleur
roomVAO.LinkAttrib(roomVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float))); // texcoords
roomVAO.LinkAttrib(roomVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float))); // normals
roomVAO.Unbind();
roomVBO.Unbind();
roomEBO.Unbind();

// Texture
Texture roomTex("./textures/texture1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
roomTex.texUnit(shaderProgram, "tex0", 0);

Texture metalTex("./textures/TexturesCom_Metal.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
metalTex.texUnit(shaderProgram, "tex0", 0);

	// Shader for light cube
	Shader lightShader("./shaders/light.vert.txt", "./shaders/light.frag.txt");
	// Generates Vertex Array Object and binds it
	// Generates Vertex Array Object and binds it
	VAO VAO2;
	VAO2.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO2(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO2(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO2.LinkAttrib(VBO2, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO2.LinkAttrib(VBO2, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(1.5f, 1.5f, 3.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Texture
	Texture sphereTex("./textures/texture1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	sphereTex.texUnit(shaderProgram, "tex0", 0);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 5.0f), FOV, nearPlane, farPlane);


    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(FOV, nearPlane, farPlane);


		shaderProgram.Activate();
glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
camera.Matrix(shaderProgram, "camMatrix");
roomTex.Bind();
roomVAO.Bind();
glDrawElements(GL_TRIANGLES, sizeof(roomIndices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

// Couloir
shaderProgram.Activate();
roomTex.Bind();
corridorVAO.Bind();
glDrawElements(GL_TRIANGLES, sizeof(corridorIndices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

// Salle rectangulaire
shaderProgram.Activate();
roomTex.Bind();
room2VAO.Bind();
glDrawElements(GL_TRIANGLES, sizeof(room2Indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

// Bouchons salle carrée
shaderProgram.Activate();
roomTex.Bind();
roomFrontCapVAO.Bind();
glDrawElements(GL_TRIANGLES, sizeof(roomFrontCapIndices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

// Bouchons salle rectangulaire
shaderProgram.Activate();
roomTex.Bind();
room2BackCapVAO.Bind();
glDrawElements(GL_TRIANGLES, sizeof(room2BackCapIndices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

		// Tells OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Export the camMatrix to the Vertex Shader of the pyramid
		camera.Matrix(shaderProgram, "camMatrix");
		// Binds texture so that is appears in rendering
		sphereTex.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);



		// Tells OpenGL which Shader Program we want to use
		lightShader.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		VAO2.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
    }

    // Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	sphereTex.Delete();
	shaderProgram.Delete();
	VAO2.Delete();
	VBO2.Delete();
	EBO2.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}