﻿#include "view.h"

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>

#include "scene/Scene.h"

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_time(), m_timer(), m_captureMouse(false)
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor
    if (m_captureMouse) {
        QApplication::setOverrideCursor(Qt::BlankCursor);
    }

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The update loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

View::~View()
{
}

void View::initializeGL() {
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    // initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Something is very wrong, glew initialization failed." << std::endl;
    }
    std::cout << "Using GLEW " <<  glewGetString( GLEW_VERSION ) << std::endl;

    // setting OpenGL settings
    glEnable(GL_DEPTH_TEST);
    // for the grass trick to work we cannot enable backface culling
    glFrontFace(GL_CCW);

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

    initializeScene();
}

void View::initializeScene() {
    m_scene = std::make_unique<Scene>();
}

void View::paintGL() {
    m_scene->render();
}

void View::resizeGL(int w, int h) {
    float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
    w = static_cast<int>(w / ratio);
    h = static_cast<int>(h / ratio);
    glViewport(0, 0, w, h);
    m_scene->getCamera()->setAspectRatio(static_cast<float>(width()) / static_cast<float>(height()));
    m_scene->updateDimensions(w, h);
    m_scene->render();
}

void View::mousePressEvent(QMouseEvent *event) {

}

void View::mouseMoveEvent(QMouseEvent *event) {
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    if (m_captureMouse) {
        int deltaX = event->x() - width() / 2;
        int deltaY = event->y() - height() / 2;
        if (!deltaX && !deltaY) return;
        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

        // TODO: Handle mouse movements here
    }
}

void View::mouseReleaseEvent(QMouseEvent *event) {

}

void View::keyPressEvent(QKeyEvent *event) {
    int key = event->key();

    if (key == Qt::Key_Escape) QApplication::quit();

    CamtransCamera *camera = m_scene->getCamera();
    switch (key) {
        case Qt::Key_Up:
            camera->rotateU(1.f);
            break;
        case Qt::Key_Down:
            camera->rotateU(-1.f);
            break;
        case Qt::Key_Right:
            camera->rotateV(-1.f);
            break;
        case Qt::Key_Left:
            camera->rotateV(1.f);
            break;
    }

    if (key == Qt::Key_W || key == Qt::Key_S) {
        glm::vec4 pos = camera->getPosition();
        glm::vec4 dir = -camera->getW();
        glm::vec4 delta;
        if (key == Qt::Key_W) {
            delta = -camera->getW();
        } else {
            delta = camera->getW();
        }
        camera->orientLook(
                    pos+delta,
                    dir,
                    camera->getV());
    }

    update();
}

void View::keyReleaseEvent(QKeyEvent *event) {

}

void View::tick() {
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = m_time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    // update();
}
