// Cone definido com base em z=0.0, ápice em z=1.0
// e raio da base=1.0

#include "Angel_util.h"
#include <vector>
#include "Cone.h"

Cone::Cone() {  }

Cone::~Cone() {  }

void Cone::desenhar()
{
    glBindVertexArray( vao );
    glDrawArrays(GL_TRIANGLE_FAN, 0, 38);
    glDrawArrays(GL_TRIANGLE_FAN, 38, 38);
}

void Cone::criaCone(GLuint program, const GLchar *position, const GLchar *normal)
{
    // Create and initialize a buffer object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    GLuint vertexbuf;
    glGenBuffers( 1, &vertexbuf);
    glBindBuffer( GL_ARRAY_BUFFER, vertexbuf);
    ndiv = 36;
    cone();
    tampaCircular();
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

void Cone::cone() {
    int passo = 360/ndiv;
    float seno, cosseno;
    // cone
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
            pontos.push_back(0.0);
            normais.push_back(cosseno);
            normais.push_back(seno);
            normais.push_back(1.0);
        }
    // tampa
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
}

void Cone::tampaCircular() {
    int passo = 360/ndiv;
    float seno, cosseno;
    // tampa
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
}

