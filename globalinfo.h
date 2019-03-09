#ifndef GLOBALINFO_H
#define GLOBALINFO_H
#include"camera.h"
#include<qopenglfunctions_4_3_core.h>
class GlobalInfo{
public:
    static Camera camera;
    static QMatrix4x4 projection;
    static GLint u_camera;
    static GLint u_projection;
    static GLuint programID;
};

#endif // GLOBALINFO_H
