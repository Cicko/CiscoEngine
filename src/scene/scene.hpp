#ifndef SCENE_HPP
#define SCENE_HPP

#include <glad/glad.h>
#include <vector>
#include <string>

struct Object {
    std::vector<float> vertices;       // pos (3) + normal (3) per vertex
    std::vector<unsigned int> indices; // Indices for drawing
    unsigned int VAO, VBO, EBO;
    float position[3];                 // Object's position in world space
    bool hasTexCoords;                 // Whether the OBJ has texture coords
};

struct Scene {
    // Collection of objects (replaces single cube)
    std::vector<Object> objects;

    // Floor data (unchanged)
    float floorVertices[121 * 6]; // 11x11 points, 3 pos + 3 normal
    unsigned int floorIndices[1200];
    unsigned int floorVAO, floorVBO, floorEBO;

    void initScene();           // Initialize floor only
    void cleanupScene();        // Cleanup all objects and floor
    bool add(const std::string& filename, float position[3]); // Add an OBJ at a position

private:
    bool loadObj(const std::string& filename, std::vector<float>& vertices, 
                 std::vector<unsigned int>& indices, bool& hasTexCoords);
    void initFloor();
    void setupObjectBuffers(Object& obj);
};

#endif