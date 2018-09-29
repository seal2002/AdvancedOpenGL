#pragma once

#include <vector>

#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <glad\glad.h>

#include "Shader.h"

const int PositionsVertexAtribute = 3;
const int ColorsVertexAtribute = 3;
const int TexCoordVertexAtribute = 2;

class Object
{
private:
    unsigned int VAO, VBO;
    float *verticle;
    int numlines;
    int numOfVerticlePerLine;
public:
    void Render(Shader &shader, glm::mat4 model)
    {
        shader.setMat4("model", model);
        // render Cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, numlines);
        glBindVertexArray(0);
    }

    Object():numlines(0), numOfVerticlePerLine(0) {}

    void Load(const char *fileName)
    {
        ifstream fstream(fileName);
        string line;
        vector<float> arr;
        if (fstream)
        {
            while (std::getline(fstream, line))
            {
                float var;
                if (line.compare(0, 1, "/") == 0)
                    continue;
                numlines++;
                numOfVerticlePerLine = 0;
                istringstream ss(line);
                while(ss >> var)
                {
                    arr.push_back(var);
                    numOfVerticlePerLine++;
                }
            }
        }
		else
		{
			std::cout << "Can not load the file " << fileName << std::endl;
		}
		fstream.close();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (arr.size() * sizeof(float)), arr.data(), GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, PositionsVertexAtribute, GL_FLOAT, GL_FALSE, numOfVerticlePerLine * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, ColorsVertexAtribute, GL_FLOAT, GL_FALSE, numOfVerticlePerLine * sizeof(float), (void*)(PositionsVertexAtribute * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, TexCoordVertexAtribute, GL_FLOAT, GL_FALSE, numOfVerticlePerLine * sizeof(float), (void*)((PositionsVertexAtribute + ColorsVertexAtribute) * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~Object() 
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }
};

