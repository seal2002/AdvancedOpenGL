#ifndef _SHADER_H
#define _SHADER_H

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

using namespace std;

class Shader
{
public:
    GLuint Program;

    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
        string vertexCode;
        string fragmentCode;
        ifstream vShaderFile;
        ifstream fShaderFile;

        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        try
        {
            //Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

        }
        catch (ifstream::failure e)
        {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << vertexPath << " " << fragmentPath << endl;
        }

        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar* fShaderCode = fragmentCode.c_str();

        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];

        //Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        }

        //Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        }

        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);
        // Print linking errors if any
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Use()
    {
        glUseProgram(this->Program);
    }

    void setFloat(const char * variable, float value)
    {
        GLint variableLoc = glGetUniformLocation(this->Program, variable);
        glUniform1f(variableLoc, value);
    }

    void setInt(const char * variable, int value)
    {
        GLint variableLoc = glGetUniformLocation(this->Program, variable);
        glUniform1i(variableLoc, value);
    }

    void setVec3(const char * variable, float x, float y, float z)
    {
        GLint variableLoc = glGetUniformLocation(this->Program, variable);
        glUniform3f(variableLoc, x, y, z);
    }

    void setVec3(const char * variable, const glm::vec3 &value)
    {
        GLint variableLoc = glGetUniformLocation(this->Program, variable);
        glUniform3f(variableLoc, value.x, value.y, value.z);
    }

    void setVec4(const char * variable, float x, float y, float z, float w)
    {
        GLint variableLoc = glGetUniformLocation(this->Program, variable);
        glUniform4f(variableLoc, x, y, z, w);
    }

    void setMat4(const char * variable, const glm::mat4 &value)
    {
        GLint variableLoc = glGetUniformLocation(this->Program, variable);
        glUniformMatrix4fv(variableLoc, 1, GL_FALSE, &value[0][0]);
    }
};

#endif // !SHADER_H