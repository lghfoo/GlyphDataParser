#ifndef RECTANGLE_H
#define RECTANGLE_H
#include<qvector2d.h>
#include"shader.h"
#include<qopenglfunctions_4_3_core.h>
#include"globalinfo.h"
class GRectangle{
    QVector2D origin;
    float width;
    float height;

    GLuint objID;
    GLuint posBufID;
    GLuint programID;

    QVector2D* posBufData;
public:
    GRectangle(const QVector2D& origin,
              float w,
              float h){
        this->origin = origin;
        width = w;
        height = h;
        posBufData = new QVector2D[4];
        posBufData[0] = origin;
        posBufData[1] = origin + QVector2D(w, 0);
        posBufData[2] = origin + QVector2D(w, -h);
        posBufData[3] = origin + QVector2D(0, -h);
    }
    void init(QOpenGLFunctions_4_3_Core* func){
        func->glGenVertexArrays(1, &objID);
        func->glBindVertexArray(objID);

        func->glGenBuffers(1, &posBufID);
        func->glBindBuffer(GL_ARRAY_BUFFER, posBufID);
        func->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D) * 4, posBufData, GL_STATIC_DRAW);
        func->glEnableVertexAttribArray(0);
        func->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
    void paint(QOpenGLFunctions_4_3_Core* func){
        func->glBindVertexArray(objID);
        func->glBindBuffer(GL_ARRAY_BUFFER, posBufID);
        func->glEnableVertexAttribArray(0);
        func->glDrawArrays(GL_LINE_LOOP, 0, 4);
    }
};

#endif // RECTANGLE_H
