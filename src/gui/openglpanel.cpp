#include "openglpanel.h"
#include <iostream>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

#include "scene/cube.h"
#include "QGuiApplication"
#include <QTimer>

using namespace graphics;
using namespace scene;

OpenGLPanel::OpenGLPanel(QWidget *parent)
     : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);

    setFormat(format);

    this->logger = new QOpenGLDebugLogger(this);
    connect(this->logger, &QOpenGLDebugLogger::messageLogged, this, &OpenGLPanel::handleOpenglMessage);

    this->timer = new QTimer(this);
    connect(this->timer, &QTimer::timeout, this, &OpenGLPanel::Redraw);
}

void OpenGLPanel::Redraw() {
    this->update();
}

void OpenGLPanel::handleOpenglMessage(const QOpenGLDebugMessage &debugMessage) {
    std::cout << debugMessage.message().toStdString() << std::endl;
}

void OpenGLPanel::initializeGL() {
    logger->initialize();
    logger->startLogging();

    initializeOpenGLFunctions();

    this->renderer = new Rasterizer();

    this->cube = new Cube();
    std::cout << cube->GetMesh().LenIndices() << std::endl;
    this->cube->Scale(0.5f);
    this->cube->Move(0.0f, 0.0f, 5.0f);

    this->cam = new Camera();
    this->cam->fov = 70;
    this->cam->roll = 0.0f;
    this->cam->yaw = 0.0f;
    this->cam->pitch = 0.0f;
    this->cam->aspectRatio = width() / height();
    this->cam->nearPlane = 0.1f;
    this->cam->farPlane = 100.0f;
    this->cam->Move(0.0f, 0.0f, 0.0f);


    renderer->Init();
    timer->start(100);
}

void OpenGLPanel::paintGL() {
    this->time += 0.1f;

    this->cube->RotateY(5);
    this->cube->RotateX(3.2f);
    this->cube->Move(0.0f, 0.0f, -0.05f);

    renderer->Clear();
    renderer->Draw(*cube, *cam);
}

void OpenGLPanel::resizeGL(int w, int h) {
    cam->aspectRatio = (float)w / (float)h;
    //this->program->LoadUniformMat4x4(projection_location, cam->CalculateProjectionMatrix());
}
