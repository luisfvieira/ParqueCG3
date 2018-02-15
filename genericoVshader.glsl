#version 150

// Alterações devidas à inclusão de textura estão marcadas com: //<<<textura

// SHADER DE VÉRTICES COM DUAS FONTES DE LUZ, AMBAS COM AS
// MESMAS CARACTERÍSTICAS DE LUZ AMBIENTE, DIFUSA E ESPECULAR

in  vec4 vPosition;
in  vec3 vNormal;
in  vec2 vTexCoords; //<<<textura

out vec4 color;
out vec2 st; //<<<textura

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec4 LightPosition1, LightPosition2;
uniform float Shininess;

void main()
{
  st = vTexCoords;  // apenas repassa as coordenadas de textura
                    // para o rasterizador  //<<<textura
  mat4 ModelView = View * Model;
  gl_Position = Projection * ModelView * vPosition;

    // Transform vertex  position into eye coordinates
    vec3 pos = (ModelView * vPosition).xyz;

    vec3 L = normalize( (View*LightPosition1).xyz - pos );
    vec3 V = normalize( -pos );
    vec3 H = normalize( L + V );

    // Transform vertex normal into eye coordinates
    vec3 N = normalize( ModelView*vec4(vNormal, 0.0) ).xyz;

    // Compute terms in the illumination equation
    vec4 ambient = AmbientProduct;

    float Kd = max( dot(L, N), 0.0 );
    vec4  diffuse = Kd*DiffuseProduct;

    // dot(N,H) substitui dot(V,R), evitando calcular R
    float Ks = pow( max(dot(N, H), 0.0), Shininess );
    vec4  specular = Ks * SpecularProduct;

    if( dot(L, N) < 0.0 ) {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
    }

    color = ambient + diffuse + specular;

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    // SEGUNDA FONTE DE LUZ
    L = normalize( (View*LightPosition2).xyz - pos );
    H = normalize( L + V );

    Kd = max( dot(L, N), 0.0 );
    diffuse = Kd*DiffuseProduct;

    Ks = pow( max(dot(N, H), 0.0), Shininess );
    specular = Ks * SpecularProduct;

    if( dot(L, N) < 0.0 ) {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
    }

    // SOMA DAS DUAS FONTES
    color = color + diffuse + specular;

    color.a = 1.0;
  }
