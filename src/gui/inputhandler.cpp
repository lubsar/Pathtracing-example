#include "inputhandler.h"

#include <algorithm>
#include <QKeyEvent>
#include <QMouseEvent>

InputHandler::InputHandler() : QObject()
{
    this->pressedKeys = std::set<int>();
}

bool InputHandler::eventFilter(QObject* obj, QEvent* event)
{
    switch(event->type()) {
        case QEvent::KeyPress: {
            this->pressedKeys.insert(static_cast<QKeyEvent*>(event)->key());
            return true;
        }
        break;
        case QEvent::KeyRelease: {
            this->pressedKeys.erase(static_cast<QKeyEvent*>(event)->key());
            return true;
        }
        break;
        case QEvent::MouseMove: {
            QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);

            this->mouseX = mouse_event->x();
            this->mouseY = mouse_event->y();

            return true;
        }
        break;
        case QEvent::MouseButtonPress: {
            this->pressedButtons.insert(static_cast<QMouseEvent*>(event)->button());
            return true;
        }
        break;
        case QEvent::MouseButtonRelease: {
            this->pressedButtons.erase(static_cast<QMouseEvent*>(event)->button());
            return true;
        }
        break;
        default:
            return QObject::eventFilter(obj, event);
    }
}

bool InputHandler::IsKeyPressed(Qt::Key key) {
    return this->pressedKeys.find(key) != this->pressedKeys.end();
}

bool InputHandler::IsMousePressed(Qt::MouseButton button) {
    return this->pressedButtons.find(button) != this->pressedButtons.end();
}

int InputHandler::GetMouseX() {
    return this->mouseX;
}

int InputHandler::GetMouseY() {
    return this->mouseY;
}
