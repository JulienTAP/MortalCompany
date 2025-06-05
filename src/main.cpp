#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "node.h"
#include "elements.h"

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

	// Shaders
	Shader shaderProgram("./shaders/default.vert", "./shaders/default.frag");
	Shader lightShader("./shaders/light.vert", "./shaders/light.frag");	


	// Textures 
	Texture MainFloorTexture[2] = {
		Texture("./textures/solSalleEclairee_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/solSalleEclairee_normal.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) // or GL_RGBA if your specular is RGBA
	};
	Texture MainWallTexture[2] = {
		Texture("./textures/murSalleEclairee_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/murSalleEclairee_normal.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) // or GL_RGBA if your specular is RGBA
	};
	Texture MainCeilingTexture[2] = {
		Texture("./textures/plafondSalleEclairee_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/plafondSalleEclairee_normal.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) // or GL_RGBA if your specular is RGBA
	};

	Texture CorridorFloorTexture[2] = {
		Texture("./textures/solPasserelle_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/solPasserelle_normal.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) // or GL_RGBA if your specular is RGBA
	};

	Texture CorridorWallTexture[2] = {
		Texture("./textures/murSalleEclairee_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/murSalleEclairee_normal.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) // or GL_RGBA if your specular is RGBA
	};

	Texture CorridorCeilingTexture[2] = {
		Texture("./textures/plafondSalleEclairee_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/plafondSalleEclairee_normal.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) // or GL_RGBA if your specular is RGBA
	};


	Texture Room2FloorTexture[2] = {
		Texture("./textures/solSalleSombre_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/solSalleSombre_normal.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) // or GL_RGBA if your specular is RGBA
	};
	Texture Room2WallTexture[2] = {
		Texture("./textures/murSalleSombre_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/murSalleSombre_normal.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) // or GL_RGBA if your specular is RGBA
	};
	Texture Room2CeilingTexture[2] = {
		Texture("./textures/plafondSalleSombre_albedo.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/plafondSalleSombre_normal.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) // or GL_RGBA if your specular is RGBA
	};

	// Meshes
	std::vector<Vertex> MFV(MainFloorVerticies, MainFloorVerticies + 4);
	std::vector<GLuint> MFI(MainFloorIndicies, MainFloorIndicies + 6);

	std::vector<Vertex> MWV(MainWallVertices, MainWallVertices + 16);
	std::vector<GLuint> MWI(MainWallIndices, MainWallIndices + 24);

	std::vector<Vertex> MCV(MainCeilingVertices, MainCeilingVertices + 4);
	std::vector<GLuint> MCI(MainCeilingIndices, MainCeilingIndices + 6);

	std::vector<Vertex> CFV(corridorFloorVertices, corridorFloorVertices + 4);
	std::vector<GLuint> CFI(corridorFloorIndices, corridorFloorIndices + 6);

	std::vector<Vertex> CWV(corridorWallVertices, corridorWallVertices + 16);
	std::vector<GLuint> CWI(corridorWallIndices, corridorWallIndices + 24);

	std::vector<Vertex> CCV(corridorCeilingVertices, corridorCeilingVertices + 4);
	std::vector<GLuint> CCI(corridorCeilingIndices, corridorCeilingIndices + 6);

	std::vector<Vertex> R2FV(room2FloorVertices, room2FloorVertices + 4);
	std::vector<GLuint> R2FI(room2FloorIndices, room2FloorIndices + 6);

	std::vector<Vertex> R2WV(room2WallVertices, room2WallVertices + 16);
	std::vector<GLuint> R2WI(room2WallIndices, room2WallIndices + 24);

	std::vector<Vertex> R2CV(room2CeilingVertices, room2CeilingVertices + 4);
	std::vector<GLuint> R2CI(room2CeilingIndices, room2CeilingIndices + 6);

	// --- Front Cap (Avant) ---
	std::vector<Vertex> RFCLV(roomFrontCapLeftVertices, roomFrontCapLeftVertices + 4);
	std::vector<GLuint> RFCLI(roomFrontCapLeftIndices, roomFrontCapLeftIndices + 6);

	std::vector<Vertex> RFCRV(roomFrontCapRightVertices, roomFrontCapRightVertices + 4);
	std::vector<GLuint> RFCRI(roomFrontCapRightIndices, roomFrontCapRightIndices + 6);

	// --- Back Cap (Arrière) ---
	std::vector<Vertex> R2BCLV(room2BackCapLeftVertices, room2BackCapLeftVertices + 4);
	std::vector<GLuint> R2BCLI(room2BackCapLeftIndices, room2BackCapLeftIndices + 6);

	std::vector<Vertex> R2BCRV(room2BackCapRightVertices, room2BackCapRightVertices + 4);
	std::vector<GLuint> R2BCRI(room2BackCapRightIndices, room2BackCapRightIndices + 6);


	// Vecteurs de textures pour chaque Mesh
	std::vector<Texture> MFTextures(MainFloorTexture, MainFloorTexture + 2);
	std::vector<Texture> MWTextures(MainWallTexture, MainWallTexture + 2);
	std::vector<Texture> MCTextures(MainCeilingTexture, MainCeilingTexture + 2);
	std::vector<Texture> CFTextures(CorridorFloorTexture, CorridorFloorTexture + 2);
	std::vector<Texture> CWTextures(CorridorWallTexture, CorridorWallTexture + 2);
	std::vector<Texture> CCTextures(CorridorCeilingTexture, CorridorCeilingTexture + 2);
	std::vector<Texture> R2FTextures(Room2FloorTexture, Room2FloorTexture + 2);
	std::vector<Texture> R2WTextures(Room2WallTexture, Room2WallTexture + 2);
	std::vector<Texture> R2CTextures(Room2CeilingTexture, Room2CeilingTexture + 2);
	// Pour les bouchons, on peut réutiliser les textures murales correspondantes
	std::vector<Texture> RFCTextures(MainWallTexture, MainWallTexture + 2);
	std::vector<Texture> R2BCTextures(Room2WallTexture, Room2WallTexture + 2);

	// Meshes
	Mesh MainFloorMesh(MFV, MFI, MFTextures, shaderProgram);
	Mesh MainWallMesh(MWV, MWI, MWTextures, shaderProgram);
	Mesh MainCeilingMesh(MCV, MCI, MCTextures, shaderProgram);

	Mesh CorridorFloorMesh(CFV, CFI, CFTextures, shaderProgram);
	Mesh CorridorWallMesh(CWV, CWI, CWTextures, shaderProgram);
	Mesh CorridorCeilingMesh(CCV, CCI, CCTextures, shaderProgram);

	Mesh Room2FloorMesh(R2FV, R2FI, R2FTextures, shaderProgram);
	Mesh Room2WallMesh(R2WV, R2WI, R2WTextures, shaderProgram);
	Mesh Room2CeilingMesh(R2CV, R2CI, R2CTextures, shaderProgram);

	Mesh RoomFrontCapLeftMesh(RFCLV, RFCLI, RFCTextures, shaderProgram);
	
	Mesh RoomFrontCapRightMesh(RFCRV, RFCRI, RFCTextures, shaderProgram);
	Mesh Room2BackCapLeftMesh(R2BCLV, R2BCLI, R2BCTextures, shaderProgram);
	
	Mesh Room2BackCapRightMesh(R2BCRV, R2BCRI, R2BCTextures, shaderProgram);

	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	std::vector <Texture> lightTextures;
	Mesh light(lightVerts, lightInd,lightTextures , lightShader);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 4.5f, 0.5f);
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
	//add the meshes to the root node
	root->add(&MainFloorMesh);
	root->add(&MainWallMesh);
	root->add(&MainCeilingMesh);
	root->add(&CorridorFloorMesh);
	root->add(&CorridorWallMesh);
	root->add(&CorridorCeilingMesh);
	root->add(&Room2FloorMesh);
	root->add(&Room2WallMesh);
	root->add(&Room2CeilingMesh);
	// Add the separated caps (avant)
	root->add(&RoomFrontCapLeftMesh);
	root->add(&RoomFrontCapRightMesh);

	// Add the separated caps (arrière)
	root->add(&Room2BackCapLeftMesh);
	root->add(&Room2BackCapRightMesh);
	

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
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(FOV, nearPlane, farPlane);

		// Draw the root node
		root->draw(camera);

		lightPos = glm::vec3(0.0f, 0.5f, 4.5f + 5.0f * cos(glfwGetTime()));
		lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);

		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


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