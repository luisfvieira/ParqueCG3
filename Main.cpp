/*
NOMES: BERNARDO, DAVID, LUIS
INSTRUÇÕES:
LIGAR/DESLIGAR POSTES: Z
CAMERA: NORMAL
*/

//glm: biblioteca matemática para OpenGL
#include <glm/glm.hpp> //vec2, vec3, mat4, etc
#include<glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
// arquivo com recursos auxiliares (do livro texto)
#include "Angel_util.h"
#include "BMPLoader.h" //<<<textura
using namespace std;
#include "Cilindro.h"
#include "Cubo.h"
#include "Cone.h"
#include "Sphere.h"

typedef glm::vec4  color4;
typedef glm::vec4  point4;

GLuint Model, View, Projection, isLightSource, ligado, withTexture;
GLuint program;
GLuint textura_chao, grama, outdoor, texCarrossel, texFlyzone;

int direction = 1;
int giro = 0;
int giro2 = 0;
static float angBase3 = 3.14159f;
bool posteOn = true;
bool pressed1 = false, pressed2 = false;

static glm::vec3 eye;
glm::vec3 center;
glm::vec3 up(0.0,1.0,0.0);

unsigned char teclaCamera;

Cubo cubo;
Cilindro cilindro;
Cone cone;
Sphere esfera;

glm::vec4 light_position1(  5.5f, 15.8f, 0.0f, 1.0f );
glm::vec4 light_position2( -5.5f, 15.8f, 0.0f, 1.0f );
glm::vec4 light_position3( -20.0f, 50.0f, -150.0f, 1.0f );

//----------------------------------------------------------------------------

void poste(glm::mat4 M) {

    float xPoste=0.5, yPoste=10.0, zPoste=0.5;
    float xHaste=4.0, yHaste=0.5, zHaste=0.5;
    float xLuminaria=1.92, yLuminaria=1.92, zLuminaria=1.92;

    M = glm::translate(M, glm::vec3(0.0, 0.0, 0.0));

    glm::mat4 matPoste = M;
    matPoste = glm::translate(matPoste, glm::vec3(0.0, yPoste, 0.0));
    matPoste = glm::scale(matPoste, glm::vec3(xPoste, yPoste, zPoste));

    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matPoste));
    cilindro.desenhar();

    glm::mat4 matHaste = M;
    matHaste = glm::scale(matHaste, glm::vec3(2*xHaste, yHaste, zHaste));
    matHaste = glm::translate(matHaste, glm::vec3(0.0, 3.5*yPoste, 0.0));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matHaste));
    cilindro.desenhar();

    glm::mat4 matLuminaria1 = M;
    matLuminaria1 = glm::scale(matLuminaria1, glm::vec3(xLuminaria, yLuminaria, zLuminaria));
    matLuminaria1 = glm::translate(matLuminaria1, glm::vec3(2.7, yPoste-1.8, 0.0));
    matLuminaria1 = glm::rotate(matLuminaria1, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matLuminaria1));
    cone.desenhar();

    // cout << xLuminaria << "\t" << yLuminaria << "\t"<<zLuminaria <<endl;

    glm::mat4 matLuminaria2 = M;
    matLuminaria2 = glm::scale(matLuminaria2, glm::vec3(xLuminaria, yLuminaria, zLuminaria));
    matLuminaria2 = glm::translate(matLuminaria2, glm::vec3(-2.7, yPoste-1.8, 0.0));
    matLuminaria2 = glm::rotate(matLuminaria2, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matLuminaria2));
    cone.desenhar();
}

