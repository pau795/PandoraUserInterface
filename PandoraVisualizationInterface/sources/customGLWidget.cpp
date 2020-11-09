#include "customGLWidget.h"

#include <iostream>

CustomGLWidget::CustomGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);
  xClick = yClick = 0;
  angleY = 0.0;
  angleX = 7*M_PI/18;
  moveX = 0;
  moveZ = 0;
  zoom = 1.0;
  radius = sqrt(3);
  DoingInteractive = NONE;
  enabled = false;
  width = 1.0f;
  height = 1.0f;

}

CustomGLWidget::~CustomGLWidget ()
{
  if (program != NULL) delete program;
  if (enabled) delete grid;
}


void CustomGLWidget::setSimulationParameters(SimulationWorld *world, string currentRaster, map<string, Shape::ShapeSettings> &settings){
    elevation = true;
    if (enabled) delete grid;
    resetView();
    grid = new Grid(world, currentRaster, settings, program);
}

void CustomGLWidget::setCurrentStep(int step){
    makeCurrent();
    grid->setCurrentStep(step);
    update();

}
void CustomGLWidget::setCurrentRaster(string rasterName){
    makeCurrent();
    grid->setCurrentRaster(rasterName);
    update();

}
void CustomGLWidget::setAgentSettings(map<string, Shape::ShapeSettings> &settings){
    makeCurrent();
    grid->setShapeSettings(settings);
    update();
}

void CustomGLWidget::initializeGL ()
{
  initializeOpenGLFunctions();
  glClearColor(0.9, 0.9, 0.9, 1.0);
  glEnable(GL_DEPTH_TEST);
  loadShaders();
  projectTransform ();
  viewTransform ();
}

void CustomGLWidget::paintGL ()
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if(enabled) grid->render(viewMatrix);
    glBindVertexArray(0);

}

void CustomGLWidget::resizeGL (int w, int h)
{
  width = w;
  height = h;
  glViewport(0, 0, w, h);
  projectTransform ();
  viewTransform ();
  update();
}

void CustomGLWidget::setEnabled(bool enabled){
    this->enabled = enabled;
}


void CustomGLWidget::loadShaders()
{
  QOpenGLShader fs(QOpenGLShader::Fragment);;
  QOpenGLShader vs(QOpenGLShader::Vertex);;
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  program = new QOpenGLShaderProgram();
  program->addShader(&fs);
  program->addShader(&vs);
  program->link();
  program->bind();
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");

}


void CustomGLWidget::projectTransform ()
{
  glm::mat4 Proj;
  Proj = glm::perspective(float(M_PI/3)*zoom , float(width) / float(height), 0.01f, 20.0f*radius);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void CustomGLWidget::viewTransform ()
{
  viewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0,0,-7*radius));
  viewMatrix = glm::translate(viewMatrix, glm::vec3(moveX,moveZ*angleX,moveZ*(angleX-M_PI/2)));
  viewMatrix = glm::rotate(viewMatrix, angleX, glm::vec3(1, 0, 0));
  viewMatrix = glm::rotate(viewMatrix, angleY, glm::vec3(0, 1, 0));
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);

}

void CustomGLWidget::resetView(){
    makeCurrent();
    xClick = yClick = 0;
    angleY = 0.0;
    angleX = 7*M_PI/18;
    moveX = 0;
    moveZ = 0;
    zoom = 1.0;
    elevation = true;
    DoingInteractive = NONE;
    projectTransform();
    viewTransform();
    update();
}



void CustomGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::RightButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = PANNING;
  }
}

void CustomGLWidget::wheelEvent(QWheelEvent *event){
    makeCurrent();
    QPoint numDegrees = event->angleDelta();
    if (numDegrees.y() < 0)  zoom *= 1.1;
    if (numDegrees.y() > 0)  zoom /= 1.1;
    if (zoom > 2.0f) zoom = 2.0f;
    else if (zoom < 0.05f) zoom = 0.05f;
    projectTransform();
    viewTransform();
    update();
}

void CustomGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void CustomGLWidget::setElevation(){
    if (enabled){
        makeCurrent();
        elevation = !elevation;
        grid->setElevation(elevation);
        update();
    }

}

void CustomGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATE){
       angleY += (e->x() - xClick) * M_PI / 180.0*0.5;
       angleX += (e->y() - yClick) * M_PI / 180.0*0.5;
       if (angleX > M_PI/2) angleX = M_PI/2;
       else if (angleX < 0) angleX = 0;
       viewTransform ();
  }
  else if (DoingInteractive == PANNING) {
      moveX += (e->x() - xClick) * 0.03 * zoom;
      moveZ -= (e->y() - yClick) * 0.03 * zoom;
      if (moveX > 6) moveX = 6;
      else if(moveX < -6) moveX = -6;
      if (moveZ > 4) moveZ = 4;
      else if(moveZ < -4) moveZ = -4;
      viewTransform ();
  }
  xClick = e->x();
  yClick = e->y();

  update();
}

