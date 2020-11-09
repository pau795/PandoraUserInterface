#include "shape.h"
#include <iostream>

using namespace std;

const string Shape::iconPaths[] = {"icons/triangle.png",
                                     "icons/quad.png",
                                     "icons/penthagon.png",
                                     "icons/hexagon.png",
                                     "icons/circle.png",
                                     "icons/tetra.png",
                                     "icons/pyramid.png",
                                     "icons/cone.png"};


const glm::vec3 Shape::BLUE = glm::vec3{0.0, 0.0, 1.0};
const glm::vec3 Shape::RED = glm::vec3{1.0, 0.0, 0.0};
const glm::vec3 Shape::GREEN = glm::vec3{0.0, 1.0, 0.0};
const glm::vec3 Shape::CYAN = glm::vec3{0.0, 1.0, 1.0};
const glm::vec3 Shape::PURPLE = glm::vec3{0.5, 0.0, 1.0};
const glm::vec3 Shape::ORANGE = glm::vec3{1.0, 0.5, 0.0};
const glm::vec3 Shape::WHITE = glm::vec3{0.95, 0.95, 0.95};
const glm::vec3 Shape::BLACK = glm::vec3{0.05, 0.05, 0.05};

const glm::vec3 Shape::defaultColors[]={ BLUE,
                                            RED,
                                            GREEN,
                                            CYAN,
                                            PURPLE,
                                            ORANGE,
                                            WHITE,
                                            BLACK};

const string Shape::getIconName(unsigned int shape){
    return iconPaths[shape];
}

unsigned int Shape::getDefaultShape(unsigned int pos){
    if (pos > CONE) pos = CONE;
    return pos;
}

unsigned int Shape::getDefaultColorId(unsigned int pos){
    if (pos > 7) pos = 0;
    return pos;
}

const glm::vec3 Shape::getColor(unsigned int pos){
    return defaultColors[pos];
}

Shape::Shape(int shape, glm::vec3 pos, float radius, float height, glm::vec3 color, QOpenGLShaderProgram *program){
    this->pos = pos;
    this->initialHeight = height;
    this->height = height;
    this->color = color;
    this->radius = radius;
    initializeOpenGLFunctions();
    vertexLoc = glGetAttribLocation (program->programId(), "vertex");
    colorLoc = glGetAttribLocation (program->programId(), "color");
    normLoc = glGetAttribLocation (program->programId(), "normal");
    transLoc = glGetUniformLocation (program->programId(), "TG");
    normalLoc = glGetUniformLocation (program->programId(), "normalMatrix");
    init_shape();
    setShapeId(shape);
}


void Shape::init_shape(){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_Pos);
    glGenBuffers(1, &VBO_Norm);
    glGenBuffers(1, &VBO_Color);
}

void Shape::render(glm::mat4 &viewMatrix){
    glBindVertexArray (VAO);
    modelTransform();
    normalMatrix(viewMatrix);
    glDrawArrays(GL_TRIANGLES, 0, numFaces*12);
}

void Shape::setShapeId(int shapeId){
    this-> shapeId = shapeId;
    switch (shapeId){
        case TRIANGLE:
            this->baseRadius = radius;
            this->topRadius = radius;
            this->numFaces = 3;
            this->height = initialHeight;
            this->rotAngle = -M_PI/6;
            break;
        case QUAD:
            this->baseRadius = radius;
            this->topRadius = radius;
            this->numFaces = 4;
            this->height = initialHeight;
            this->rotAngle = -M_PI/4;
            break;
        case PENTHAGON:
            this->baseRadius = radius;
            this->topRadius = radius;
            this->numFaces = 5;
            this->height = initialHeight;
            this->rotAngle = M_PI/10;
            break;
        case HEXAGON:
            this->baseRadius = radius;
            this->topRadius = radius;
            this->numFaces = 6;
            this->height = initialHeight;
            this->rotAngle = 0;
            break;
        case CIRCLE:
            this->baseRadius = radius;
            this->topRadius = radius;
            this->numFaces = 20;
            this->height = initialHeight;
            this->rotAngle = 0;
            break;
        case TETRA:
            this->baseRadius = radius;
            this->topRadius = 0;
            this->numFaces = 3;
            this->height = initialHeight / 0.5;
            this->rotAngle = -M_PI/6;
            break;
        case PYRAMID:
            this->baseRadius = radius;
            this->topRadius = 0;
            this->numFaces = 4;
            this->height = initialHeight / 0.5;
            this->rotAngle = -M_PI/4;
            break;
        case CONE:
            this->baseRadius = radius;
            this->topRadius = 0;
            this->numFaces = 20;
            this->height = initialHeight / 0.5;
            this->rotAngle = 0;
            break;
        default:
            this->baseRadius = radius;
            this->topRadius = radius;
            this->numFaces = 20;
            this->height = initialHeight;
            this->rotAngle = 0;
        break;
    }
    setData();

}