void placa(glm::mat4 M, GLuint texPlaca, float x, float y, float z) {

    float xHaste=0.5, yHaste=10.0, zHaste=0.5;
    float xPlaca=15.0, yPlaca=10.0, zPlaca=0.5;

    M = glm::translate(M, glm::vec3(x, y, z));
    M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //  Haste 1
    glm::mat4 matHaste1 = M;
    matHaste1 = glm::translate(matHaste1, glm::vec3(5.0, yHaste/2, -0.5));
    matHaste1 = glm::scale(matHaste1, glm::vec3(xHaste, yHaste, zHaste));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matHaste1));
    cubo.desenhar();

    //  Haste 2
    glm::mat4 matHaste2 = M;
    matHaste2 = glm::translate(matHaste2, glm::vec3(-5.0, yHaste/2, -0.5));
    matHaste2 = glm::scale(matHaste2, glm::vec3(xHaste, yHaste, zHaste));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matHaste2));
    cubo.desenhar();

    //  Placa
    glm::mat4 matPlaca = M;
    glUniform1i(withTexture, true);
    glBindTexture(GL_TEXTURE_2D, texPlaca);
    matPlaca = glm::translate(matPlaca, glm::vec3(0.0, yPlaca, 0.0));
    matPlaca = glm::scale(matPlaca, glm::vec3(xPlaca, yPlaca, zPlaca));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matPlaca));
    cubo.desenhar();
    glUniform1i(withTexture, false);
}

void desenhaFonteDeLuz(glm::vec4 pos, float r)
{   // desenha uma esfera de raio r, representando a fonte de luz
    glm::mat4 luz;
    luz = glm::translate(luz, glm::vec3(pos.x,pos.y,pos.z));
    luz = glm::scale(luz, glm::vec3(r,r,r));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(luz));
    glUniform1i(ligado, posteOn);
    glUniform1i(isLightSource, true);
    esfera.draw();
    glUniform1i(isLightSource, false);
}

void desenhaLua(glm::vec4 pos, float r)
{   // desenha uma esfera de raio r, representando a fonte de luz
    glm::mat4 luz;
    luz = glm::translate(luz, glm::vec3(pos.x,pos.y,pos.z));
    luz = glm::scale(luz, glm::vec3(r,r,r));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(luz));
    glUniform1i(ligado, false);
    glUniform1i(isLightSource, true);
    esfera.draw();
    glUniform1i(isLightSource, false);
}

//----------------------------------------------------------------------------

void cavalo(glm::mat4 M) {
    // Modelo com parte inferior do corpo (barriga) situada
    // em y=0.0, ou seja, no chão
    // Matriz M: transformações para posicionar o cavalo

    //Dimensões das partes do cavalo
    float xCorpo=1.5, yCorpo=1.0, zCorpo=0.5;
    float xPesc=0.3,  yPesc=0.6,  zPesc=0.2;  //pescoço
    float xCab=0.5,   yCab=0.2,   zCab=0.2;   //cabeça
    M = glm::translate(M, glm::vec3(0.0, yCorpo/2.0, 0.0));

   // corpo (orientado segundo o eixo X)
   glm::mat4 matCorpo = M;
   matCorpo = glm::scale(matCorpo, glm::vec3(xCorpo, yCorpo, zCorpo));
   glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matCorpo));
   cubo.desenhar();

   // pescoço
   glm::mat4 matPesc = M;
   matPesc = glm::translate(matPesc, glm::vec3(xCorpo/2-xPesc/2, yCorpo/2+yPesc/2, 0.0));
   matPesc = glm::scale(matPesc, glm::vec3(xPesc, yPesc, zPesc));
   glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matPesc));
   cubo.desenhar();

   // cabeça
   glm::mat4 matCab = M;
   matCab = glm::translate(matCab, glm::vec3(xCorpo/2.0+xCab/2.0-xPesc, yCorpo/2.0+yPesc+yCab/2.0, 0.0));
   matCab = glm::scale(matCab, glm::vec3(xCab, yCab, zCab));
   glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matCab));
   cubo.desenhar();
}

