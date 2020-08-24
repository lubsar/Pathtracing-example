#include "inputhandler.h"

#include <algorithm>
#include <QKeyEvent>
#include <QMouseEvent>

InputHandler::InputHandler() : QObject()
{
    this->m_pressedKeys = std::set<int>();
}

bool InputHandler::eventFilter(QObject* obj, QEvent* event)
{
    switch(event->type()) {
        case QEvent::KeyPress: {
            this->m_pressedKeys.insert(static_cast<QKeyEvent*>(event)->key());
            return true;
        }
        break;
        case QEvent::KeyRelease: {
            int key = static_cast<QKeyEvent*>(event)->key();

            if(this->m_clickedKeys.find(key) != this->m_clickedKeys.end()) {
                this->m_clickedKeys.erase(key);
            }

            this->m_pressedKeys.erase(key);

            return true;
        }
        break;
        case QEvent::MouseMove: {
            QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);

            this->m_mouseX = mouse_event->x();
            this->m_mouseY = mouse_event->y();

            return true;
        }
        break;
        case QEvent::MouseButtonPress: {
            this->m_pressedButtons.insert(static_cast<QMouseEvent*>(event)->button());
            return true;
        }
        break;
        case QEvent::MouseButtonRelease: {
            this->m_pressedButtons.erase(static_cast<QMouseEvent*>(event)->button());
            return true;
        }
        break;
        default:
            return QObject::eventFilter(obj, event);
    }
}

bool InputHandler::IsKeyPressed(Qt::Key key) {
    return this->m_pressedKeys.find(key) != this->m_pressedKeys.end();
}

bool InputHandler::IsKeyClicked(Qt::Key key) {
    if(IsKeyPressed(key) && this->m_clickedKeys.find(key) == this->m_clickedKeys.end()) {
        this->m_clickedKeys.insert(key);
        return true;
    } else {
        return false;
    }
}

bool InputHandler::IsMousePressed(Qt::MouseButton button) {
    return this->m_pressedButtons.find(button) != this->m_pressedButtons.end();
}

int InputHandler::GetMouseX() {
    return this->m_mouseX;
}

int InputHandler::GetMouseY() {
    return this->m_mouseY;
}
