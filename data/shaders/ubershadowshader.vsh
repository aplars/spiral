//#define NUMBER_OF_CASCADES 3

attribute highp highp vec3 posAttr;
#ifdef BONE_ANIMATION
attribute highp highp vec4 bAttr;
attribute highp highp vec4 wAttr;
#endif

uniform highp mat4 u_modelMatrix;
uniform highp mat4 u_sunViewMatrix;
uniform highp mat4 u_projectionMatrix;
#ifdef BONE_ANIMATION
uniform highp mat4 u_bones[100];
#endif

varying vec3 v_posAttr;

void main()
{
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

  gl_Position = u_projectionMatrix * viewmodelbone * vec4(posAttr, 1.0);
}