void carrossel(glm::mat4 posicao)
{
    static float angCar=0.0; // rotacao do carrossel
    float altMedia=4.0;      // altura média dos cavalos
    float amplitude=2.0;     // amplitude da variação da altura
    float raioCar=9.0;       // raio da circunferência do carrossel

    angCar += 0.001;
    if( angCar > 2.0*M_PI ) angCar -= 2.0*M_PI;

    // plataforma (placa de dimensões 21x0.8x21, com face inferior em y=0.0)
    float altPlat=0.8;
    glUniform1i(withTexture, true);
    glBindTexture(GL_TEXTURE_2D, textura_chao); //<<<textura
    glm::mat4 matPlat(posicao);
    matPlat = glm::translate(matPlat, glm::vec3(0.0, altPlat/2.0, 0.0));
    matPlat = glm::scale(matPlat, glm::vec3(21.0, 0.8, 21.0));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matPlat));
    cubo.desenhar();
    glUniform1i(withTexture, false);

    // mastro (eixo) do carrossel
    float altMastro = 10.0;
    float diamMastro = 0.3;  //diâmetro do mastro
    glm::mat4 matMastro(posicao);
    matMastro = glm::rotate(matMastro, angCar, glm::vec3(0.0f,1.0f,0.0f));
    matMastro = glm::rotate(matMastro, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
    matMastro = glm::scale(matMastro, glm::vec3(diamMastro,diamMastro,altMastro));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matMastro));
    cilindro.desenhar();

    // cobertura
    glm::mat4 matCob(posicao);
    matCob = glm::translate(matCob, glm::vec3(0.0, 10.0, 0.0));
    matCob = glm::rotate(matCob, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
    matCob = glm::scale(matCob, glm::vec3(10.0, 10.0, 2.0));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matCob));
    cone.desenhar();

    // 8 cavalos com hastes
    float altCav = altMedia + amplitude*sin(2.0*angCar);
    float angDefasagem = 2.0*M_PI/8.0; // defasagem entre os cavalos
    float diamHaste = 0.1;
    glm::mat4 matCavalo, rotCavalo(posicao), matHaste;
    rotCavalo = glm::rotate(rotCavalo, angCar, glm::vec3(0.0f,1.0f,0.0f));
    for (int i = 0; i < 8; i++){
       rotCavalo = glm::rotate(rotCavalo, angDefasagem, glm::vec3(0.0f,1.0f,0.0f));
       matCavalo = glm::translate(rotCavalo, glm::vec3(0.0f,altCav,raioCar));
       cavalo(matCavalo);

       matHaste = glm::translate(rotCavalo, glm::vec3(0.0f,0.0f,raioCar));
       matHaste = glm::rotate(matHaste, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
       matHaste = glm::scale(matHaste, glm::vec3(diamHaste,diamHaste,altMastro));
       glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matHaste));
       cilindro.desenhar();
    }
}

void chao()
{
    // chão (placa de dimensões 80x0.3x80, com face superior em y=0.0)
    glUniform1i(withTexture, true);
    glBindTexture(GL_TEXTURE_2D, grama); //<<<textura
    glm::mat4 matChao;
    matChao = glm::translate(matChao, glm::vec3(0.0, -0.15, 0.0));
    matChao = glm::scale(matChao, glm::vec3(80.0, 0.3, 80.0));
    glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matChao));
    cubo.desenhar();
    glUniform1i(withTexture, false);
}

