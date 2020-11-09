#ifndef TILE_H
#define TILE_H

#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Tile: protected QOpenGLFunctions_3_3_Core
{
public:
    Tile(glm::vec2 minPos, glm::vec2 maxPos, float height, QOpenGLShaderProgram *program);
    void render(glm::mat4 &viewMatrix);
    float getHeight();
    void setHeight(float height);
    void setElevation(bool elevation);
    glm::vec3 getCenter();



private:
    GLuint VAO, VBO_Pos, VBO_Norm, VBO_Color;
    glm::vec2 minPos, maxPos;
    glm::vec3 color;
    float height;
    glm::vec3 center;
    GLuint vertexLoc, normLoc, colorLoc, transLoc, normalLoc;
    void normalMatrix(glm::mat4 &viewMatrix);

    bool elevation;
    void modelTransform();
    void init_tile();
    void setColorData();
    void setNormalData();
    void setVertexData();
    glm::mat4 modelMatrix();
    void calculateColor();

};

#endif // TILE_H
