#include "openglpanel.h"
#include <iostream>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

#include "scene/cube.h"
#include "scene/meshsphere.h"
#include "QGuiApplication"
#include <QTimer>

using namespace graphics;
using namespace scene;

OpenGLPanel::OpenGLPanel(QWidget *parent)
     : QOpenGLWidget(parent)
{
    // opengl context version
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);

    setFormat(format);

    // logger
    this->logger = new QOpenGLDebugLogger(this);
    connect(this->logger, &QOpenGLDebugLogger::messageLogged, this, &OpenGLPanel::handleOpenglMessage);

    // loop timer
    this->timer = new QTimer(this);
    connect(this->timer, &QTimer::timeout, this, &OpenGLPanel::Redraw);

    // input
    this->handler = new InputHandler();
    installEventFilter(this->handler);

    setMouseTracking(true);
    grabKeyboard();
}

// called every cycle of timer
void OpenGLPanel::Redraw() {
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

    // calls function paintGL
    this->update();
}

void OpenGLPanel::initializeGL() {
    logger->initialize();
    logger->startLogging();

    initializeOpenGLFunctions();

    this->renderer = new Renderer();
    this->tracer = new Tracer();
    this->scene = new Scene();

    MeshSphere* s = new MeshSphere(20, 20);
    s->RotateX(-90.0f);
    s->Move(0.0f, 0.0f, -2.1f);
    //s->GetMesh().SetWireFrame(true);
    scene->AddSphere(new Sphere({0.0f, 0.0f, 0.0f}, 0.5f));
    scene->AddSphere(new Sphere({0.0f, -0.7f, 0.0f}, 0.25f));
    scene->AddSphere(new Sphere({0.0f, 1.0f, 0.0f}, 0.7f));
    scene->AddSphere(new Sphere({1.0f, 2.0f, -7.0f}, 0.4f));
    scene->AddSphere(new Sphere({2.0f, 2.0f, -5.0f}, 0.33f));
    scene->AddSphere(new Sphere({3.0f, 4.0f, 0.0f}, 0.21f));
    scene->AddObject(s);

    /*
    Cube* cube;
    for(int i = 0; i < 10; i++) {
        for(int o = 0; o < 10; o++) {
            cube = new Cube();
            cube->Scale(0.1f);
            cube->Move(-0.5f + i * 0.1f, -0.5f + o * 0.1f, -1.0f);
            scene->AddObject(cube);
        }
    }
    */

    this->cam = new Camera();
    this->cam->fov = 70;
    this->cam->aspectRatio = width() / height();
    this->cam->nearPlane = 0.1f;
    this->cam->farPlane = 100.0f;

    timer->start(100);
}

// *OpenGL functions must be called from this function
void OpenGLPanel::paintGL() {
    if(this->handler->IsKeyPressed(Qt::Key_T) && !traced) {
        tracer->Prepare();
        tracer->Draw(*(this->scene), *cam);
        traced = true;
    } else {
        this->traced = false;
    }

    renderer->Prepare();
    renderer->SetPolygonMode(GL_FILL);
    renderer->Clear();
    renderer->Draw(*(this->scene), *cam);

    if(tracer->HasRays()) {
        renderer->SetPolygonMode(GL_LINE);
        renderer->Draw(tracer->GetRays(), *cam);
    }
}

// if component is resized
void OpenGLPanel::resizeGL(int w, int h) {
    cam->aspectRatio = (float)w / (float)h;
}

void OpenGLPanel::handleOpenglMessage(const QOpenGLDebugMessage &debugMessage) {
    std::cout << debugMessage.message().toStdString() << std::endl;
}
