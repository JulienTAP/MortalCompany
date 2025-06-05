#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "node.h"
#include "sphere.h"
#include "cylinder.h"

/// constants for the camera
const float FOV = 45.0f;
const float nearPlane = 0.1f;
const float farPlane = 100.0f;

const unsigned int width = 800;
const unsigned int height = 800;

// use left mouse button to interact with the camera
// use z, q, d, d to move the camera
// use left control to move down
// use space to move up

// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


int main(){

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    

    GLFWwindow* window = glfwCreateWindow(width,height,"TP 2",NULL,NULL);
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

    Shader color_shader	("./shaders/node.vert", "./shaders/node.frag");

// Matrice de base pour placer l'humain dans la scène
glm::mat4 human_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
    * glm::scale(glm::mat4(1.0f), glm::vec3(0.9f, 0.9f, 0.9f));
Node* human = new Node(human_mat);

// Tête
Sphere* head = new Sphere(&color_shader, 0.5f, 32, 32);
glm::mat4 mat_head = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.2f, 0.0f));
Node* head_node = new Node(mat_head);
head_node->add(head);
human->add(head_node);

// Tronc
Cylinder* trunk = new Cylinder(&color_shader, 1.2f, 0.4f, 32);
glm::mat4 mat_trunk = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
Node* trunk_node = new Node(mat_trunk);
trunk_node->add(trunk);
human->add(trunk_node);

// Bras gauche
Cylinder* left_arm = new Cylinder(&color_shader, 0.9f, 0.12f, 32);
glm::mat4 mat_left_arm = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 1.7f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(160.0f), glm::vec3(0.0f, 0.0f, 1.0f));
Node* left_arm_node = new Node(mat_left_arm);
left_arm_node->add(left_arm);
human->add(left_arm_node);

// Main gauche
Sphere* left_hand = new Sphere(&color_shader, 0.15f, 16, 16);
glm::mat4 mat_left_hand = glm::translate(glm::mat4(1.0f), glm::vec3(-0.95f, 1.2f, 0.0f));
Node* left_hand_node = new Node(mat_left_hand);
left_hand_node->add(left_hand);
human->add(left_hand_node);

// Bras droit
Cylinder* right_arm = new Cylinder(&color_shader, 0.9f, 0.12f, 32);
glm::mat4 mat_right_arm = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 1.7f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(-160.0f), glm::vec3(0.0f, 0.0f, 1.0f));
Node* right_arm_node = new Node(mat_right_arm);
right_arm_node->add(right_arm);
human->add(right_arm_node);

// Main droite
Sphere* right_hand = new Sphere(&color_shader, 0.15f, 16, 16);
glm::mat4 mat_right_hand = glm::translate(glm::mat4(1.0f), glm::vec3(0.95f, 1.2f, 0.0f));
Node* right_hand_node = new Node(mat_right_hand);
right_hand_node->add(right_hand);
human->add(right_hand_node);

// Jambe gauche
Cylinder* left_leg = new Cylinder(&color_shader, 1.0f, 0.15f, 32);
glm::mat4 mat_left_leg = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, 0.0f, 0.0f));
Node* left_leg_node = new Node(mat_left_leg);
left_leg_node->add(left_leg);
human->add(left_leg_node);

// Pied gauche
Sphere* left_foot = new Sphere(&color_shader, 0.18f, 16, 16);
glm::mat4 mat_left_foot = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, -0.9f, 0.0f));
Node* left_foot_node = new Node(mat_left_foot);
left_foot_node->add(left_foot);
human->add(left_foot_node);

// Jambe droite
Cylinder* right_leg = new Cylinder(&color_shader, 1.0f, 0.15f, 32);
glm::mat4 mat_right_leg = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, 0.0f, 0.0f));
Node* right_leg_node = new Node(mat_right_leg);
right_leg_node->add(right_leg);
human->add(right_leg_node);

// Pied droit
Sphere* right_foot = new Sphere(&color_shader, 0.18f, 16, 16);
glm::mat4 mat_right_foot = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, -0.9f, 0.0f));
Node* right_foot_node = new Node(mat_right_foot);
right_foot_node->add(right_foot);
human->add(right_foot_node);


	Texture textures[]{
		Texture("./textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

    // Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("./shaders/default.vert", "./shaders/default.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex, shaderProgram);


	// Shader for light cube
	Shader lightShader("./shaders/light.vert", "./shaders/light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Create light mesh
	Mesh light(lightVerts, lightInd, tex, lightShader);



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 floorPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 floorModel = glm::mat4(1.0f);
	floorModel = glm::translate(floorModel, floorPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Node *root = new Node();
	root->add(&floor);

	Node *lightNode = new Node(glm::translate(glm::mat4(1.0f), lightPos));
	lightNode->add(&light);

	root->add(lightNode);


	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f), FOV, nearPlane, farPlane);


    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window, {}, 1.0f / 60.0f);
		// Draw the root node (which now includes the human model)
				// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(FOV, nearPlane, farPlane);

		// Draw the root node
	
		root->draw(camera);
		


		lightPos = glm::vec3(0.5f * sin(glfwGetTime()), 0.5f, 0.5f * cos(glfwGetTime()));
		lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);

		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


		color_shader.Activate();
		human->draw(camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
    }

    // Delete all the objects we've created
	shaderProgram.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}