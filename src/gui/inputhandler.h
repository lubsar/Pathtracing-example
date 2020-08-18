#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <QObject>
#include <QEvent>

#include <set>

class InputHandler : public QObject
{
    Q_OBJECT

private:
    std::set<int> pressedKeys;
    std::set<int> pressedButtons;

    int mouseX;
    int mouseY;

protected:
    bool eventFilter(QObject* obj, QEvent* event);

public:
    InputHandler();

    bool IsKeyPressed(Qt::Key key);

    bool IsMousePressed(Qt::MouseButton button);
    int GetMouseX();
    int GetMouseY();
};

#endif // KEYBOARDHANDLER_H
