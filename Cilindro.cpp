#include "Angel_util.h"
#include <vector>
#include "Cilindro.h"

Cilindro::Cilindro() {  }

Cilindro::~Cilindro() {  }

void Cilindro::desenhar()
{
    glBindVertexArray( vao );
    glDrawArrays(GL_QUAD_STRIP, 0, 74);
    glDrawArrays(GL_TRIANGLE_FAN, 74, 38);
    glDrawArrays(GL_TRIANGLE_FAN, 112, 38);
}

void Cilindro::criaCilindro(GLuint program, const GLchar *position, const GLchar *normal)
{
    // Create and initialize a buffer object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    GLuint vertexbuf;
    glGenBuffers( 1, &vertexbuf);
    glBindBuffer( GL_ARRAY_BUFFER, vertexbuf);
    ndiv = 36;
    cilindro();
    tampasCirculares();
    glBufferData(
        GL_ARRAY_BUFFER,
        (pontos.size() + normais.size()) * sizeof(float),
        NULL,
        GL_STATIC_DRAW);
    glBufferSubData( GL_ARRAY_BUFFER,
                    0,
                    pontos.size()*sizeof(float),
                    &pontos.front() );
    glBufferSubData( GL_ARRAY_BUFFER,
                    pontos.size()*sizeof(float),
                    normais.size()*sizeof(float),
                    &normais.front() );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, position );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( program, normal );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(pontos.size()*sizeof(float)) );

}

void Cilindro::cilindro()
{
    int passo = 360/ndiv;
    float seno, cosseno;
    for (int j = 0; j <= 360; j += passo){
        cosseno = float(cos((M_PI / 180 * j)));
        seno = float(sin((M_PI / 180 * j)));
        // ponto na base z=1.0
        pontos.push_back(cosseno);
        pontos.push_back(seno);
        pontos.push_back(1.0);
        normais.push_back(cosseno);
        normais.push_back(seno);
        normais.push_back(0.0);

        // ponto na base z=0.0
        pontos.push_back(cosseno);
        pontos.push_back(seno);
        pontos.push_back(0.0);
        normais.push_back(cosseno);
        normais.push_back(seno);
        normais.push_back(0.0);
    }
}

void Cilindro::tampasCirculares()
{
    int passo = 360/ndiv;
    float seno, cosseno;
    // tampa 1
        pontos.push_back(0.0);
        pontos.push_back(0.0);
        pontos.push_back(0.0);
        normais.push_back(0.0);
        normais.push_back(0.0);
        normais.push_back(-1.0);
        for (int k = 0; k <= 360; k += passo){
            cosseno = float(cos((M_PI / 180 * k)));
            seno = float(sin((M_PI / 180 * k)));
            pontos.push_back(cosseno);
            pontos.push_back(seno);
            pontos.push_back(0.0);
            normais.push_back(0.0);
            normais.push_back(0.0);
            normais.push_back(-1.0);
        }

    // tampa 2
        pontos.push_back(0.0);
        pontos.push_back(0.0);
        pontos.push_back(1.0);
        normais.push_back(0.0);
        normais.push_back(0.0);
        normais.push_back(1.0);
        for (int k = 0; k <= 360; k += passo){
            cosseno = float(cos((M_PI / 180 * k)));
            seno = float(sin((M_PI / 180 * k)));
            pontos.push_back(cosseno);
            pontos.push_back(seno);
            pontos.push_back(1.0);
            normais.push_back(0.0);
            normais.push_back(0.0);
            normais.push_back(1.0);
        }
}
