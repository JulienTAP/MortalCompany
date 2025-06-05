#include "elements.h"

// Salle principale
Vertex MainFloorVerticies[4] = 
{//               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
    Vertex{glm::vec3(-3.5f, -1.0f, -3.5f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 3.5f, -1.0f, -3.5f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 3.5f, -1.0f,  3.5f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-3.5f, -1.0f,  3.5f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(0,1)}
};
GLuint MainFloorIndicies[6] = {0, 1, 2, 2, 3, 0};

Vertex MainWallVertices[16] = {
    // Face arrière
    Vertex{glm::vec3(-3.5f, -1.0f, -3.5f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 3.5f, -1.0f, -3.5f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 3.5f,  1.0f, -3.5f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-3.5f,  1.0f, -3.5f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(0,1)},
    // // Face avant
    // Vertex{glm::vec3(-3.5f, -1.0f,  3.5f), glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(0,0)},
    // Vertex{glm::vec3( 3.5f, -1.0f,  3.5f), glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(1,0)},
    // Vertex{glm::vec3( 3.5f,  1.0f,  3.5f), glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(1,1)},
    // Vertex{glm::vec3(-3.5f,  1.0f,  3.5f), glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(0,1)},
    // Face gauche
    Vertex{glm::vec3(-3.5f, -1.0f, -3.5f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-3.5f,  1.0f, -3.5f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3(-3.5f,  1.0f,  3.5f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-3.5f, -1.0f,  3.5f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(1,0)},
    // Face droite
    Vertex{glm::vec3( 3.5f, -1.0f, -3.5f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 3.5f,  1.0f, -3.5f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3( 3.5f,  1.0f,  3.5f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3( 3.5f, -1.0f,  3.5f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(1,0)}
};
GLuint MainWallIndices[24] = {
    0,1,2, 2,3,0,      // back
    4,5,6, 6,7,4,      // front
    8,9,10, 10,11,8,   // left
    12,13,14, 14,15,12 // right
};

Vertex MainCeilingVertices[4] = {
    Vertex{glm::vec3(-3.5f,  1.0f, -3.5f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 3.5f,  1.0f, -3.5f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 3.5f,  1.0f,  3.5f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-3.5f,  1.0f,  3.5f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(0,1)}
};
GLuint MainCeilingIndices[6] = {0, 1, 2, 2, 3, 0};

// Couloir
Vertex corridorFloorVertices[4] = {
    Vertex{glm::vec3(-0.5f, -1.0f,  3.5f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 0.5f, -1.0f,  3.5f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 0.5f, -1.0f,  6.0f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-0.5f, -1.0f,  6.0f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(0,1)}
};
GLuint corridorFloorIndices[6] = {0, 1, 2, 2, 3, 0};

Vertex corridorWallVertices[16] = {
    // // Face arrière
    // Vertex{glm::vec3(-0.5f, -1.0f,  3.5f), glm::vec3(1,1,1), glm::vec3(0,0,-1), glm::vec2(0,0)},
    // Vertex{glm::vec3( 0.5f, -1.0f,  3.5f), glm::vec3(1,1,1), glm::vec3(0,0,-1), glm::vec2(1,0)},
    // Vertex{glm::vec3( 0.5f,  1.0f,  3.5f), glm::vec3(1,1,1), glm::vec3(0,0,-1), glm::vec2(1,1)},
    // Vertex{glm::vec3(-0.5f,  1.0f,  3.5f), glm::vec3(1,1,1), glm::vec3(0,0,-1), glm::vec2(0,1)},
    // // Face avant
    // Vertex{glm::vec3(-0.5f, -1.0f,  6.0f), glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(0,0)},
    // Vertex{glm::vec3( 0.5f, -1.0f,  6.0f), glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(1,0)},
    // Vertex{glm::vec3( 0.5f,  1.0f,  6.0f), glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(1,1)},
    // Vertex{glm::vec3(-0.5f,  1.0f,  6.0f), glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(0,1)},
    // Face gauche
    Vertex{glm::vec3(-0.5f, -1.0f,  3.5f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-0.5f,  1.0f,  3.5f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3(-0.5f,  1.0f,  6.0f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-0.5f, -1.0f,  6.0f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(1,0)},
    // Face droite
    Vertex{glm::vec3( 0.5f, -1.0f,  3.5f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 0.5f,  1.0f,  3.5f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3( 0.5f,  1.0f,  6.0f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3( 0.5f, -1.0f,  6.0f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(1,0)}
};
GLuint corridorWallIndices[24] = {
    0,1,2, 2,3,0,      // back
    4,5,6, 6,7,4,      // front
    8,9,10, 10,11,8,   // left
    12,13,14, 14,15,12 // right
};

Vertex corridorCeilingVertices[4] = {
    Vertex{glm::vec3(-0.5f,  1.0f,  3.5f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 0.5f,  1.0f,  3.5f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 0.5f,  1.0f,  6.0f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-0.5f,  1.0f,  6.0f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(0,1)}
};
GLuint corridorCeilingIndices[6] = {0, 1, 2, 2, 3, 0};

// Salle rectangulaire
Vertex room2FloorVertices[4] = {
    Vertex{glm::vec3(-3.0f, -1.0f,  6.0f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 3.0f, -1.0f,  6.0f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 3.0f, -1.0f, 10.0f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-3.0f, -1.0f, 10.0f),  glm::vec3(0,1,0),glm::vec3(1,1,1), glm::vec2(0,1)}
};
GLuint room2FloorIndices[6] = {0, 1, 2, 2, 3, 0};

Vertex room2WallVertices[16] = {
    // // Face arrière
    // Vertex{glm::vec3(-3.0f, -1.0f,  6.0f), glm::vec3(1,1,1), glm::vec3(0,0,-1), glm::vec2(0,0)},
    // Vertex{glm::vec3( 3.0f, -1.0f,  6.0f), glm::vec3(1,1,1), glm::vec3(0,0,-1), glm::vec2(1,0)},
    // Vertex{glm::vec3( 3.0f,  1.0f,  6.0f), glm::vec3(1,1,1), glm::vec3(0,0,-1), glm::vec2(1,1)},
    // Vertex{glm::vec3(-3.0f,  1.0f,  6.0f), glm::vec3(1,1,1), glm::vec3(0,0,-1), glm::vec2(0,1)},
    // Face avant
    Vertex{glm::vec3(-3.0f, -1.0f, 10.0f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 3.0f, -1.0f, 10.0f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 3.0f,  1.0f, 10.0f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-3.0f,  1.0f, 10.0f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(0,1)},
    // Face gauche
    Vertex{glm::vec3(-3.0f, -1.0f,  6.0f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-3.0f,  1.0f,  6.0f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3(-3.0f,  1.0f, 10.0f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-3.0f, -1.0f, 10.0f),  glm::vec3(1,0,0),glm::vec3(1,1,1), glm::vec2(1,0)},
    // Face droite
    Vertex{glm::vec3( 3.0f, -1.0f,  6.0f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 3.0f,  1.0f,  6.0f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(0,1)},
    Vertex{glm::vec3( 3.0f,  1.0f, 10.0f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3( 3.0f, -1.0f, 10.0f),  glm::vec3(-1,0,0),glm::vec3(1,1,1), glm::vec2(1,0)}
};
GLuint room2WallIndices[24] = {
    0,1,2, 2,3,0,      // back
    4,5,6, 6,7,4,      // front
    8,9,10, 10,11,8,   // left
    12,13,14, 14,15,12 // right
};

Vertex room2CeilingVertices[4] = {
    Vertex{glm::vec3(-3.0f,  1.0f,  6.0f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3( 3.0f,  1.0f,  6.0f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3( 3.0f,  1.0f, 10.0f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-3.0f,  1.0f, 10.0f),  glm::vec3(0,-1,0),glm::vec3(1,1,1), glm::vec2(0,1)}
};
GLuint room2CeilingIndices[6] = {0, 1, 2, 2, 3, 0};

// Bouchons salle principale (avant) - séparés en 2 parties

// Partie gauche (jusqu'au bord gauche du couloir)
Vertex roomFrontCapLeftVertices[4] = {
    Vertex{glm::vec3(-3.5f, -1.0f, 3.5f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-0.5f, -1.0f, 3.5f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3(-0.5f,  1.0f, 3.5f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-3.5f,  1.0f, 3.5f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(0,1)}
};
GLuint roomFrontCapLeftIndices[6] = {0, 1, 2, 2, 3, 0};

// Partie droite (à partir du bord droit du couloir)
Vertex roomFrontCapRightVertices[4] = {
    Vertex{glm::vec3(0.5f, -1.0f, 3.5f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(3.5f, -1.0f, 3.5f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3(3.5f,  1.0f, 3.5f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(0.5f,  1.0f, 3.5f),  glm::vec3(0,0,-1),glm::vec3(1,1,1), glm::vec2(0,1)}
};
GLuint roomFrontCapRightIndices[6] = {0, 1, 2, 2, 3, 0};


// Bouchons salle rectangulaire (arrière) - séparés en 2 parties

// Partie gauche (jusqu'au bord gauche du couloir)
Vertex room2BackCapLeftVertices[4] = {
    Vertex{glm::vec3(-3.0f, -1.0f, 6.0f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(-0.5f, -1.0f, 6.0f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3(-0.5f,  1.0f, 6.0f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(-3.0f,  1.0f, 6.0f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(0,1)}
};
GLuint room2BackCapLeftIndices[6] = {0, 1, 2, 2, 3, 0};

// Partie droite (à partir du bord droit du couloir)
Vertex room2BackCapRightVertices[4] = {
    Vertex{glm::vec3(0.5f, -1.0f, 6.0f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(0,0)},
    Vertex{glm::vec3(3.0f, -1.0f, 6.0f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(1,0)},
    Vertex{glm::vec3(3.0f,  1.0f, 6.0f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(1,1)},
    Vertex{glm::vec3(0.5f,  1.0f, 6.0f),  glm::vec3(0,0,1),glm::vec3(1,1,1), glm::vec2(0,1)}
};
GLuint room2BackCapRightIndices[6] = {0, 1, 2, 2, 3, 0};