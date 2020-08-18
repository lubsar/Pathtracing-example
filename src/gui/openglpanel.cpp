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

    this->handler = new InputHandler();
    installEventFilter(this->handler);

    setMouseTracking(true);
    grabKeyboard();
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
    this->scene = new Scene();

    Cube* cube;
    for(int i = 0; i < 10; i++) {
        for(int o = 0; o < 10; o++) {
            cube = new Cube();
            cube->Scale(0.1f);
            cube->Move(-0.5f + i * 0.1f, -0.5f + o * 0.1f, -1.0f);
            scene->AddObject(cube);
        }
    }

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

    float speed = 0.1f;

    float xMove = 0.0f;
    float yMove = 0.0f;
    float zMove = 0.0f;

    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;

    if(handler->IsKeyPressed(Qt::Key_W)) {
        zMove -= speed;
    }
    if(handler->IsKeyPressed(Qt::Key_S)) {
        zMove += speed;
    }
    if(handler->IsKeyPressed(Qt::Key_A)) {
        xMove -= speed;
    }
    if(handler->IsKeyPressed(Qt::Key_D)) {
        xMove += speed;
    }
    if(handler->IsKeyPressed(Qt::Key_Right)) {
        pitch += 2;
    }
    if(handler->IsKeyPressed(Qt::Key_Left)) {
        pitch -= 2;
    }
    if(handler->IsKeyPressed(Qt::Key_Up)) {
        yaw += 2;
    }
    if(handler->IsKeyPressed(Qt::Key_Down)) {
        yaw -= 2;
    }
    if(handler->IsKeyPressed(Qt::Key_Shift)) {
        yMove -= speed;
    }
    if(handler->IsKeyPressed(Qt::Key_Space)) {
        yMove += speed;
    }

    this->cam->Move(xMove, yMove, zMove);
    this->cam->pitch += pitch;
    this->cam->yaw += yaw;

    /*
    this->cube->RotateY(5);
    this->cube->RotateX(3.2f);
    this->cube->Move(0.0f, 0.0f, -0.05f);
    */

    renderer->Clear();
    renderer->Draw(*(this->scene), *cam);
}

void OpenGLPanel::resizeGL(int w, int h) {
    cam->aspectRatio = (float)w / (float)h;
    //this->program->LoadUniformMat4x4(projection_location, cam->CalculateProjectionMatrix());
}