void flyzone(glm::mat4 posicao)
{
    //  Movimento
  if (direction == 1) angBase3 += 0.001f;
  else  angBase3 -= 0.001f;
  if (glm::radians(angBase3) > 0.085f || glm::radians(angBase3) < 0.025f){
    if (giro < 3){
      direction *= -1;
      giro++;
    }
  }

  if (glm::radians(angBase3) < -0.08f || glm::radians(angBase3) > 0.19f) direction *= -1;

  // plataforma semelhante a do carrossel
  float altPlat=0.8;
  glUniform1i(withTexture, true);
  glBindTexture(GL_TEXTURE_2D, textura_chao);
  glm::mat4 matPlat(posicao);
  matPlat = glm::translate(matPlat, glm::vec3(0.0, altPlat/2.0, 0.0));
  matPlat = glm::scale(matPlat, glm::vec3(21.0, 0.8, 21.0));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matPlat));
  cubo.desenhar();
  glUniform1i(withTexture, false);

  // bases
  float altBases = 15.0;
  float diamBases = 0.5;
  glm::mat4 matBase1(posicao);
  glm::mat4 matBase2(posicao);
  glm::mat4 posicao2;
  posicao2 = glm::translate(posicao2, glm::vec3(0.0,14.0,20.0));
  glm::mat4 matBase3(posicao2);

  //base1
  matBase1 = glm::translate(matBase1, glm::vec3(0.0, 7.5, -5.0));
  matBase1 = glm::rotate(matBase1, glm::radians(-60.0f), glm::vec3(1.0f,0.0f,0.0f));
  matBase1 = glm::scale(matBase1, glm::vec3(diamBases,diamBases,altBases));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matBase1));
  cubo.desenhar();

  //base2
  matBase2 = glm::translate(matBase2, glm::vec3(0.0, 7.5, 5.0));
  matBase2 = glm::rotate(matBase2, glm::radians(60.0f), glm::vec3(1.0f,0.0f,0.0f));
  matBase2 = glm::scale(matBase2, glm::vec3(diamBases,diamBases,altBases));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matBase2));
  cubo.desenhar();

  //base3 (centro)
  glUniform1i(withTexture, true);
  glBindTexture(GL_TEXTURE_2D, outdoor);
  matBase3 = glm::rotate(matBase3, angBase3, glm::vec3(0.0f,0.0f,1.0f));
  matBase3 = glm::translate(matBase3, glm::vec3(0.0, 4.0, 0.0));
  matBase3 = glm::rotate(matBase3, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
  matBase3 = glm::scale(matBase3, glm::vec3(diamBases,diamBases,altBases/1.2));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matBase3));
  cubo.desenhar();
  glUniform1i(withTexture, false);

  //suporte
  glm::mat4 matSup(posicao2);
  //matSup = glm::translate(matSup, glm::vec3(0.0, 1.0, 0.0));
  matSup = glm::rotate(matSup, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
  matSup = glm::scale(matSup, glm::vec3(0.5,3.0,0.5));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matSup));
  cubo.desenhar();


  //bancos
  glm::mat4 matBanco(posicao2);
  glm::mat4 matBancos[2];

  /*base dos bancos
  matBanco = glm::rotate(matBanco, angBase3, glm::vec3(0.0f,0.0f,1.0f));
  matBanco = glm::rotate(matBanco, 2*angBase3 , glm::vec3(0.0f, 1.0f, 0.0f));
  matBanco = glm::translate(matBanco, glm::vec3(0.0, 10.5, 0.0));
  matBanco = glm::rotate(matBanco, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
  matBanco = glm::scale(matBanco, glm::vec3(5.0,5.0,0.5));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matBanco));
  cubo.desenhar();*/

  //bancos 1 e 3
  glUniform1i(withTexture, true);
  glBindTexture(GL_TEXTURE_2D, textura_chao);
  matBancos[0] = glm::translate(matBancos[0], glm::vec3(0.0,14.0,20.0));
  matBancos[0] = glm::rotate(matBancos[0], angBase3, glm::vec3(0.0f,0.0f,1.0f));
  matBancos[0] = glm::rotate(matBancos[0], 2*angBase3 , glm::vec3(0.0f, 1.0f, 0.0f));
  matBancos[0] = glm::translate(matBancos[0], glm::vec3(0.0, 10.5, 0.0));
  matBancos[0] = glm::scale(matBancos[0], glm::vec3(4.5,0.2,0.7));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matBancos[0]));
  cubo.desenhar();
  glUniform1i(withTexture, false);

  //bancos 2 e 4
  glUniform1i(withTexture, true);
  glBindTexture(GL_TEXTURE_2D, textura_chao);
  matBancos[1] = glm::translate(matBancos[1], glm::vec3(0.0,14.0,20.0));
  matBancos[1] = glm::rotate(matBancos[1], angBase3, glm::vec3(0.0f,0.0f,1.0f));
  matBancos[1] = glm::rotate(matBancos[1], 2*angBase3 , glm::vec3(0.0f, 1.0f, 0.0f));
  matBancos[1] = glm::translate(matBancos[1], glm::vec3(0.0, 10.5, 0.0));
  matBancos[1] = glm::scale(matBancos[1], glm::vec3(0.7,0.2,4.5));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matBancos[1]));
  cubo.desenhar();
  glUniform1i(withTexture, false);

  //ferro dos bancos
  glm::mat4 matFerros[4];

  //ferro 1
  glUniform1i(withTexture, true);
  glBindTexture(GL_TEXTURE_2D, textura_chao);
  matFerros[0] = glm::translate(matFerros[0], glm::vec3(0.0,14.0,20.0));
  matFerros[0] = glm::rotate(matFerros[0], angBase3, glm::vec3(0.0f,0.0f,1.0f));
  matFerros[0] = glm::rotate(matFerros[0], 2*angBase3 , glm::vec3(0.0f, 1.0f, 0.0f));
  matFerros[0] = glm::rotate(matFerros[0], glm::radians(45.0f), glm::vec3(0.0f,1.0f,0.0f));
  matFerros[0] = glm::translate(matFerros[0], glm::vec3(0.0, 10.5, 0.0));
  matFerros[0] = glm::translate(matFerros[0], glm::vec3(0.0, 0.0, 1.8));
  matFerros[0] = glm::scale(matFerros[0], glm::vec3(2.6, 0.2, 0.2));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matFerros[0]));
  cubo.desenhar();
  glUniform1i(withTexture, false);

  //ferro 2
  glUniform1i(withTexture, true);
  glBindTexture(GL_TEXTURE_2D, textura_chao);
  matFerros[1] = glm::translate(matFerros[1], glm::vec3(0.0,14.0,20.0));
  matFerros[1] = glm::rotate(matFerros[1], angBase3, glm::vec3(0.0f,0.0f,1.0f));
  matFerros[1] = glm::rotate(matFerros[1], 2*angBase3 , glm::vec3(0.0f, 1.0f, 0.0f));
  matFerros[1] = glm::rotate(matFerros[1], glm::radians(45.0f), glm::vec3(0.0f,1.0f,0.0f));
  matFerros[1] = glm::translate(matFerros[1], glm::vec3(0.0, 10.5, 0.0));
  matFerros[1] = glm::translate(matFerros[1], glm::vec3(0.0, 0.0, -1.8));
  matFerros[1] = glm::scale(matFerros[1], glm::vec3(2.6, 0.2, 0.2));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matFerros[1]));
  cubo.desenhar();
  glUniform1i(withTexture, false);

  //ferro 3
  glUniform1i(withTexture, true);
  glBindTexture(GL_TEXTURE_2D, textura_chao);
  matFerros[2] = glm::translate(matFerros[2], glm::vec3(0.0,14.0,20.0));
  matFerros[2] = glm::rotate(matFerros[2], angBase3, glm::vec3(0.0f,0.0f,1.0f));
  matFerros[2] = glm::rotate(matFerros[2], 2*angBase3 , glm::vec3(0.0f, 1.0f, 0.0f));
  matFerros[2] = glm::rotate(matFerros[2], glm::radians(-45.0f), glm::vec3(0.0f,1.0f,0.0f));
  matFerros[2] = glm::translate(matFerros[2], glm::vec3(0.0, 10.5, 0.0));
  matFerros[2] = glm::translate(matFerros[2], glm::vec3(0.0, 0.0, 1.8));
  matFerros[2] = glm::scale(matFerros[2], glm::vec3(2.6, 0.2, 0.2));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matFerros[2]));
  cubo.desenhar();
  glUniform1i(withTexture, false);

  //ferro 4
  glUniform1i(withTexture, true);
  glBindTexture(GL_TEXTURE_2D, textura_chao);
  matFerros[3] = glm::translate(matFerros[3], glm::vec3(0.0,14.0,20.0));
  matFerros[3] = glm::rotate(matFerros[3], angBase3, glm::vec3(0.0f,0.0f,1.0f));
  matFerros[3] = glm::rotate(matFerros[3], 2*angBase3 , glm::vec3(0.0f, 1.0f, 0.0f));
  matFerros[3] = glm::rotate(matFerros[3], glm::radians(-45.0f), glm::vec3(0.0f,1.0f,0.0f));
  matFerros[3] = glm::translate(matFerros[3], glm::vec3(0.0, 10.5, 0.0));
  matFerros[3] = glm::translate(matFerros[3], glm::vec3(0.0, 0.0, -1.8));
  matFerros[3] = glm::scale(matFerros[3], glm::vec3(2.6, 0.2, 0.2));
  glUniformMatrix4fv(Model,1,GL_FALSE, glm::value_ptr(matFerros[3]));
  cubo.desenhar();
  glUniform1i(withTexture, false);

}

