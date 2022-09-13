#pragma once

#include "utilities.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DIM 256

class Model {
public:
    Model() {
        computeField();
        generateTriangles();

        fprintf(stderr, "Vertices size is %lu\n", _vertices.size());
        fprintf(stderr, "Normals size is %lu\n", _normals.size());
    }

    void setupBuffers();
    void draw();
    void deleteGLResources();

private:
    float gridValues[DIM][DIM][DIM];

    std::vector<glm::vec3> _vertices;
    std::vector<glm::vec3> _normals;


    GLuint _vao;
    GLuint _vertexBuffer;
    GLuint _normalBuffer;

    void computeField();
    void generateTriangles();
    float fieldFunction(float x, float y, float z);
};

float Model::fieldFunction(float x, float y, float z) {
    // equation of a sphere centered at (DIM/2, DIM/2, DIM/2) with radius DIM/2
    // positive outside the sphere and negative inside. The surface has value = 0
    return pow(x - DIM / 2, 2) + pow(y - DIM / 2, 2) + pow(z - DIM / 2, 2) - pow(DIM / 2, 2);
}

void Model::computeField() {
    // implicit positions of grid points: pos = <i / DIM, j / DIM, k / DIM>
    for (size_t i = 0; i < DIM; i++) {
        for (size_t j = 0; j < DIM; j++) {
            for (size_t k = 0; k < DIM; k++) {
                gridValues[i][j][k] = fieldFunction((float)i / (float) DIM, (float)j / (float) DIM, (float)k / (float) DIM);
            }
        }
    }
}

void Model::generateTriangles() {
    for (size_t i = 0; i < DIM - 1; i++) {
        for (size_t j = 0; j < DIM - 1; j++) {
            for (size_t k = 0; k < DIM - 1; k++) {
                gridValues[i][j][k] = fieldFunction((float)i / (float) DIM, (float)j / (float) DIM, (float)k / (float) DIM);
            }
        }
    }
}

void Model::setupBuffers() {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // vertex buffer
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices.at(0)), _vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), (void *) 0);
    glEnableVertexAttribArray(0);

    // normal buffer
    glGenBuffers(1, &_normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(_normals.at(0)), _normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(glm::vec3), (void *) 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::deleteGLResources() {
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteBuffers(1, &_normalBuffer);
    glDeleteVertexArrays(1, &_vao);
}

void Model::draw() {
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 3 * _vertices.size());
}