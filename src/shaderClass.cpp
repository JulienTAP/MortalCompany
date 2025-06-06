#include "shaderClass.h"
#include <stdexcept> // Include for std::runtime_error

string get_file_contents(const char* filename)
{
    ifstream in(filename, ios::binary);
    if (in)
    {
        string contents;
        in.seekg(0, ios::end);
        contents.resize(in.tellg());
        in.seekg(0, ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw std::runtime_error("Failed to open file: " + string(filename));
}


Shader::Shader(const char* vertexFile,const char* fragmentFile)
{
    string vertexCode = get_file_contents(vertexFile);
    if(vertexCode.empty())
    {
        throw std::runtime_error("Vertex shader file is empty or could not be read.");
    }
    string fragmentCode = get_file_contents(fragmentFile);
    if(fragmentCode.empty())
    {
        throw std::runtime_error("Fragment shader file is empty or could not be read.");
    }

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexSource,NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentSource,NULL);
    glCompileShader(fragmentShader);

    ID = glCreateProgram();

    glAttachShader(ID,vertexShader);
    glAttachShader(ID,fragmentShader);

    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}