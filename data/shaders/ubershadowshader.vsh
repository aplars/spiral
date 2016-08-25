//#define NUMBER_OF_CASCADES 3

struct DirectionalLight
{
  vec3 direction;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

attribute highp highp vec3 posAttr;
attribute highp highp vec3 norAttr;

#ifdef BONE_ANIMATION
attribute highp highp vec4 bAttr;
attribute highp highp vec4 wAttr;
#endif

uniform highp mat4 u_modelMatrix;
uniform highp mat4 u_sunViewMatrix;
uniform highp mat4 u_shadowProjectionMatrix;
#ifdef BONE_ANIMATION
uniform highp mat4 u_bones[100];
#endif
uniform DirectionalLight u_directionalLight;

varying vec3 v_posAttr;


float getSlopeScaledBias(vec3 N, vec3 L)
{
  float cosTheta = dot(N, L);
  float bias = 0.5*tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
  bias = clamp(bias, 0,0.5);
  return bias;
}

void main()
{
  //posAttr = posAttr + norAttr*1.4;
  mat4 boneTransform = mat4(0.0);
#ifdef BONE_ANIMATION
  boneTransform  = u_bones[int(bAttr[0])] * wAttr[0];
  boneTransform += u_bones[int(bAttr[1])] * wAttr[1];
  boneTransform += u_bones[int(bAttr[2])] * wAttr[2];
  boneTransform += u_bones[int(bAttr[3])] * wAttr[3];
#else
  boneTransform = mat4(1.0);
#endif
  mat4 viewmodel = u_sunViewMatrix * u_modelMatrix;
  mat4 viewmodelbone = viewmodel * boneTransform;

  vec4 N = (u_modelMatrix * boneTransform * vec4(norAttr, 0));
  vec4 P = (u_modelMatrix * boneTransform * vec4(posAttr, 1));

 // N.xyz = normalize(N.xyz);
  //P.xyz+=N.xyz*15.5;

  float bias = 0.0;//getSlopeScaledBias(N, u_directionalLight.direction);
  vec4 pos = u_shadowProjectionMatrix * u_sunViewMatrix * P;
  if(pos.z < -0.999) pos.z = -0.999;
  gl_Position = pos;
}
