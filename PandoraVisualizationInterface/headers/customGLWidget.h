#ifndef CUSTOMGLWIDGET_H
#define CUSTOMGLWIDGET_H

#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <vector>
#include <simulationWorld.h>
#include "grid.h"
#include "shape.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;


class CustomGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    CustomGLWidget(QWidget *parent=0);
   ~CustomGLWidget();
    void setElevation();
    void resetView();
    void setSimulationParameters(SimulationWorld* world, string currentRaster, map<string, Shape::ShapeSettings> &settings);
    void setCurrentStep(int step);
    void setCurrentRaster(string rasterName);
    void setAgentSettings(map<string, Shape::ShapeSettings> &settings);
    void setEnabled(bool enabled);


protected:

  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL(int width, int height);

  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);

private:
  void loadShaders();
  void projectTransform();
  void viewTransform ();

  QOpenGLShaderProgram *program;
  GLuint projLoc, viewLoc, normalLoc;

  glm::mat4 viewMatrix;
  glm::mat4 projectMatrix;

  typedef  enum {NONE, ROTATE, PANNING} InteractiveAction;
  InteractiveAction DoingInteractive;
  int xClick, yClick;
  float angleX, angleY;
  float moveX, moveZ;
  int width, height;
  float zoom;
  float radius;
  bool elevation;
  bool enabled;

  Grid* grid;
};

#endif // CUSTOMGLWIDGET_H
