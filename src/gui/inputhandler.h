#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <QObject>
#include <QEvent>

#include <set>

class InputHandler : public QObject
{
    Q_OBJECT

private:
    std::set<int> m_pressedKeys;
    std::set<int> m_pressedButtons;
    std::set<int> m_clickedKeys;

    int m_mouseX;
    int m_mouseY;

protected:
    bool eventFilter(QObject* obj, QEvent* event);

public:
    InputHandler();

    bool IsKeyPressed(Qt::Key key);
    bool IsKeyClicked(Qt::Key key);

    bool IsMousePressed(Qt::MouseButton button);

    int GetMouseX();
    int GetMouseY();
};

#endif // KEYBOARDHANDLER_H
