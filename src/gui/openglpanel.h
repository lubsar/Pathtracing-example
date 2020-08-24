#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLDebugLogger>
#include <qopenglfunctions_4_5_core.h>

#include "inputhandler.h"
#include "graphics/tracer.h"

#include "scene/cube.h"
#include "scene/sphere.h"


class OpenGLPanel : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
private:
    graphics::Renderer* m_renderer;
    graphics::Tracer* m_tracer;
    QOpenGLDebugLogger* m_logger;

    InputHandler* m_input;

    scene::Camera* m_cam;
    scene::Scene* m_scene;

    bool m_traced;
    bool m_trace;
    bool m_showResult;
    bool m_showRays;
    bool m_showFrustum;

    QTimer* m_timer;

    void Update();
    void handleOpenglMessage(const QOpenGLDebugMessage &debugMessage);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

public:
    OpenGLPanel(QWidget* parent = nullptr);
    ~OpenGLPanel();
};

#endif // OPENGLPANEL_H
