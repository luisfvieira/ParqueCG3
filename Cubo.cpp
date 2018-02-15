#define GLEW_STATIC
#include <GL/glew.h>

#include "Angel_util.h"
#include "Cubo.h"

Cubo::Cubo() {  }

Cubo::~Cubo() {  }

void Cubo::desenhar()
{
    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLES, 0, numVertices );
}

void Cubo::criaCubo(GLuint program, const GLchar *position,
                    const GLchar *normal)
{
    // Create and initialize a buffer object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    GLuint vertexbuf;
    glGenBuffers( 1, &vertexbuf);
    glBindBuffer( GL_ARRAY_BUFFER, vertexbuf);
    cubo();
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof(points) + sizeof(normals) + sizeof(tex_coord),
                  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), &points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), &normals );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
                     sizeof(tex_coord), &tex_coord );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, position );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( program, normal );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)) );
}

void Cubo::criaCubo(GLuint program, const GLchar *position,
                    const GLchar *normal, const GLchar *tCoords)
{
    criaCubo(program, position, normal);
    GLuint vTCoords = glGetAttribLocation( program, tCoords );
    glEnableVertexAttribArray( vTCoords );
    glVertexAttribPointer( vTCoords, 2, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)+sizeof(normals)) );
}

void Cubo::cubo()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

void Cubo::quad( int a, int b, int c, int d )
{
    static int index = 0;
    // calcular normal da face
    glm::vec3 u; // = vertices[b].xyz - vertices[a].xyz;
    u.x = vertices[b].x - vertices[a].x;
    u.y = vertices[b].y - vertices[a].y;
    u.z = vertices[b].z - vertices[a].z;
    glm::vec3 v; // = vertices[c].xyz - vertices[b].xyz;
    v.x = vertices[c].x - vertices[b].x;
    v.y = vertices[c].y - vertices[b].y;
    v.z = vertices[c].z - vertices[b].z;

    glm::vec3 normal = glm::normalize( glm::cross(u, v) );
    glm::vec4 normal4 = glm::vec4(normal,0.0);
    // triangulo 1
    normals[index] = normal4; points[index] = vertices[a];
    tex_coord[index] = glm::vec2(0.0f,0.0f);
    index++;
    normals[index] = normal4; points[index] = vertices[b];
    tex_coord[index] = glm::vec2(1.0f,0.0f);
    index++;
    normals[index] = normal4; points[index] = vertices[c];
    tex_coord[index] = glm::vec2(1.0f,1.0f);
    index++;
    // triangulo 2
    normals[index] = normal4; points[index] = vertices[a];
    tex_coord[index] = glm::vec2(0.0f,0.0f);
    index++;
    normals[index] = normal4; points[index] = vertices[c];
    tex_coord[index] = glm::vec2(1.0f,1.0f);
    index++;
    normals[index] = normal4; points[index] = vertices[d];
    tex_coord[index] = glm::vec2(0.0f,1.0f);
    index++;
}
