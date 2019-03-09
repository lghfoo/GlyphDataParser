#ifndef GLYPHDATAPARSER_H
#define GLYPHDATAPARSER_H
#include<qvector.h>
#include<qfile.h>
#include"gmath.h"
#include<qstring.h>
#include<qdebug.h>
#include<qvector2d.h>
#include<qopenglfunctions_4_3_core.h>

#define int8 char
#define int16 short
#define int32 int
#define uint8 unsigned char
#define uint16 unsigned short
#define uint32 unsigned int
#define Fixed int32 //32-bit signed fixed-point number (16.16)
#define FWORD int16  //describes a quantity in font design units
#define UFWORD uint16 //describes a quantity in font design units
#define F2DOT14 int16 //16-bit signed fixed number with the low 14 bits of fraction (2.14).
#define LONGDATETIME long long //Date represented in number of seconds since 12:00 midnight, January 1, 1904. The value is represented as a signed 64-bit integer.
#define Offset16 uint16 //Short offset to a table, same as uint16, NULL offset = 0x0000
#define Offset32 uint32 //Long offset to a table, same as uint32, NULL offset = 0x00000000

typedef uint8 Tag[4];//Array of four uint8s (length = 32 bits) used to identify a table, design-variation axis, script, language system, feature, or baseline
class SimpleGlyphData;
struct GlyphBase{
    virtual QVector<SimpleGlyphData*> getDatas() = 0;
};

struct SimpleGlyphData: GlyphBase{
    GLsizei contoursNum;
    GLint* first;
    GLsizei* count;
    QVector2D* data;
    GLuint dataLen;
    QVector2D* translation;
    SimpleGlyphData(){
        translation = nullptr;
    }
    virtual QVector<SimpleGlyphData*> getDatas() override{
        QVector<SimpleGlyphData*>ret;
        ret.clear();
        ret.push_back(this);
        return ret;
    }
};


struct CompositeGlyphComponent{
    uint16 glyphIndex;
    QVector2D* offset;
    QPoint* pinPoint;
    QVector<F2DOT14>scale;
    CompositeGlyphComponent(){
        offset = nullptr;
        pinPoint = nullptr;
        scale.clear();
    }
};

struct CompositeGlyphData: GlyphBase{
    QVector<GlyphBase*>*holder;
    uint16 componentNum;
    CompositeGlyphComponent** components;
    virtual QVector<SimpleGlyphData*>getDatas()override{
        QVector<SimpleGlyphData*>ret;
        ret.clear();
        for(int i = 0; i < componentNum; i++){
            ((SimpleGlyphData*)((*holder)[components[i]->glyphIndex]))->translation = components[i]->offset;
            ret.push_back((SimpleGlyphData*)((*holder)[components[i]->glyphIndex]));
        }
        return ret;
    }
};

class GlyphDataParser{
public:
    QVector<GlyphBase*>datas;
    uint16* mapper;
    GlyphDataParser(const char* filename){
        QFile file(filename);
        mapper = nullptr;
        if(!file.open(QIODevice::Text | QIODevice::ReadOnly)){
            qDebug()<<"can't open file \""<<filename<<"\"";
            return;
        }
#define SKIP QString::SplitBehavior::SkipEmptyParts
        QString all = file.readAll();
        QStringList lines = all.split("\n", SKIP);
        int cnt = 0;
        while(cnt < lines.size()){
            //contour
            if(lines[cnt].startsWith("con")){
                parseSimpleGlyph(lines, cnt);
            }
            //component
            else if(lines[cnt].startsWith("com")){
                parseCompositeGlyph(lines, cnt);
            }
            //pass
            else if(lines[cnt].startsWith("p")){
                datas.push_back(nullptr);
                cnt++;
            }
            //map
            else if(lines[cnt].startsWith("m")){
                createMap(lines[cnt]);
                cnt++;
            }
        }

        file.close();
    }

    void parseSimpleGlyph(QStringList& lines, int& cnt){
        SimpleGlyphData* data = new SimpleGlyphData();
        QString line = lines[cnt++];
        data->contoursNum = line.split(":", SKIP)[1].toInt();
        GLint* first = new GLint[data->contoursNum];
        GLsizei* count = new GLsizei[data->contoursNum];
        GLuint dataLen = 0;
        QVector<QVector2D*>tmpDatas = QVector<QVector2D*>();
        for(int i = 0; i < data->contoursNum; i++){
            QString flags = lines[cnt++].split(":", SKIP)[1];
            QStringList points = lines[cnt++].split(":", SKIP)[1].split(",", SKIP);
            QVector<QVector2D>pointVec = QVector<QVector2D>();
            for(int i = 0; i < points.size(); i++){
                QStringList li = points[i].split(" ", SKIP);
                pointVec.push_back(QVector2D(li[0].toFloat(), li[1].toFloat()));
            }
            tmpDatas.push_back(getPointsData(flags, pointVec, count[i]));
            if(i == 0){
                first[i] = 0;
            }
            else{
                first[i] = first[i - 1] + count[i - 1];
            }
            dataLen += count[i];
        }
        data->first = first;
        data->count = count;
        data->dataLen = dataLen;
        data->data = new QVector2D[data->dataLen];
        for(int i = 0; i < data->contoursNum; i++){
            memcpy(data->data + first[i], tmpDatas[i], count[i] * sizeof(QVector2D));
            delete[]tmpDatas[i];
        }
        datas.push_back(data);
    }

