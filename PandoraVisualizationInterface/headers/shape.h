#ifndef CYLINDER_H
#define CYLINDER_H



#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;


class Shape: protected QOpenGLFunctions_3_3_Core
{
public:

    //Builds a predefined cylindric shape, using the defined shapes in the class
    Shape(int shape, glm::vec3 pos, float radius, float height, glm::vec3 color, QOpenGLShaderProgram *program);

    struct ShapeSettings{
        bool active;
        int colorId;
        int shape;
    };


    void render(glm::mat4 &viewMatrix);
    void setRadius(float baseRadius, float topRadius);
    void setHeight(float height);
    void setNumFaces(float numFaces);
    void setColor(const glm::vec3 &color);
    void setPosition(const glm::vec3 &pos);
    float getBaseRadius();
    float getTopRadius();
    float getHeight();
    int getNumFaces();
    int getShapeId();
    void setShapeId(int shapeId);
    void setSettings(ShapeSettings &st);

    const static string getIconName(unsigned int shape);
    static unsigned int getDefaultShape(unsigned int pos);
    static unsigned int getDefaultColorId(unsigned int pos);
    const static glm::vec3 getColor(unsigned int pos);

    const static unsigned int TRIANGLE = 0;
    const static unsigned int QUAD = 1;
    const static unsigned int PENTHAGON = 2;
    const static unsigned int HEXAGON = 3;
    const static unsigned int CIRCLE = 4;
    const static unsigned int TETRA = 5;
    const static unsigned int PYRAMID = 6;
    const static unsigned int CONE = 7;

    const static glm::vec3 BLUE;
    const static glm::vec3 RED;
    const static glm::vec3 GREEN;
    const static glm::vec3 CYAN;
    const static glm::vec3 PURPLE;
    const static glm::vec3 ORANGE;
    const static glm::vec3 WHITE;
    const static glm::vec3 BLACK;


private:
    GLuint VAO, VBO_Pos, VBO_Norm, VBO_Color;
    glm::vec3 pos, color;
    float radius, baseRadius, topRadius;
    float height, initialHeight;
    int numFaces;
    float rotAngle;
    int shapeId;
    GLuint vertexLoc, normLoc, colorLoc, transLoc, normalLoc;

    void modelTransform();
    void init_shape();
    void setData();
    glm::mat4 modelMatrix();
    void normalMatrix(glm::mat4 &viewMatrix);
    const static string iconPaths[];
    const static glm::vec3 defaultColors[];

};


#endif // CYLINDER_H
