#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"camera.h"
#include"texture.h"
#include"shaderClass.h"

class Mesh
{
public:
	// Stores the vertex data
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	// Store VAO in public so it can be used in the Draw function
	VAO vao;

	//Store shader to draw nodes more easily
	Shader shader;

	// Initializes the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures,Shader& shader);

	// Draws the mesh
	void Draw(Camera& camera);
};
#endif