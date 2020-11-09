#include "tile.h"
#include "grid.h"
#include <iostream>
using namespace std;


Tile::Tile(glm::vec2 minPos, glm::vec2 maxPos, float height, QOpenGLShaderProgram* program){
    this->minPos = minPos;
    this->maxPos = maxPos;
    this->height = height;
    calculateColor();
    elevation = true;
    initializeOpenGLFunctions();
    vertexLoc = glGetAttribLocation (program->programId(), "vertex");
    colorLoc = glGetAttribLocation (program->programId(), "color");
    normLoc = glGetAttribLocation (program->programId(), "normal");
    transLoc = glGetUniformLocation (program->programId(), "TG");
    normalLoc = glGetUniformLocation (program->programId(), "normalMatrix");
    init_tile();
}

void Tile::init_tile(){


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_Pos);
    setVertexData();

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_Norm);
    setNormalData();

    glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normLoc);

    glGenBuffers(1, &VBO_Color);
    setColorData();

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);


}

void Tile::setHeight(float height){
    this->height = height;
    calculateColor();
    setVertexData();
    setColorData();
}

void Tile::setElevation(bool elevation) {
    this->elevation = elevation;
    setVertexData();
    setColorData();
}

glm::vec3 Tile::getCenter(){
    return center;
}

float Tile::getHeight(){
    return height;
}

void Tile::setColorData(){
    if (not elevation){
        glm::vec3 colorGround[6] = {
            color, color, color, color, color, color
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorGround), colorGround, GL_STATIC_DRAW);
    }
    else {
        glm::vec3 colorGround[30] = {
            color, color, color, color, color, color,
            color, color, color, color, color, color,
            color, color, color, color, color, color,
            color, color, color, color, color, color,
            color, color, color, color, color, color
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorGround), colorGround, GL_STATIC_DRAW);
    }

}

void Tile::setVertexData(){
    if (not elevation){
        glm::vec3 posGround[6] = {
          glm::vec3(minPos.x, -1.0, maxPos.y),
          glm::vec3(maxPos.x, -1.0, maxPos.y),
          glm::vec3(minPos.x, -1.0, minPos.y),
          glm::vec3(minPos.x, -1.0, minPos.y),
          glm::vec3(maxPos.x, -1.0, maxPos.y),
          glm::vec3(maxPos.x, -1.0, minPos.y)
        };
        center.x = (minPos.x + maxPos.x) /2;
        center.y = -1;
        center.z = (minPos.y + maxPos.y) /2;
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(posGround), posGround, GL_STATIC_DRAW);
    }
    else {


        float minHeight = -1.0;
        float tileHeight = minHeight + height;

        glm::vec3 posGround[30] = {
            glm::vec3(minPos.x, tileHeight, maxPos.y),
            glm::vec3(maxPos.x, tileHeight, maxPos.y),
            glm::vec3(minPos.x, tileHeight, minPos.y),
            glm::vec3(minPos.x, tileHeight, minPos.y),
            glm::vec3(maxPos.x, tileHeight, maxPos.y),
            glm::vec3(maxPos.x, tileHeight, minPos.y),

            glm::vec3(minPos.x, tileHeight, maxPos.y),
            glm::vec3(minPos.x, tileHeight, minPos.y),
            glm::vec3(minPos.x, minHeight, maxPos.y),
            glm::vec3(minPos.x, minHeight, maxPos.y),
            glm::vec3(minPos.x, tileHeight, minPos.y),
            glm::vec3(minPos.x, minHeight, minPos.y),

            glm::vec3(maxPos.x, tileHeight, minPos.y),
            glm::vec3(minPos.x, tileHeight, minPos.y),
            glm::vec3(maxPos.x, minHeight, minPos.y),
            glm::vec3(maxPos.x, minHeight, minPos.y),
            glm::vec3(minPos.x, tileHeight, minPos.y),
            glm::vec3(minPos.x, minHeight, minPos.y),

            glm::vec3(maxPos.x, tileHeight, maxPos.y),
            glm::vec3(maxPos.x, tileHeight, minPos.y),
            glm::vec3(maxPos.x, minHeight, maxPos.y),
            glm::vec3(maxPos.x, minHeight, maxPos.y),
            glm::vec3(maxPos.x, tileHeight, minPos.y),
            glm::vec3(maxPos.x, minHeight, minPos.y),

            glm::vec3(maxPos.x, tileHeight, maxPos.y),
            glm::vec3(minPos.x, tileHeight, maxPos.y),
            glm::vec3(maxPos.x, minHeight, maxPos.y),
            glm::vec3(maxPos.x, minHeight, maxPos.y),
            glm::vec3(minPos.x, tileHeight, maxPos.y),
            glm::vec3(minPos.x, minHeight, maxPos.y),
        };
        center.x = (minPos.x + maxPos.x) /2;
        center.y = tileHeight;
        center.z = (minPos.y + maxPos.y) /2;
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(posGround), posGround, GL_STATIC_DRAW);
    }
}

void Tile::setNormalData(){
    if (not elevation){
        glm::vec3 normal = glm::vec3{0.0, 1.0, 0.0};
        glm::vec3 norm[6] = {
          normal, normal, normal, normal, normal, normal
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Norm);
        glBufferData(GL_ARRAY_BUFFER, sizeof(norm), norm, GL_STATIC_DRAW);
    }
    else {
        glm::vec3 normal[5] = {
            glm::vec3{0.0, 1.0, 0.0},
            glm::vec3{-1.0, 0.0, 0.0},
            glm::vec3{0.0, 0.0, -1.0},
            glm::vec3{1.0, 0.0, 0.0},
            glm::vec3{0.0, 0.0, 1.0}
        };
        glm::vec3 norm[30]{
            normal[0], normal[0], normal[0], normal[0], normal[0], normal[0],
            normal[1], normal[1], normal[1], normal[1], normal[1], normal[1],
            normal[2], normal[2], normal[2], normal[2], normal[2], normal[2],
            normal[3], normal[3], normal[3], normal[3], normal[3], normal[3],
            normal[4], normal[4], normal[4], normal[4], normal[4], normal[4]
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Norm);
        glBufferData(GL_ARRAY_BUFFER, sizeof(norm), norm, GL_STATIC_DRAW);
    }
}

void Tile::normalMatrix(glm::mat4 &viewMatrix){
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * modelMatrix()));
    glUniformMatrix4fv (normalLoc, 1, GL_FALSE, &normalMatrix[0][0]);
}

void Tile::calculateColor(){
    float value = 0.4 + (1 - (height*2)) * 0.6;
    color = Grid::palette * value;
}

void Tile::render(glm::mat4 &viewMatrix){
    glBindVertexArray (VAO);
    modelTransform();
    normalMatrix(viewMatrix);
    if (elevation) glDrawArrays(GL_TRIANGLES, 0, 30);
    else glDrawArrays(GL_TRIANGLES, 0, 6);

}
glm::mat4 Tile::modelMatrix(){
    glm::mat4 TG(1.f);
    TG = glm::translate(TG, glm::vec3(-5.0,0,-5.0));
    return TG;
}

void Tile::modelTransform(){
    glm:: mat4 TG = modelMatrix();
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
