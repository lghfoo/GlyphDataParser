#ifndef GLYPH_H
#define GLYPH_H
#include<qopenglfunctions_4_3_core.h>
#include"glyphdataparser.h"
#include"shader.h"
#include"globalinfo.h"
class Glyph{
    SimpleGlyphData* data;
    GLuint objID;
    GLuint posBufID;
    GLuint programID;
    GLint u_translation;
public:
    bool isInited;
    Glyph(SimpleGlyphData* data){
        this->data = data;
        isInited = false;
    }

    void destroy(QOpenGLFunctions_4_3_Core* func){
        func->glDeleteVertexArrays(1, &objID);
        func->glDeleteBuffers(1, &posBufID);
    }

    void init(QOpenGLFunctions_4_3_Core* func){
        isInited = true;
        func->glGenVertexArrays(1, &objID);
        func->glBindVertexArray(objID);

        func->glGenBuffers(1, &posBufID);
        func->glBindBuffer(GL_ARRAY_BUFFER, posBufID);
        func->glBufferData(GL_ARRAY_BUFFER, data->dataLen * sizeof(QVector2D), data->data, GL_STATIC_DRAW);
        func->glEnableVertexAttribArray(0);
        func->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        u_translation = func->glGetUniformLocation(GlobalInfo::programID, "translation");
    }

    void paint(QOpenGLFunctions_4_3_Core* func){
        func->glBindVertexArray(objID);
        func->glBindBuffer(GL_ARRAY_BUFFER, posBufID);
        func->glEnableVertexAttribArray(0);
        if(data->translation != nullptr){
            func->glUniform2f(u_translation, data->translation->x(), data->translation->y());
        }
        else{
            func->glUniform2f(u_translation, 0.0, 0.0);
        }
        func->glMultiDrawArrays(GL_LINE_LOOP, data->first, data->count, data->contoursNum);
        if(data->translation != nullptr){
            func->glUniform2f(u_translation, 0.0, 0.0);
        }
    }

    void setData(SimpleGlyphData* data, QOpenGLFunctions_4_3_Core* func){
        this->data = data;
        func->glBindBuffer(GL_ARRAY_BUFFER, posBufID);
        func->glBufferData(GL_ARRAY_BUFFER, data->dataLen * sizeof(QVector2D), data->data, GL_STATIC_DRAW);
    }
};

#endif // GLYPH_H
