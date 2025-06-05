#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "VBO.h"
#include "mesh.h"
#include "node.h"
#include "texture.h"
#include "shaderClass.h"

// Salle principale 
extern Vertex MainFloorVerticies[4];
extern GLuint MainFloorIndicies[6];
extern Vertex MainWallVertices[16];
extern GLuint MainWallIndices[24];
extern Vertex MainCeilingVertices[4];
extern GLuint MainCeilingIndices[6];

// Couloir
extern Vertex corridorFloorVertices[4];
extern GLuint corridorFloorIndices[6];
extern Vertex corridorWallVertices[16];
extern GLuint corridorWallIndices[24];
extern Vertex corridorCeilingVertices[4];
extern GLuint corridorCeilingIndices[6];

// Salle rectangulaire
extern Vertex room2FloorVertices[4];
extern GLuint room2FloorIndices[6];
extern Vertex room2WallVertices[16];
extern GLuint room2WallIndices[24];
extern Vertex room2CeilingVertices[4];
extern GLuint room2CeilingIndices[6];

// Bouchons salle principale (avant) - séparés en 3 parties
extern Vertex roomFrontCapLeftVertices[4];
extern GLuint roomFrontCapLeftIndices[6];
extern Vertex roomFrontCapDoorVertices[4];
extern GLuint roomFrontCapDoorIndices[6];
extern Vertex roomFrontCapRightVertices[4];
extern GLuint roomFrontCapRightIndices[6];

// Bouchons salle rectangulaire (arrière) - séparés en 3 parties
extern Vertex room2BackCapLeftVertices[4];
extern GLuint room2BackCapLeftIndices[6];


extern Vertex room2BackCapRightVertices[4];
extern GLuint room2BackCapRightIndices[6];

// // Shaders
// extern Shader shaderProgram;
// extern Shader lightShader;

// extern Texture MainFloorTexture[2];
// extern Texture MainWallTexture[2];
// extern Texture MainCeilingTexture[2];
// extern Texture CorridorFloorTexture[2];
// extern Texture Room2FloorTexture[2];
// extern Texture Room2WallTexture[2];
// extern Texture Room2CeilingTexture[2];

// //Meshes
// extern Mesh MainFloorMesh;
// extern Mesh MainWallMesh;
// extern Mesh MainCeilingMesh;
// extern Mesh CorridorFloorMesh;
// extern Mesh CorridorWallMesh;
// extern Mesh CorridorCeilingMesh;
// extern Mesh Room2FloorMesh;
// extern Mesh Room2WallMesh;
// extern Mesh Room2CeilingMesh;
// extern Mesh RoomFrontCapMesh;
// extern Mesh Room2BackCapMesh;

