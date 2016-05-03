//#define NUMBER_OF_CASCADES 3

struct DirectionalLight
{
  vec3 direction;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

//struct PointLight
//{
//  vec3 position;
//  vec4 ambient;
//  vec4 diffuse;
//  vec4 specular;
//};

vec4 blinnPhong(
    in vec4 La,
    in vec4 Ld,
    in vec4 Ls,
    in vec4 Ma,
    in vec4 Md,
    in vec4 Ms,
    in vec3 L,
    in vec3 N,
    in vec3 E,
    in float s,
    in float ss)
{
  float NdotL = dot(N, normalize(L));
  vec3 H = normalize(E+L);
  vec4 amb = La*Ma;
  vec4 dif = Ld*Md*max(0, NdotL);

  vec4 spe = vec4(0.0, 0.0, 0.0, 1.0);
  if(NdotL >= 0)
  {
    spe = Ls*Ms*max(0, pow(dot(N, H), s)*ss);
  }

  return vec4((spe+dif+amb).xyz, 1);
}

varying vec3 v_posAttr;
varying vec2 v_texAttr;
varying vec4 v_norAttr;
varying vec4 v_shadowCoord[NUMBER_OF_CASCADES];
varying float v_posViewZ;

#ifdef AMBIENT_TEXTURE
uniform sampler2D u_ambientTexture;

#else
uniform highp vec4 u_ambientMaterial;
#endif
#ifdef DIFFUSE_TEXTURE
uniform sampler2D u_diffuseTexture;
#else
uniform highp vec4 u_diffuseMaterial;
#endif
#ifdef SPECULAR_TEXTURE
uniform sampler2D u_specularTexture;
#else
uniform highp vec4 u_specularMaterial;
#endif
uniform float u_shadowMapCascadeDistance[NUMBER_OF_CASCADES];
uniform sampler2D u_shadowMap[NUMBER_OF_CASCADES];
uniform float u_shininess;
uniform float u_shininessStrength;
uniform vec3 u_eyePosition;
uniform DirectionalLight u_directionalLight;

vec4 ambientMaterialProperty() {
#ifdef AMBIENT_TEXTURE
  return texture2D(u_ambientTexture, v_texAttr);
#else
  return u_ambientMaterial;
#endif
}

vec4 diffuseMaterialProperty() {
#ifdef DIFFUSE_TEXTURE
  return texture2D(u_diffuseTexture, v_texAttr);
#else
  return u_diffuseMaterial;
#endif
}

vec4 specularMaterialProperty() {
#ifdef SPECULAR_TEXTURE
  return texture2D(u_specularTexture, v_texAttr);
#else
  return u_specularMaterial;
#endif
}

void main()
{
   vec4 cascadeColors[5];
   cascadeColors[0] = vec4(1,0.5,0.5,0.5);
   cascadeColors[1] = vec4(0.5,1,0.5,0.5);
   cascadeColors[2] = vec4(0.5,0.5,1,0.5);
   cascadeColors[3] = vec4(1,1,0.5,0.5);
   cascadeColors[4] = vec4(0.5,1,1,0.5);

   vec4 ambient = ambientMaterialProperty();
   vec4 diffuse = diffuseMaterialProperty();
   vec4 specular = specularMaterialProperty();

   vec4 o = blinnPhong(
      u_directionalLight.ambient,
      u_directionalLight.diffuse,
      u_directionalLight.specular,
      ambient,
      diffuse,
      specular,
      u_directionalLight.direction,
      v_norAttr.xyz,
      u_eyePosition - v_posAttr,
      u_shininess,
      u_shininessStrength);
   vec4 shadowCoordDivW;

   float visibility = 1.0;
   float distanceFromLight = 10000;

   vec4 cascadeColor = vec4(1,1,1,0.5);
   for(int i = 0; i < NUMBER_OF_CASCADES; i++) {
      if(v_posViewZ < u_shadowMapCascadeDistance[i]){
         shadowCoordDivW = v_shadowCoord[i]/v_shadowCoord[i].w;
         cascadeColor = cascadeColors[i];
         distanceFromLight = texture2D(u_shadowMap[i], shadowCoordDivW.xy).z;

         float bias = 0.005;
         shadowCoordDivW.z -= bias;
         if(v_shadowCoord[i].w > 0.0 )
         {
            visibility = 1.0;
            if(distanceFromLight < shadowCoordDivW.z)
                  visibility = 0.5;
         }
         break;
      }
   }
   //gl_FragColor =  visibility * o * cascadeColor;
   gl_FragColor =  visibility * o;
 }