void exibe( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    chao();

    int qtdBrinquedos = 2;
    glm::mat4 brinquedos[qtdBrinquedos];
    glm::vec3 posBrinquedos[qtdBrinquedos] = {glm::vec3(0.0,0.0,-30.0), glm::vec3(0.0,0.0,20.0)};

    for (int i=0; i<qtdBrinquedos; i++) {
        brinquedos[i] = glm::translate(brinquedos[i], posBrinquedos[i]);
    }
    carrossel(brinquedos[0]);
    flyzone(brinquedos[1]);

    desenhaFonteDeLuz(light_position1, 0.5);
    desenhaFonteDeLuz(light_position2, 0.5);
    desenhaLua(light_position3, 3.5);

    glm::mat4 matPoste;
    poste(matPoste);

    // Placa Outdoor
    glm::mat4 matPlaca;
    placa(matPlaca, outdoor, 30.0f, 0.0f, 0.0f);

    //  Placa Carrossel
    glm::mat4 matPlacaCarrossel;
    placa(matPlacaCarrossel, texCarrossel, -30.0f, 0.0, -30.0);

    //  Placa Flyzone
    glm::mat4 matPlacaFlyzone;
    placa(matPlacaFlyzone, texFlyzone, -30.0f, 0.0, 20.0);
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void posicionaCamera(unsigned char tecla) {
    static float angCam = 0.0;  // giro da câmera em torno da origem
    float raioCam = 50.0; // raio da circunferência do movimento da câmera
    switch( tecla ) {
        case 'd': // camera em posição default
            center = glm::vec3(0.0,0.0,0.0);
            eye = glm::vec3(0.0,20.0,50.0);
            break;
        case 'p': // camera em posição pessoa
            center = glm::vec3(0.0,0.0,0.0);
            eye = glm::vec3(0.0,5.0,40.0);
            break;
        case 'j': // camera gira para a esquerda
        case 'l':  // camera gira para a direita
            center = glm::vec3(0.0,0.0,0.0);
            if (tecla == 'j')
                angCam -= 1.5;
            else
                angCam += 1.5;
            eye.x = sin(glm::radians(angCam)) * raioCam;
            eye.z = cos(glm::radians(angCam)) * raioCam;
            break;
        case 'i': // camera sobe
            center = glm::vec3(0.0,0.0,0.0);
            eye.y += 1.2;
            break;
        case 'k':  // camera desce
            eye.y -= 1.2;
            break;
        case 'b':
            eye = glm::vec3(-4.0f*sin(angBase3), 9.0f+cos(angBase3), 20.0f*cos(angBase3));
            center = glm::vec3(1.0f*sin(angBase3), 7.5f+cos(angBase3), 20.0f*cos(angBase3));
            break;
        case '1':   //  camera placa ou carrossel
            eye = glm::vec3(-20.0f, 8.0f, -30.0f);
            if (!pressed2)  center = glm::vec3(-30.0f, 10.0f, -30.0f);
            else    center = glm::vec3(0.0, 10.0f, -30.0f);
            pressed2 = !pressed2;
            break;
        case '2':   //  camera placa ou flyzone
            eye = glm::vec3(-20.0f, 8.0f, 20.0f);
            if (!pressed1)  center = glm::vec3(-30.0f, 10.0f, 20.0f);
            else    center = glm::vec3(0.0, 10.0f, 20.0f);
            pressed1 = !pressed1;
            break;
        case 'z':
            posteOn = !posteOn;
            break;
    }
    glm::mat4 matVis = glm::lookAt(eye, center, up);
    glUniformMatrix4fv(View,1,GL_FALSE, glm::value_ptr(matVis));
    glutPostRedisplay();
}

void teclado( unsigned char tecla, int x, int y )
{
    if (tecla==033)   // Escape Key
	    exit( EXIT_SUCCESS );
    else {
        teclaCamera = tecla;
        posicionaCamera(teclaCamera);
    }
}

//----------------------------------------------------------------------------

void idle( void )
{
   exibe();
}

//----------------------------------------------------------------------------

// OpenGL initialization
void init()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "genericoVshader.glsl",
                          "genericoFshader.glsl" );
    glUseProgram( program );

    cubo.criaCubo(program, "vPosition", "vNormal");
    cilindro.criaCilindro(program, "vPosition", "vNormal");
    cone.criaCone(program, "vPosition", "vNormal");
    esfera.init(program, "vPosition", "vNormal");

