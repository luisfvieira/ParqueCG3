// Cone definido com base em z=0.0, ápice em z=1.0
// e raio da base=1.0

#ifndef CONE_H
#define CONE_H

#include "glm/glm.hpp" //vec2, vec3, mat4, etc

class Cone
{
public:
    Cone();
    void criaCone(GLuint, const GLchar *, const GLchar *);
    ~Cone();
    void desenhar();

private:
    GLuint vao;
    int ndiv; // número de facetas que formam o cone
    std::vector<float> pontos, normais;
    void cone();
    void tampaCircular();
};
#endif // CONE_H
