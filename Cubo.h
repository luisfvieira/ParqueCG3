// Modelo de um cubo de lado unitário

#ifndef CUBO_H
#define CUBO_H

#include "../include/glm/glm.hpp" //vec2, vec3, mat4, etc

class Cubo
{
public:
    Cubo();
    void criaCubo(GLuint, const GLchar *, const GLchar *);
    void criaCubo(GLuint, const GLchar *, const GLchar *, const GLchar *);
    ~Cubo();
    void desenhar();

private:
    static const int numVertices = 36;
    GLuint vao;
    glm::vec4 points[numVertices];
    glm::vec4 normals[numVertices];
    glm::vec2 tex_coord[numVertices];

    glm::vec4 vertices[8] = {
        glm::vec4( -0.5, -0.5,  0.5, 1.0 ),
        glm::vec4( -0.5,  0.5,  0.5, 1.0 ),
        glm::vec4(  0.5,  0.5,  0.5, 1.0 ),
        glm::vec4(  0.5, -0.5,  0.5, 1.0 ),
        glm::vec4( -0.5, -0.5, -0.5, 1.0 ),
        glm::vec4( -0.5,  0.5, -0.5, 1.0 ),
        glm::vec4(  0.5,  0.5, -0.5, 1.0 ),
        glm::vec4(  0.5, -0.5, -0.5, 1.0 )
    };

    void quad( int, int, int, int );
    void cubo();
};

#endif // CUBO_H
