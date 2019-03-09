#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include<qevent.h>
#include<qcursor.h>
class InputManager{
public:
    enum{KEY_W, KEY_S, KEY_A, KEY_D, KEY_Q, KEY_E, KEY_NUM};
    enum{LEFT_BTN, MID_BTN, RIGHT_BTN, BTN_NUM};
    bool isKeyPressed[KEY_NUM];
    bool isButtonPressed[BTN_NUM];

    QPoint* prevMousePos;
    QPoint* curMousePos;
    InputManager(){
        memset(isKeyPressed, 0, sizeof(isKeyPressed));
        memset(isButtonPressed, 0, sizeof(isButtonPressed));
        prevMousePos = curMousePos = nullptr;
    }

    void update(){
        QPoint curPos = QCursor::pos();
        if(curMousePos != nullptr){
            if(prevMousePos == nullptr){
                prevMousePos = new QPoint(curMousePos->x(), curMousePos->y());
            }
            else{
                prevMousePos->setX(curMousePos->x());
                prevMousePos->setY(curMousePos->y());
            }
            curMousePos->setX(curPos.x());
            curMousePos->setY(curPos.y());
        }
        else{
            curMousePos = new QPoint(curPos.x(), curPos.y());
        }
    }

    QPoint mouseDelta(){
        if(curMousePos != nullptr && prevMousePos != nullptr){
            return (*curMousePos) - (*prevMousePos);
        }
        return QPoint(0, 0);
    }

    void onKeyPressEvent(QKeyEvent* event){
        if(event->key() == Qt::Key_W){
            isKeyPressed[KEY_W] = true;
        }
        else if(event->key() == Qt::Key_S){
            isKeyPressed[KEY_S] = true;
        }
        else if(event->key() == Qt::Key_A){
            isKeyPressed[KEY_A] = true;
        }
        else if(event->key() == Qt::Key_D){
            isKeyPressed[KEY_D] = true;
        }
        else if(event->key() == Qt::Key_Q){
            isKeyPressed[KEY_Q] = true;
        }
        else if(event->key() == Qt::Key_E){
            isKeyPressed[KEY_E] = true;
        }
    }

    void onKeyReleaseEvent(QKeyEvent* event){
        if(event->key() == Qt::Key_W){
            isKeyPressed[KEY_W] = false;
        }
        else if(event->key() == Qt::Key_S){
            isKeyPressed[KEY_S] = false;
        }
        else if(event->key() == Qt::Key_A){
            isKeyPressed[KEY_A] = false;
        }
        else if(event->key() == Qt::Key_D){
            isKeyPressed[KEY_D] = false;
        }
        else if(event->key() == Qt::Key_Q){
            isKeyPressed[KEY_Q] = false;
        }
        else if(event->key() == Qt::Key_E){
            isKeyPressed[KEY_E] = false;
        }
    }

    void onMousePressEvent(QMouseEvent* event){
        if(event->button() == Qt::LeftButton){
            isButtonPressed[LEFT_BTN] = true;
        }
        else if(event->button() == Qt::RightButton){
            isButtonPressed[RIGHT_BTN] = true;
        }
        else if(event->button() == Qt::MiddleButton){
            isButtonPressed[MID_BTN] = true;
        }
    }

    void onMouseReleaseEvent(QMouseEvent* event){
        if(event->button() == Qt::LeftButton){
            isButtonPressed[LEFT_BTN] = false;
        }
        else if(event->button() == Qt::RightButton){
            isButtonPressed[RIGHT_BTN] = false;
        }
        else if(event->button() == Qt::MiddleButton){
            isButtonPressed[MID_BTN] = false;
        }
    }
};

#endif // INPUTMANAGER_H
