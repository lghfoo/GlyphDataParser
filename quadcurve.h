#ifndef QUADCURVE_H
#define QUADCURVE_H
#include<qvector2d.h>
#include"shader.h"
#include"gmath.h"
#include<qopenglfunctions_4_3_core.h>
class QuadCurve{
    QVector2D* points;
    GLuint objID;
    GLuint posBufID;
    GLuint programID;

    QVector2D* samples;
    int samplesNum;
public:
    QuadCurve(){}
    QuadCurve(QVector2D p0, QVector2D p1, QVector2D p2){
        points = new QVector2D[3];
        points[0] = p0;
        points[1] = p1;
        points[2] = p2;

        samplesNum = 32;
        BezierCurve2D* curve = new BezierCurve2D(points, 3);
        samples = curve->getSamples(samplesNum);
    }

    void init(QOpenGLFunctions_4_3_Core* func){
        func->glGenVertexArrays(1, &objID);
        func->glBindVertexArray(objID);

        func->glGenBuffers(1, &posBufID);
        func->glBindBuffer(GL_ARRAY_BUFFER, posBufID);
        func->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D) * (samplesNum + 2), samples, GL_STATIC_DRAW);
        func->glEnableVertexAttribArray(0);
        func->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        programID = Shader::LoadShaders(func, ":/shaders/vertex_shader.vert", ":/shaders/fragment_shader.frag");
        func->glUseProgram(programID);
    }

    void paint(QOpenGLFunctions_4_3_Core* func){
        func->glBindVertexArray(objID);
        func->glBindBuffer(GL_ARRAY_BUFFER, posBufID);
        func->glEnableVertexAttribArray(0);
        func->glDrawArrays(GL_LINE_STRIP, 0, samplesNum + 2);
    }
};

#endif // QUADCURVE_H
