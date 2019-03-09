#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<qopenglwindow.h>
#include<qlayout.h>
#include<qtextedit.h>
#include<qwidget.h>
#include"gmath.h"
#include"qdebug.h"
#include"quadcurve.h"
#include"glyphdataparser.h"
#include"glyph.h"
#include<qopenglfunctions_4_3_core.h>
#include<qevent.h>
#include"globalinfo.h"
#include"inputmanager.h"
#include"grectangle.h"
class MainWindow: public QOpenGLWindow, public QOpenGLFunctions_4_3_Core{
    Q_OBJECT
public:
    GlyphDataParser* parser;
    QVector<Glyph*> glyph;
    GRectangle* rect;
    int curPos;
    InputManager* inputManager;
    QWidget* widget;
    QTextEdit* textEdit;
    QString curCode;
    MainWindow(){
        QSurfaceFormat format;
        format.setSamples(4);
        this->setFormat(format);
        this->resize(800, 600);
        inputManager = new InputManager();
        rect = new GRectangle(QVector2D(-1.0, 1.0), 2.0, 2.0);
        GlobalInfo::camera.translate(QVector3D(0.0, 0, 5.0));
        parser = new GlyphDataParser(":/glyphs/glyphs");
        curCode = "";


        widget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout();
        textEdit = new QTextEdit();
        layout->addWidget(textEdit);
        widget->setLayout(layout);
        widget->show();
    }



    void initializeGL()Q_DECL_OVERRIDE{
        this->initializeOpenGLFunctions();
        connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
        this->glClearColor(0.0, 0.0, 0.0, 1.0);
        for(int i = 0; i < glyph.size(); i++){
            glyph[i]->init(this);
        }
//        rect->init(this);

        setPos(0);

        GlobalInfo::programID = Shader::LoadShaders(this, ":/shaders/vertex_shader.vert", ":/shaders/fragment_shader.frag");
        glUseProgram(GlobalInfo::programID);

        GlobalInfo::u_camera = glGetUniformLocation(GlobalInfo::programID, "camera");
        GlobalInfo::u_projection = glGetUniformLocation(GlobalInfo::programID, "projection");
    }

    void paintGL()Q_DECL_OVERRIDE{
        this->glClear(GL_COLOR_BUFFER_BIT);
        glUniformMatrix4fv(GlobalInfo::u_camera, 1, GL_FALSE, GlobalInfo::camera.getMatrix().data());
        glUniformMatrix4fv(GlobalInfo::u_projection, 1, GL_FALSE, GlobalInfo::projection.data());
        for(auto g : glyph){
            if(!g->isInited)g->init(this);
            g->paint(this);
        }
//        rect->paint(this);
    }

    void resizeGL(int w, int h)Q_DECL_OVERRIDE{
        GlobalInfo::projection.setToIdentity();
        GlobalInfo::projection.perspective(45, float(w) / float(h), 0.1, 1000);
    }

protected:
    void mousePressEvent(QMouseEvent *event)Q_DECL_OVERRIDE
    {
        inputManager->onMousePressEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent* event)Q_DECL_OVERRIDE{
        inputManager->onMouseReleaseEvent(event);
    }
    void keyPressEvent(QKeyEvent* event)Q_DECL_OVERRIDE{
        if(event->key() == Qt::Key_Right){
            this->setPos((curPos + 1) % parser->datas.size());
        }
        else if(event->key() == Qt::Key_Left){
            this->setPos((curPos - 1 + parser->datas.size()) % parser->datas.size());
        }
        inputManager->onKeyPressEvent(event);
    }
    void keyReleaseEvent(QKeyEvent* event)Q_DECL_OVERRIDE{
        inputManager->onKeyReleaseEvent(event);
    }

    void setPos(int pos){
        this->curPos = pos;
        for(auto g : glyph){
            g->destroy(this);
            delete g;
        }
        glyph.clear();
        if(parser->datas[curPos] == nullptr){
            qDebug()<<"null";
            return;
        }
        QVector<SimpleGlyphData*>datas = parser->datas[curPos]->getDatas();
        for(auto data : datas){
//            if(data->translation != nullptr)
//            qDebug()<<(*(data->translation));
            glyph.push_back(new Glyph(data));
        }
    }
public slots:
    void update(){

        QString code = textEdit->document()->toRawText();
        if(code.size() > 0){
            uint16 x = code[0].unicode();
            int index = parser->mapper[x];
            if(index == -1){
                qDebug()<<"can't find glyph";
            }
            else{
                if(index != curPos){
                    setPos(index);

                }
            }
        }


        inputManager->update();
        float rotSpeed = 0.1f;
        if(inputManager->isButtonPressed[InputManager::LEFT_BTN]){
            GlobalInfo::camera.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), -rotSpeed * inputManager->mouseDelta().x()));
            GlobalInfo::camera.rotate(QQuaternion::fromAxisAndAngle(GlobalInfo::camera.right(), -rotSpeed * inputManager->mouseDelta().y()));
        }

        QVector3D translation;
        Camera* camera = &(GlobalInfo::camera);
        if(inputManager->isKeyPressed[InputManager::KEY_W]){
            translation += camera->forward();
        }
        if(inputManager->isKeyPressed[InputManager::KEY_S]){
            translation -= camera->forward();
        }
        if(inputManager->isKeyPressed[InputManager::KEY_A]){
            translation -= camera->right();
        }
        if(inputManager->isKeyPressed[InputManager::KEY_D]){
            translation += camera->right();
        }
        if(inputManager->isKeyPressed[InputManager::KEY_E]){
            translation += camera->up();
        }
        if(inputManager->isKeyPressed[InputManager::KEY_Q]){
            translation -= camera->up();
        }
        float transSpeed = 0.1f;
        camera->translate(translation * transSpeed);
        QOpenGLWindow::update();//important!!!
    }
};

#endif // MAINWINDOW_H