//**************************************************
    // Initialize shader lighting parameters
    glm::vec4 light_ambient( 0.2f, 0.2f, 0.2f, 1.0f );
    glm::vec4 light_diffuse( 1.0f, 1.0f, 1.0f, 1.0f );
    glm::vec4 light_specular( 1.0f, 1.0f, 1.0f, 1.0f );

    glm::vec4 material_ambient( 1.0f, 0.0f, 1.0f, 1.0f );
    glm::vec4 material_diffuse( 0.8f, 0.6f, 0.0f, 1.0f );
    glm::vec4 material_specular( 0.9f, 0.7f, 0.0f, 1.0f );
    float  material_shininess = 100.0f;

    color4 ambient_product = light_ambient * material_ambient;
    color4 diffuse_product = light_diffuse * material_diffuse;
    color4 specular_product = light_specular * material_specular;

    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
		  1, glm::value_ptr(ambient_product ));
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
		  1, glm::value_ptr(diffuse_product ));
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
		  1, glm::value_ptr(specular_product ));

    glUniform4fv( glGetUniformLocation(program, "LightPosition1"),
		  1, glm::value_ptr(light_position1 ));
    glUniform4fv( glGetUniformLocation(program, "LightPosition2"),
		  1, glm::value_ptr(light_position2 ));
    glUniform4fv( glGetUniformLocation(program, "LightPosition2"),
      1, glm::value_ptr(light_position3 ));

    glUniform1f( glGetUniformLocation(program, "Shininess"),
		 material_shininess );

    // Retrieve transformation uniform variable locations
    Model = glGetUniformLocation( program, "Model" );
    Projection = glGetUniformLocation( program, "Projection" );
    View = glGetUniformLocation( program, "View" );

    isLightSource = glGetUniformLocation( program, "isLightSource" );
    ligado = glGetUniformLocation( program, "ligado" );
    withTexture = glGetUniformLocation( program, "withTexture" );

