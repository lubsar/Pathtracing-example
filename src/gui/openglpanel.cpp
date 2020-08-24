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
    this->m_logger = new QOpenGLDebugLogger(this);
    connect(this->m_logger, &QOpenGLDebugLogger::messageLogged, this, &OpenGLPanel::handleOpenglMessage);

    // loop timer
    this->m_timer = new QTimer(this);
    connect(this->m_timer, &QTimer::timeout, this, &OpenGLPanel::Update);

    // input
    this->m_input = new InputHandler();
    installEventFilter(this->m_input);

    setMouseTracking(true);
    grabKeyboard();
}

OpenGLPanel::~OpenGLPanel() {
    delete this->m_renderer;
    delete this->m_tracer;
    //delete this->m_logger; // context erro

    delete this->m_timer;
    delete this->m_input;

    delete this->m_cam;
    delete this->m_scene;
}

// called every cycle of timer
void OpenGLPanel::Update() {
    // camera
    float speed = 0.1f;

    float xMove = 0.0f;
    float yMove = 0.0f;
    float zMove = 0.0f;

    float pitch = 0.0f;
    //float roll = 0.0f;
    float yaw = 0.0f;

    if(m_input->IsKeyPressed(Qt::Key_W)) {
        zMove -= speed;
    }
    if(m_input->IsKeyPressed(Qt::Key_S)) {
        zMove += speed;
    }
    if(m_input->IsKeyPressed(Qt::Key_A)) {
        xMove -= speed;
    }
    if(m_input->IsKeyPressed(Qt::Key_D)) {
        xMove += speed;
    }
    if(m_input->IsKeyPressed(Qt::Key_Up)) {
        pitch += 2;
    }
    if(m_input->IsKeyPressed(Qt::Key_Down)) {
        pitch -= 2;
    }
    if(m_input->IsKeyPressed(Qt::Key_Right)) {
        yaw += 2;
    }
    if(m_input->IsKeyPressed(Qt::Key_Left)) {
        yaw -= 2;
    }
    if(m_input->IsKeyPressed(Qt::Key_Shift)) {
        yMove -= speed;
    }
    if(m_input->IsKeyPressed(Qt::Key_Space)) {
        yMove += speed;
    }

    this->m_cam->Move(xMove, yMove, zMove);
    this->m_cam->pitch += pitch;
    this->m_cam->yaw += yaw;

    // other

    if(m_input->IsKeyClicked(Qt::Key_T)) {
        this->m_trace = true;
    }
    if(m_input->IsKeyClicked(Qt::Key_R)) {
        this->m_showRays = !this->m_showRays;
    }
    if(m_input->IsKeyClicked(Qt::Key_F)) {
        this->m_showFrustum = !this->m_showFrustum;
    }
    if(m_input->IsKeyClicked(Qt::Key_I)) {
        this->m_showResult = !this->m_showResult;
    }

    // calls function paintGL
    this->update();
}

void OpenGLPanel::initializeGL() {
    this->m_logger->initialize();
    this->m_logger->startLogging();

    initializeOpenGLFunctions();

    this->m_renderer = new Renderer();
    this->m_tracer = new Tracer();
    this->m_scene = new Scene();

    this->m_scene->AddSphere(new Sphere({-0.4f, 0.3f, -2.0f}, 0.12f, {1.0f, 0.0f, 0.1f}));
    this->m_scene->AddSphere(new Sphere({0.0f, -0.7f, 1.0f}, 0.25f, {1.0f, 1.0f, 0.0f}));
    this->m_scene->AddSphere(new Sphere({0.0f, 1.0f, 0.5f}, 0.4f, {0.0f, 1.0f, 0.0f}));
    this->m_scene->AddSphere(new Sphere({-1.0f, 2.0f, -7.0f}, 0.4f, {0.0f, 0.7f, 1.0f}));
    this->m_scene->AddSphere(new Sphere({2.0f, 2.0f, -5.0f}, 0.33f, {1.0f, 1.0f, 1.0f}));
    this->m_scene->AddSphere(new Sphere({3.0f, 4.0f, -1.37f}, 0.21f, {0.5f, 1.0f, 0.0f}));
    this->m_scene->AddSphere(new Sphere({-0.2f, -0.1f, -0.9f}, 0.2f, {1.0f, 1.0f, 0.3f}));
    this->m_scene->AddSphere(new Sphere({1.0f, -1.350f, -5.0f}, 0.6f, {0.5f, 0.7f, 0.5f}));
    this->m_scene->AddSphere(new Sphere({-3.0f, -1.0f, -1.37f}, 0.35f, {0.9f, 1.0f, 0.34f}));
    this->m_scene->AddSphere(new Sphere({-0.2f, -0.5f, -0.9f}, 0.35f, {1.0f, 0.0f, 0.3f}));
    this->m_scene->AddSphere(new Sphere({1.0f, -0.7f, -5.0f}, 0.11f, {0.5f, 0.7f, 0.12f}));
    this->m_scene->AddSphere(new Sphere({2.5, 0.0f, -2.5f}, 0.45f, {0.3f, 1.0f, 0.34f}));
    this->m_scene->AddSphere(new Sphere({-3.0f, -0.35f, -1.37f}, 0.45f, {0.3f, 1.0f, 0.34f}));

    //scene->SetLight(new SphericalLight({1.5, 0.5f, 5.0f}, {1.0f, 1.0f, 1.0f}, 10));
    this->m_scene->SetLight(new SphericalLight({1.5, 0.0f, -2.0f}, {1.0f, 1.0f, 1.0f}, 10));

    this->m_cam = new Camera();
    this->m_cam->fov = 70;
    this->m_cam->aspectRatio = width() / height();
    this->m_cam->nearPlane = 0.1f;
    this->m_cam->farPlane = 100.0f;

    this->m_timer->start(100);
}

// *OpenGL functions must be called from here
void OpenGLPanel::paintGL() {
    if(this->m_trace) {
        this->m_tracer->Draw(*this->m_scene, *this->m_cam, width(), height());
        this->m_traced = true;
        this->m_trace = false;
    }

    this->m_renderer->Prepare();
    this->m_renderer->SetPolygonMode(GL_FILL);
    this->m_renderer->Clear();
    this->m_renderer->Draw(*this->m_scene, *this->m_cam);

    if(m_tracer->HasResult()) {
        if(this->m_showRays) {
            this->m_renderer->SetPolygonMode(GL_LINE);
            m_renderer->Draw(this->m_tracer->GetRays(), *this->m_cam);
        }

        if(this->m_showResult) {
            this->m_renderer->SetPolygonMode(GL_FILL);
            this->m_renderer->DrawTracerResult(this->m_tracer->GetResultTexture());
        }
    }
}

// when component is resized
void OpenGLPanel::resizeGL(int w, int h) {
    m_cam->aspectRatio = (float)w / (float)h;
}

void OpenGLPanel::handleOpenglMessage(const QOpenGLDebugMessage &debugMessage) {
    std::cout << debugMessage.message().toStdString() << std::endl;
}
