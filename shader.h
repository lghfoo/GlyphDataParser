#ifndef SHADER_H
#define SHADER_H
#include<qopenglfunctions_4_3_core.h>
#include"filemanager.h"
class Shader{
public:
    static GLuint LoadShaders(QOpenGLFunctions_4_3_Core* func, const char * vertex_file_path,const char * fragment_file_path,
                              int transformFeedbackVaringCount = 0, const GLchar* const* transformFeedbackVaringName = nullptr){
        // Create the shaders
        GLuint VertexShaderID = func->glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = func->glCreateShader(GL_FRAGMENT_SHADER);

        //Create the program
        GLuint ProgramID = func->glCreateProgram();

        // Read the Vertex Shader code from the file
        QString vshCode = FileManager::readWholeFile(vertex_file_path);
        if(vshCode.isEmpty()){
            qDebug()<<"read vsh file error";
            return 0;
        }
        std::string VertexShaderCode = vshCode.toStdString();

        // Read the Fragment Shader code from the file
        QString fshCode = FileManager::readWholeFile(fragment_file_path);
        if(fshCode.isEmpty()){
            qDebug()<<"read fsh file error";
            return 0;
        }
        std::string FragmentShaderCode = fshCode.toStdString();

        GLint Result = GL_FALSE;
        int InfoLogLength;


        // Compile Vertex Shader
        printf("Compiling shader : %s\n", vertex_file_path);
        char const * VertexSourcePointer = VertexShaderCode.c_str();
        func->glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
        if(transformFeedbackVaringCount != 0){
            func->glTransformFeedbackVaryings(ProgramID, transformFeedbackVaringCount, transformFeedbackVaringName, GL_INTERLEAVED_ATTRIBS);
        }
        func->glCompileShader(VertexShaderID);

        // Check Vertex Shader
        func->glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        func->glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            func->glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("%s\n", &VertexShaderErrorMessage[0]);
        }



        // Compile Fragment Shader
        printf("Compiling shader : %s\n", fragment_file_path);
        char const * FragmentSourcePointer = FragmentShaderCode.c_str();
        func->glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        func->glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        func->glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        func->glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
            func->glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("%s\n", &FragmentShaderErrorMessage[0]);
        }



        // Link the program
        printf("Linking program\n");
        func->glAttachShader(ProgramID, VertexShaderID);
        func->glAttachShader(ProgramID, FragmentShaderID);
        func->glLinkProgram(ProgramID);

        // Check the program
        func->glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        func->glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            func->glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
        }


        func->glDetachShader(ProgramID, VertexShaderID);
        func->glDetachShader(ProgramID, FragmentShaderID);

        func->glDeleteShader(VertexShaderID);
        func->glDeleteShader(FragmentShaderID);

        return ProgramID;
    }

    static GLuint LoadShaders(QOpenGLFunctions_4_3_Core* func, const char * vertex_file_path, const char* geometry_file_path, const char * fragment_file_path){
        // Create the shaders
        GLuint VertexShaderID = func->glCreateShader(GL_VERTEX_SHADER);
        GLuint GeometryShaderID = func->glCreateShader(GL_GEOMETRY_SHADER);
        GLuint FragmentShaderID = func->glCreateShader(GL_FRAGMENT_SHADER);

        //Create the program
        GLuint ProgramID = func->glCreateProgram();

        // Read the Vertex Shader code from the file
        QString vshCode = FileManager::readWholeFile(vertex_file_path);
        if(vshCode.isEmpty()){
            qDebug()<<"read vsh file error";
            return 0;
        }
        std::string VertexShaderCode = vshCode.toStdString();

        // Read the Fragment Shader code from the file
        QString fshCode = FileManager::readWholeFile(fragment_file_path);
        if(fshCode.isEmpty()){
            qDebug()<<"read fsh file error";
            return 0;
        }
        std::string FragmentShaderCode = fshCode.toStdString();

        //Read the Geometry Shader code from the file
        QString gshCode = FileManager::readWholeFile(geometry_file_path);
        if(gshCode.isEmpty()){
            qDebug()<<"read gsh file error";
            return 0;
        }
        std::string GeometryShaderCode = gshCode.toStdString();

        GLint Result = GL_FALSE;
        int InfoLogLength;

        // Compile Vertex Shader
        printf("Compiling shader : %s\n", vertex_file_path);
        char const * VertexSourcePointer = VertexShaderCode.c_str();
        func->glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
        func->glCompileShader(VertexShaderID);

        // Check Vertex Shader
        func->glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        func->glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            func->glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("%s\n", &VertexShaderErrorMessage[0]);
        }



        //Compile Geometry Shader
        printf("Compiling shader : %s\n", geometry_file_path);
        char const* GeometrySourcePointer = GeometryShaderCode.c_str();
        func->glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
        func->glCompileShader(GeometryShaderID);

        //Check Geometry Shader
        func->glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
        func->glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> GeometryShaderErrorMessage(InfoLogLength+1);
            func->glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
            printf("%s\n", &GeometryShaderErrorMessage[0]);
        }


        // Compile Fragment Shader
        printf("Compiling shader : %s\n", fragment_file_path);
        char const * FragmentSourcePointer = FragmentShaderCode.c_str();
        func->glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        func->glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        func->glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        func->glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
            func->glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("%s\n", &FragmentShaderErrorMessage[0]);
        }


        // Link the program
        printf("Linking program\n");
        func->glAttachShader(ProgramID, VertexShaderID);
        func->glAttachShader(ProgramID, GeometryShaderID);
        func->glAttachShader(ProgramID, FragmentShaderID);
        func->glLinkProgram(ProgramID);

        // Check the program
        func->glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        func->glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            func->glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
        }


        func->glDetachShader(ProgramID, VertexShaderID);
        func->glDetachShader(ProgramID, GeometryShaderID);
        func->glDetachShader(ProgramID, FragmentShaderID);

        func->glDeleteShader(VertexShaderID);
        func->glDeleteShader(GeometryShaderID);
        func->glDeleteShader(FragmentShaderID);

        return ProgramID;
    }

};

#endif // SHADER_H
