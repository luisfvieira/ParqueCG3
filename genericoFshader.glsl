#version 150

// Altera��es devidas � inclus�o de textura est�o marcadas com: //<<<textura

in  vec4 color;
in  vec2 st; //<<<textura
out vec4 fColor;

uniform sampler2D texMap; //prov� acesso a um objeto de textura //<<<textura
uniform bool isLightSource; //<<<textura  // devido � textura, foi preciso transferir
                                          // do shader de v�rtices para c�
uniform bool withTexture;
uniform bool ligado;

void main()
{
    if (isLightSource) {
    if (ligado){
        fColor = vec4(1.0f,1.0f,0.0f,1.0f);
    }
    else{
        fColor = vec4(1.0f,1.0f,1.0f,1.0f);
    }
  }
    else if (withTexture)
       // combinar cor com textura
	   fColor = texture(texMap, st) * color; //<<<textura
    else
       fColor = color;

}

