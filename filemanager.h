#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include<qstring.h>
#include<qfile.h>
#include<qdebug.h>
class FileManager{
public:
    static QString readWholeFile(const char* filename){
        QFile file(filename);
        //打开文件
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<<"Can't open the file: "<<filename<<endl;
            return "";
        }
        //读取全部内容
        QString ret = file.readAll();
        //关闭文件
        file.close();
        return ret;
    }

};

#endif // FILEMANAGER_H