void Shape::setData(){
    glm::vec3 vertices[numFaces*12];
    glm::vec3 normals[numFaces*12];
    glm::vec3 colors[numFaces*12];
    float angleInc = 2*M_PI / numFaces;
    glm::vec3 topCenter = glm::vec3{0.0, height, 0.0};
    glm::vec3 baseCenter = glm::vec3{0.0, 0.0, 0.0};;
    glm::vec3 iTPos = glm::vec3{topCenter.x + topRadius*glm::cos(0), topCenter.y, topCenter.z + topRadius * glm::sin(0)};
    glm::vec3 iBPos = glm::vec3{baseCenter.x + baseRadius*glm::cos(0), baseCenter.y, baseCenter.z + baseRadius * glm::sin(0)};
    float a = angleInc;
    //generates a generic cylindric shape
    //for each angle step, we generate a portion of the cylinder made of 4 triangles:
    //one at the top face, two at the side, and one at the bottom face
    for (int i = 0; i < numFaces; ++i){
        glm::vec3 tPos = glm::vec3{topCenter.x + topRadius*glm::cos(a), topCenter.y, topCenter.z + topRadius *glm::sin(a)};
        glm::vec3 bPos = glm::vec3{baseCenter.x + baseRadius*glm::cos(a), baseCenter.y, baseCenter.z + baseRadius *glm::sin(a)};


        //FIRST TRIANGLE

        vertices[12*i] = topCenter;
        vertices[12*i+1] = iTPos;
        vertices[12*i+2] = tPos;

        normals[12*i] = glm::vec3{0.0, 1.0, 0.0};
        normals[12*i+1] = glm::vec3{0.0, 1.0, 0.0};
        normals[12*i+2] = glm::vec3{0.0, 1.0, 0.0};

        colors[12*i] = color;
        colors[12*i+1] = color;
        colors[12*i+2] = color;

        //SECOND TRIANGLE

        vertices[12*i+3] = iTPos;
        vertices[12*i+4] = tPos;
        vertices[12*i+5] = bPos;

        //we calculate the normal for the triangles at the side,
        //selecting the vectors which are not null, in case there is a conic kind of shape
        glm::vec3 u, v, norm;
        if (glm::distance(iTPos, tPos) >= glm::distance(iBPos, bPos)){
              u = tPos - iTPos, v = iBPos - iTPos;
              norm = normalize(cross(u,v));
        }
        else {
            u = bPos - iBPos, v = iTPos - iBPos;
            norm = normalize(cross(v,u));
        };


        normals[12*i+3] = norm;
        normals[12*i+4] = norm;
        normals[12*i+5] = norm;

        colors[12*i+3] = color;
        colors[12*i+4] = color;
        colors[12*i+5] = color;

         //THIRD TRIANGLE

        vertices[12*i+6] = iTPos;
        vertices[12*i+7] = iBPos;
        vertices[12*i+8] = bPos;

        normals[12*i+6] = norm;
        normals[12*i+7] = norm;
        normals[12*i+8] = norm;

        colors[12*i+6] = color;
        colors[12*i+7] = color;
        colors[12*i+8] = color;

        //FORTH TRIANGLE

        vertices[12*i+9] = iBPos;
        vertices[12*i+10] = bPos;
        vertices[12*i+11] = baseCenter;

        normals[12*i+9] = glm::vec3{0.0, -1.0, 0.0};
        normals[12*i+10] = glm::vec3{0.0, -1.0, 0.0};
        normals[12*i+11] = glm::vec3{0.0, -1.0, 0.0};

        colors[12*i+9] = color;
        colors[12*i+10] = color;
        colors[12*i+11] = color;

        a+=angleInc;
        iTPos = tPos;
        iBPos = bPos;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Norm);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);


}

void Shape::setRadius(float baseRadius, float topRadius){
    this->baseRadius = baseRadius;
    this->topRadius = topRadius;
    setData();
}

void Shape::setHeight(float height){
    this->height = height;
    setData();
}

void Shape::setNumFaces(float numFaces){
    this->numFaces = numFaces;
    setData();
}

void Shape::setColor(const glm::vec3 &color){
    this->color.x = color.x;
    this->color.y = color.y;
    this->color.z = color.z;
    setData();
}

void Shape::setPosition(const glm::vec3 &pos){
    this->pos.x = pos.x;
    this->pos.y = pos.y;
    this->pos.z = pos.z;
}

void Shape::setSettings(ShapeSettings &st){
    this->color = getColor(st.colorId);
    setShapeId(st.shape);

}

float Shape::getBaseRadius(){
    return baseRadius;
}

float Shape::getTopRadius(){
    return topRadius;
}

float Shape::getHeight(){
    return height;
}

int Shape::getNumFaces(){
    return numFaces;
}

int Shape::getShapeId(){
    return shapeId;
}

glm::mat4 Shape::modelMatrix(){
    glm::mat4 TG(1.f);
    TG = glm::translate(TG, glm::vec3{-5.0,0.0,-5.0} + pos);
    TG = glm::rotate(TG, float(rotAngle), glm::vec3{0,1,0});
    return TG;
}

void Shape::normalMatrix(glm::mat4 &viewMatrix){
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * modelMatrix()));
    glUniformMatrix4fv (normalLoc, 1, GL_FALSE, &normalMatrix[0][0]);
}

void Shape::modelTransform(){
    glm::mat4 TG = modelMatrix();
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}