//**************************************************

    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glm::mat4 proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 200.0f);
    glUniformMatrix4fv(Projection,1,GL_FALSE, glm::value_ptr(proj));

    posicionaCamera('d'); //posição default

//**************************************************

   BMPClass bmp; //<<<textura
   BMPLoad("branco.bmp",bmp); //<<<textura
   glGenTextures(1, &textura_chao); //<<<textura
   glBindTexture(GL_TEXTURE_2D, textura_chao); //<<<textura
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmp.width,bmp.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp.bytes);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura

   BMPClass bmp2;
   BMPLoad("grass.bmp",bmp2); //<<<textura
   glGenTextures(1, &grama); //<<<textura
   glBindTexture(GL_TEXTURE_2D, grama); //<<<textura
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmp2.width,bmp2.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp2.bytes);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura

   BMPClass bmp3;
   BMPLoad("red.bmp",bmp3); //<<<textura
   glGenTextures(1, &outdoor); //<<<textura
   glBindTexture(GL_TEXTURE_2D, outdoor); //<<<textura
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmp3.width,bmp3.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp3.bytes);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura

   BMPClass bmp4;
   BMPLoad("carrossel.bmp", bmp4);
   glGenTextures(1, &texCarrossel);
   glBindTexture(GL_TEXTURE_2D, texCarrossel); //<<<textura
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmp4.width,bmp4.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp4.bytes);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura

   BMPClass bmp5;
   BMPLoad("flyzone.bmp", bmp5);
   glGenTextures(1, &texFlyzone);
   glBindTexture(GL_TEXTURE_2D, texFlyzone); //<<<textura
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmp5.width,bmp5.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp5.bytes);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //ou GL_LINEAR  //<<<textura


   GLuint tex_loc; //<<<textura
   tex_loc = glGetUniformLocation(program, "texMap"); //<<<textura
   glUniform1i(tex_loc, 0);

} //init

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutCreateWindow( "Parque" );

    glewInit();

    init();

    glutDisplayFunc( exibe );
    glutKeyboardFunc( teclado );
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
