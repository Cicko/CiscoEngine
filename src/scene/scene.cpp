#include "scene.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool Scene::loadObj(const std::string& filename, std::vector<float>& vertices, 
                    std::vector<unsigned int>& indices, bool& hasTexCoords) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return false;
    }

    std::vector<float> positions;    // v: x, y, z
    std::vector<float> normals;      // vn: nx, ny, nz
    std::vector<float> texCoords;    // vt: u, v
    std::string line;
    hasTexCoords = false;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
        }
        else if (type == "vn") {
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);
        }
        else if (type == "vt") {
            hasTexCoords = true;
            float u, v;
            iss >> u >> v;
            texCoords.push_back(u);
            texCoords.push_back(v);
        }
        else if (type == "f") {
            std::string v1, v2, v3;
            iss >> v1 >> v2 >> v3;
            for (const auto& vertex : {v1, v2, v3}) {
                std::istringstream vss(vertex);
                int vi = 0, vti = -1, vni = 0;
                char slash;

                vss >> vi;
                vss >> slash;
                if (vss.peek() != '/') {
                    vss >> vti;
                    vss >> slash;
                }
                vss >> vni;

                vi--; if (vti != -1) vti--; vni--;

                vertices.push_back(positions[vi * 3]);
                vertices.push_back(positions[vi * 3 + 1]);
                vertices.push_back(positions[vi * 3 + 2]);
                vertices.push_back(normals[vni * 3]);
                vertices.push_back(normals[vni * 3 + 1]);
                vertices.push_back(normals[vni * 3 + 2]);
                if (hasTexCoords && vti >= 0) {
                    vertices.push_back(texCoords[vti * 2]);
                    vertices.push_back(texCoords[vti * 2 + 1]);
                }
                indices.push_back(indices.size());
            }
        }
    }
    file.close();
    return true;
}

void Scene::initFloor() {
    int index = 0;
    for (int z = 0; z <= 10; z++) {
        for (int x = 0; x <= 10; x++) {
            floorVertices[index++] = (x - 5.0f);
            floorVertices[index++] = -1.0f; // Floor at y = -1
            floorVertices[index++] = (z - 5.0f);
            floorVertices[index++] = 0.0f;
            floorVertices[index++] = 1.0f;
            floorVertices[index++] = 0.0f;
        }
    }

    index = 0;
    for (int z = 0; z < 10; z++) {
        for (int x = 0; x < 10; x++) {
            int topLeft = z * 11 + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * 11 + x;
            int bottomRight = bottomLeft + 1;
            floorIndices[index++] = topLeft;
            floorIndices[index++] = bottomLeft;
            floorIndices[index++] = topRight;
            floorIndices[index++] = topRight;
            floorIndices[index++] = bottomLeft;
            floorIndices[index++] = bottomRight;
        }
    }

    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Scene::setupObjectBuffers(Object& obj) {
    glGenVertexArrays(1, &obj.VAO);
    glGenBuffers(1, &obj.VBO);
    glGenBuffers(1, &obj.EBO);
    glBindVertexArray(obj.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, obj.vertices.size() * sizeof(float), obj.vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.indices.size() * sizeof(unsigned int), obj.indices.data(), GL_STATIC_DRAW);

    int stride = obj.hasTexCoords ? 8 * sizeof(float) : 6 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    if (obj.hasTexCoords) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    glBindVertexArray(0);
}

void Scene::initScene() {
    initFloor();
    // No default cube; use add() to load objects
}

void Scene::cleanupScene() {
    for (auto& obj : objects) {
        glDeleteVertexArrays(1, &obj.VAO);
        glDeleteBuffers(1, &obj.VBO);
        glDeleteBuffers(1, &obj.EBO);
    }
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteBuffers(1, &floorEBO);
}

bool Scene::add(const std::string& filename, float position[3]) {
    Object obj;
    obj.position[0] = position[0];
    obj.position[1] = position[1];
    obj.position[2] = position[2];

    if (!loadObj(filename, obj.vertices, obj.indices, obj.hasTexCoords)) {
        return false;
    }

    setupObjectBuffers(obj);
    objects.push_back(obj);
    return true;
}