    void parseCompositeGlyph(QStringList& lines, int& cnt){
        CompositeGlyphData* data = new CompositeGlyphData();
        data->holder = &datas;
        QString line = lines[cnt++];
        data->componentNum = line.split(":", SKIP)[1].toInt();
        if(data->componentNum <= 0){
            qDebug()<<"warning: component num == 0!!!";
            exit(1);
        }
        data->components = new CompositeGlyphComponent*[data->componentNum];
        for(int i = 0; i < data->componentNum; i++){
            CompositeGlyphComponent* component = new CompositeGlyphComponent();
            uint16 glyphIndex = lines[cnt++].split(":", SKIP)[1].toUShort();
            component->glyphIndex = glyphIndex;
            QStringList argList = lines[cnt++].split(":", SKIP);
            if(argList[0].startsWith("xy")){
                QStringList args = argList[1].split(",", SKIP);
                component->offset = new QVector2D(args[0].toFloat(), args[1].toFloat());
            }
            else{
                "warning: arg are pin points";
                exit(2);
            }

            QStringList scale = lines[cnt++].split(":", SKIP)[1].split(",", SKIP);
            data->components[i] = component;
        }
        datas.push_back(data);
    }

    void createMap(QString& line){
        if(mapper == nullptr){
            mapper = new uint16[65536];
        }
        QStringList mappings = line.split(",", SKIP);
        if(mappings.size() != 65536){
            qDebug()<<"error mapping size!!!";
            exit(3);
        }
        for(int i = 0; i < mappings.size(); i++){
            mapper[i] = mappings[i].toUShort();
        }
    }

#undef SKIP
    QVector2D* getPointsData(QString& flags, QVector<QVector2D>&points, int& pointCnt){
        int samplesNum = 8;
        bool* isProcessed = new bool[flags.size()];
        memset(isProcessed, 0, sizeof(bool) * flags.size());
        QVector<QVector2D>tmpPoints = QVector<QVector2D>();
#define NEXT(x) ((i+x) % flags.size())
#define PREV(x) ((i-x + flags.size()) % flags.size())
        BezierCurve2D* curve;
        for(int i = 0; i < flags.size(); i++){
            if(isProcessed[i])break;
            isProcessed[i] = true;
            if(flags[i] == '1'){
                if(flags[NEXT(1)] == '1'){
                    tmpPoints.push_back(points[i]);
                }
                else{
                    if(flags[NEXT(2)] == '1'){
                        curve = new BezierCurve2D(points[i], points[NEXT(1)], points[NEXT(2)]);
                    }
                    else{
                        curve = new BezierCurve2D(points[i], points[NEXT(1)], (points[NEXT(1)] + points[NEXT(2)]) / 2.0);
                    }
                    QVector2D* samples = curve->getSamples(samplesNum);
                    for(int j= 0; j < samplesNum + 2 - 1; j++){
                        tmpPoints.push_back(samples[j]);
                    }
                    isProcessed[NEXT(1)] = true;
                    i++;
                    delete[]samples;
                    delete curve;
                }
            }
            else {
                QVector2D p0, p2;
                if(flags[PREV(1)] == '0'){
                    p0 = (points[PREV(1)] + points[i]) / 2.0;
                }
                else{
                    p0 = points[PREV(1)];
                    isProcessed[PREV(1)] = true;
                }

                if(flags[NEXT(1)] == '0'){
                    p2 = (points[i] + points[NEXT(1)]) / 2.0;
                }
                else{
                    p2 = points[NEXT(1)];
                }
                curve = new BezierCurve2D(p0, points[i], p2);
                QVector2D* samples = curve->getSamples(samplesNum);
                for(int j= 0; j < samplesNum + 2 - 1; j++){
                    tmpPoints.push_back(samples[j]);
                }
                delete[]samples;
                delete curve;
            }
        }
#undef NEXT
#undef PREV
        delete[]isProcessed;
        pointCnt = tmpPoints.size();
        if(pointCnt == 0)return nullptr;
        QVector2D* ret = new QVector2D[pointCnt];
        for(int i = 0; i < pointCnt; i++){
            ret[i] = tmpPoints[i];
        }
        return ret;
    }
};

#endif // GLYPHDATAPARSER_H
