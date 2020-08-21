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

public:
    OpenGLPanel(QWidget* parent = nullptr);

private:
    graphics::Renderer* renderer;
    graphics::Tracer* tracer;
    QOpenGLDebugLogger* logger;

    InputHandler* handler;

    scene::Camera* cam;
    scene::Scene* scene;

    float time = 0.0f;
    bool traced;
    QTimer *timer;

    void Redraw();
    void handleOpenglMessage(const QOpenGLDebugMessage &debugMessage);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // OPENGLPANEL_H
