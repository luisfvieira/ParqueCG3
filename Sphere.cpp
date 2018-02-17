#include <GL/glew.h>

#include "Sphere.h"
#include "Angel_util.h"
#include <cmath>  // M_PI

#include <vector>
#include <iostream>

Sphere::Sphere()
{
    isInited = false;
    m_vao = 0;
    m_vboVertex = 0;
    m_vboIndex = 0;

    lats = 30;
    longs = 30;
}

Sphere::~Sphere()
{

}

void Sphere::init(GLuint program, const GLchar *position, const GLchar *normal)
{
    int i, j;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    int indicator = 0;
    for(i = 0; i <= lats; i++) {
       double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
       double z0  = sin(lat0);
       double zr0 =  cos(lat0);

       double lat1 = M_PI * (-0.5 + (double) i / lats);
       double z1 = sin(lat1);
       double zr1 = cos(lat1);

       for(j = 0; j <= longs; j++) {
           double lng = 2 * M_PI * (double) (j - 1) / longs;
           double x = cos(lng);
           double y = sin(lng);

           vertices.push_back(x * zr0);
           vertices.push_back(y * zr0);
           vertices.push_back(z0);
           indices.push_back(indicator);
           indicator++;

           vertices.push_back(x * zr1);
           vertices.push_back(y * zr1);
           vertices.push_back(z1);
           indices.push_back(indicator);
           indicator++;
       }
       indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
   }

    // 创建并绑定环境
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, position );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    // Para a esfera, coordenadas de vértices e de normais são as mesmas
    GLuint vNormal = glGetAttribLocation( program, normal );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    glGenBuffers(1, &m_vboIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    numsToDraw = indices.size();

    isInited = true;
}

void Sphere::cleanup()
{
    if (!isInited) {
        return;
    }
    if(m_vboVertex) {
        glDeleteBuffers(1, &m_vboVertex);
    }
    if(m_vboIndex) {
        glDeleteBuffers(1, &m_vboIndex);
    }
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
    }

    isInited = false;
    m_vao = 0;
    m_vboVertex = 0;
    m_vboIndex = 0;
}

void Sphere::draw()
{
    if (!isInited) {
        std::cout << "please call init() before draw()" << std::endl;
    }

    // draw sphere
    glBindVertexArray(m_vao);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
    glDrawElements(GL_QUAD_STRIP, numsToDraw, GL_UNSIGNED_INT, NULL);
}

