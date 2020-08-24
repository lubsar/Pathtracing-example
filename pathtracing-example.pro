QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11, file_copies

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    libs\glm\glm \
    src

SOURCES += \
    src/graphics/glslprogram.cpp \
    src/graphics/renderer.cpp \
    src/graphics/texturedquad.cpp \
    src/graphics/tracer.cpp \
    src/gui/inputhandler.cpp \
    src/gui/mainwindow.cpp \
    src/gui/openglpanel.cpp \
    src/scene/camera.cpp \
    src/scene/cube.cpp \
    src/scene/mesh.cpp \
    src/scene/meshsphere.cpp \
    src/scene/object.cpp \
    src/scene/scene.cpp \
    src/scene/sphere.cpp \
    src/scene/sphericallight.cpp \
    src/util/maths.cpp \
    src/util/shader.cpp \
    src\main.cpp

HEADERS += \
    src/graphics/glslprogram.h \
    src/graphics/renderer.h \
    src/graphics/texturedquad.h \
    src/graphics/tracer.h \
    src/gui/inputhandler.h \
    src/gui/mainwindow.h \
    src/gui/openglpanel.h \
    src/scene/camera.h \
    src/scene/cube.h \
    src/scene/mesh.h \
    src/scene/meshsphere.h \
    src/scene/object.h \
    src/scene/scene.h \
    src/scene/sphere.h \
    src/scene/sphericallight.h \
    src/util/maths.h \
    src/util/shader.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shaders/renderer.frag \
    shaders/renderer.vert \
    shaders/renderer_instanced.vert \
    shaders/renderer_texture.frag \
    shaders/renderer_texture.vert \
    shaders/tracer.glsl

COPIES += shaders

shaders.files = $$files(shaders/*)
shaders.path = $$OUT_PWD/shaders

FORMS += \
    src/gui/mainwindow.ui
