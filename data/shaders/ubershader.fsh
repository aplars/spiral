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
float texture2DCompare(sampler2D depths, vec2 uv, float compare){
    float depth = texture2D(depths, uv).r;
    return step(compare, depth);
}

float texture2DShadowLerp(sampler2D depths, vec2 size, vec2 uv, float compare){
    vec2 texelSize = vec2(1.0)/size;
    vec2 f = fract(uv*size+0.5);
    vec2 centroidUV = floor(uv*size+0.5)/size;

    float lb = texture2DCompare(depths, centroidUV+texelSize*vec2(0.0, 0.0), compare);
    float lt = texture2DCompare(depths, centroidUV+texelSize*vec2(0.0, 1.0), compare);
    float rb = texture2DCompare(depths, centroidUV+texelSize*vec2(1.0, 0.0), compare);
    float rt = texture2DCompare(depths, centroidUV+texelSize*vec2(1.0, 1.0), compare);
    float a = mix(lb, lt, f.y);
    float b = mix(rb, rt, f.y);
    float c = mix(a, b, f.x);
    return c;
}

float PCF(sampler2D depths, vec2 size, vec2 uv){
    float result = 0.0;
    for(int x=-2; x<=2; x++){
        for(int y=-2; y<=2; y++){
            vec2 off = vec2(x,y)/size;
            result += texture2D(depths, uv+off);
        }
    }
    return result/25.0;
}
void blinnPhong(
    in vec4 Ld,
    in vec4 Ls,
    in vec4 Ma,
    in vec4 Md,
    in vec4 Ms,
    in vec3 L,
    in vec3 N,
    in vec3 E,
    in float s,
    in float ss,
    out vec4 oamb,
    out vec4 odif,
    out vec4 ospe)
{
  float NdotL = dot(N, normalize(L));
  vec3 H = normalize(E+L);
  vec4 amb = Ma;
  vec4 dif = Ld*Md*max(0, NdotL);

  vec4 spe = vec4(0.0, 0.0, 0.0, 1.0);
  if(NdotL >= 0)
  {
    spe = Ls*Ms*max(0, pow(dot(N, H), s)*ss);
  }

  oamb = Ma;
  odif = dif;
  ospe = spe;
}

varying vec3 v_posAttr;
varying vec2 v_texAttr;
varying vec4 v_norAttr;
varying vec4 v_shadowCoord[NUMBER_OF_CASCADES];
varying float v_posViewZ;

#ifdef AMBIENT_TEXTURE
uniform sampler2D u_ambientTexture;

#else
uniform highp vec4 u_ambientColor;
#endif
#ifdef DIFFUSE_TEXTURE
uniform sampler2D u_diffuseTexture;
#endif
//#else
uniform highp vec4 u_diffuseMaterial;
//#endif
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


void main()
{
   vec4 cascadeColors[10];
   cascadeColors[0] = vec4(1,0.5,0.5,0.5);
   cascadeColors[1] = vec4(0.5,1,0.5,0.5);
   cascadeColors[2] = vec4(0.5,0.5,1,0.5);
   cascadeColors[3] = vec4(1,1,0.5,0.5);
   cascadeColors[4] = vec4(0.5,1,1,0.5);
   cascadeColors[5] = vec4(0.5,1,0.5,0.5);

#ifdef AMBIENT_TEXTURE
  vec4 ambient = texture2D(u_ambientTexture, v_texAttr);
#else
  vec4 ambient = u_ambientColor;
#endif
#ifdef DIFFUSE_TEXTURE
  vec4 diffuseTex = texture2D(u_diffuseTexture, v_texAttr);
#else
  vec4 diffuseTex = vec4(1,1,1,1);
#endif
  vec4 diffuse = u_diffuseMaterial;

#ifdef SPECULAR_TEXTURE
  vec4 specular = texture2D(u_specularTexture, v_texAttr);
#else
  vec4 specular = u_specularMaterial;
#endif
  vec4 amb, dif, spe;
  blinnPhong(
        u_directionalLight.diffuse,
        u_directionalLight.specular,
        ambient,
        diffuse,
        specular,
        u_directionalLight.direction,
        v_norAttr.xyz,
        u_eyePosition - v_posAttr,
        u_shininess,
        u_shininessStrength,
        amb,
        dif,
        spe);

  vec4 shadowCoordDivW;

   float visibility = 1.0;
   float distanceFromLight = 10000;

   vec4 cascadeColor = vec4(1,1,1,0.5);
   //float bias = 0.0005;
   for(int i = 0; i < NUMBER_OF_CASCADES; i++) {
      if(v_posViewZ < u_shadowMapCascadeDistance[i]){
         shadowCoordDivW = v_shadowCoord[i]/v_shadowCoord[i].w;
         cascadeColor = cascadeColors[i];
         distanceFromLight = texture2D(u_shadowMap[i], shadowCoordDivW.xy).z;
         //distanceFromLight = PCF(u_shadowMap[i], vec2(1024, 1024), shadowCoordDivW.xy);
         if(v_shadowCoord[i].w > 0.0 )
         {

            visibility = 1.0;
            if(distanceFromLight < shadowCoordDivW.z) {
              //visibility = distanceFromLight;
              visibility = 0.0;
            }
         }
         break;
      }
   }
   vec4 finalColor = (diffuseTex*amb + visibility * (diffuseTex*dif + spe));
   //vec4 finalColor = (diffuseTex*amb +  (diffuseTex*dif + spe));
   gl_FragColor =  finalColor;
 }
