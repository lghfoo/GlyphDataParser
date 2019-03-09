#ifndef GMATH_H
#define GMATH_H
#include<qvector.h>
#include<qvector3d.h>
#include<qvector2d.h>
class GMathUtil{
public:
    static long long factorial(long long v){
        if(v == 0)return 1;
        long long ret = 1;
        for(int i = 1; i <= v; i++){
            ret *= i;
        }
        return ret;
    }

    static long long binomialCoefficient(long long n, long long i){
        return factorial(n) / (factorial(i) * factorial(n - i));
    }
};

class BezierCurve2D{
    QVector2D* points;
    int len;
public:
    BezierCurve2D(QVector2D* points, int len){
        this->points = points;
        this->len = len;
    }

    ~BezierCurve2D(){
        delete[]points;
    }

    BezierCurve2D(QVector2D p0,
                  QVector2D p1,
                  QVector2D p2){
        this->points = new QVector2D[3];
        this->len = 3;
        points[0] = p0;
        points[1] = p1;
        points[2] = p2;
    }

    //samplesNum: num of control points
    QVector2D* getSamples(int samplesNum){
        float dt = 1.0 / float(samplesNum + 1);
        QVector2D* samples = new QVector2D[samplesNum + 2];
        for(int i = 0; i < samplesNum + 2; i++){
            float t = dt * float(i);
            if(i == samplesNum + 1)t = 1.0;
            samples[i] = getValue(t);
        }
        return samples;
    }

    QVector2D getValue(float t){
        QVector2D ret = QVector2D(0.0, 0.0);
        for(int i = 0; i < len; i++){
            ret += float(GMathUtil::binomialCoefficient(len - 1, i)) * pow(t, i) * pow(1 - t, len - 1 - i) * points[i];
        }
        return ret;
    }
};

#endif // GMATH_H

