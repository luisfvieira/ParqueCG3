// Modelo de cilindro com eixo em z, raio=1.0 e altura=1.0.
// Bases em z=0.0 e z=1.0

#ifndef CILINDRO_H
#define CILINDRO_H

#include <vector>

class Cilindro
{
public:
    Cilindro();
    void criaCilindro(GLuint, const GLchar *, const GLchar *);
    ~Cilindro();
    void desenhar();

private:
    GLuint vao;
    int ndiv; // número de facetas que formam o cilindro
    std::vector<float> pontos, normais;
    void cilindro();
    void tampasCirculares();
};

#endif // CILINDRO_H
