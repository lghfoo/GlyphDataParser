#include"globalinfo.h"
Camera GlobalInfo::camera = Camera();
QMatrix4x4 GlobalInfo::projection = QMatrix4x4();
GLint GlobalInfo::u_camera = -1;
GLint GlobalInfo::u_projection = -1;
GLuint GlobalInfo::programID = -1;
