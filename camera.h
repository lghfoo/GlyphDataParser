#ifndef CAMERA_H
#define CAMERA_H
#include<qvector3d.h>
#include<qquaternion.h>
#include<qmatrix4x4.h>
class Camera{
    QVector3D translation;
    QQuaternion rotation;
    QMatrix4x4 matrix;
    bool dirty;
public:
    Camera(){
        dirty = false;
    }
    QMatrix4x4& getMatrix(){
        if(dirty){
            matrix.setToIdentity();
            matrix.rotate(rotation.conjugated());
            matrix.translate(-translation);
        }
        return matrix;
    }
    void rotate(QQuaternion rot){
        rotation = rot * rotation;
        dirty = true;
    }
    void translate(QVector3D trans){
        translation += trans;
        dirty = true;
    }
    QVector3D forward(){
        return rotation.rotatedVector(QVector3D(0, 0, -1));
    }
    QVector3D right(){
        return rotation.rotatedVector(QVector3D(1, 0, 0));
    }
    QVector3D up(){
        return rotation.rotatedVector(QVector3D(0, 1, 0));
    }
};

#endif // CAMERA_H
