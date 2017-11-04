
struct DirectionalLight
{
  vec3 direction;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

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
  float NdotL = dot(normalize(N), normalize(L));
  vec3 H = normalize(E+L);
  vec4 amb = Ma;
  vec4 dif = Ld*Md*max(0, NdotL);

  vec4 spe = vec4(0.0, 0.0, 0.0, 1.0);
  if(NdotL >= 0)
  {
    spe = Ls*Ms*max(0, pow(dot(N, H), s)*ss);

    spe.a = 1.0;
  }

 // if(spe.x < 0)
  //  spe.x = 1000;
 // if(spe.y < 0)
  //  spe.y = 1000;
//  if(spe.z < 0)
   // spe.z = 1000;

  oamb = Ma;
  odif = dif;
  ospe = spe;
}



vec3 aeralPerspectiveFog(
    in vec3  rgb,      // original color of the pixel
    in float distance, // camera to point distance
    in vec3  rayDir,   // camera to point vector
    in vec3  sunDir,
    in vec3 rgbFog,
    in float fogDensity)
{
  //const float b = 0.0005;
  const float LOG2 = 1.442695;
  float fogFactor = exp2( -fogDensity *
                     fogDensity *
                     distance *
                     distance *
                     LOG2 );


  float fogAmount = 1.0 - exp( -distance*fogDensity );

  fogFactor = clamp( fogFactor, 0.0, 1.0 );

  if(fogAmount > 1.0) fogAmount = 1.0;
  if(fogAmount < 0.0) fogAmount = 0.0;

//  float sunAmount = max( dot( normalize(rayDir), normalize(sunDir) ), 0.0 );
//  vec3  fogColor  = mix( vec3(0.5,0.6,0.7), // bluish
//                         vec3(1.0,0.9,0.7), // yellowish
//                         pow(sunAmount,8.0) );
  vec3  fogColor  = rgbFog;
  return mix( fogColor, rgb, fogFactor );
  //return vec4(fogFactor, fogFactor, fogFactor,1);
  //return vec4(fogFactor, fogFactor, fogFactor,1);
  //return vec4(fogColor, 1);
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
uniform sampler2D u_aeralPerspectiveFogColorTexture;
uniform float u_fogDensity;
uniform vec3 u_sunPosition;
uniform int u_isTwoSided;

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
  vec4 diffuse = texture2D(u_diffuseTexture, v_texAttr);
#else
  vec4 diffuse = u_diffuseMaterial;
#endif

#ifdef SPECULAR_TEXTURE
  vec4 specular = texture2D(u_specularTexture, v_texAttr);
#else
  vec4 specular = u_specularMaterial;
#endif
  vec4 amb = vec4(0,0,0,1);
  vec4 dif = vec4(0,0,0,1);
  vec4 spe = vec4(0,0,0,1);

  vec4 shadowCoordDivW;

  float visibility = 1.0;
  float distanceFromLight = 10000;

   vec4 cascadeColor = vec4(1,1,1,0.5);
   float bias = 0.005 + 0.001 * tan(acos(dot(normalize(v_norAttr.xyz), normalize(u_directionalLight.direction))));
   for(int i = 0; i < NUMBER_OF_CASCADES; i++) {
     if(v_posViewZ < u_shadowMapCascadeDistance[i]){
       shadowCoordDivW = v_shadowCoord[i]/v_shadowCoord[i].w;
       cascadeColor = cascadeColors[i];
       distanceFromLight = texture2D(u_shadowMap[i], shadowCoordDivW.xy).z;
       if(v_shadowCoord[i].w > 0 )
       {
         visibility = 1.0;
         if(distanceFromLight < shadowCoordDivW.z - bias) {
           visibility = 0.0;
         }
       }
       break;
     }
   }

   vec3 theLightingNormal = v_norAttr.xyz;

   float nDotL =   visibility * max(dot(normalize(theLightingNormal), normalize(u_directionalLight.direction)), 0);
   vec4 finalColor = vec4(u_directionalLight.ambient.rgb * diffuse.rgb + u_directionalLight.diffuse.rgb * diffuse.rgb * nDotL, diffuse.a);

   float dist = distance(v_posAttr, u_eyePosition);

   vec3 dir = normalize(v_posAttr - u_eyePosition);
   vec3 sunDir = normalize(u_sunPosition);
   float elevation = dot(sunDir, vec3(0, -1, 0));
   elevation = (elevation) * 0.5 + 0.5;
   vec4 fogColor = texture2D(u_aeralPerspectiveFogColorTexture, vec2(elevation, 0.1));

   finalColor.rgb = aeralPerspectiveFog(finalColor.rgb, dist, dir, u_directionalLight.direction, fogColor.rgb, u_fogDensity);
   gl_FragColor =  finalColor ;
}
