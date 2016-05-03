//#define NUMBER_OF_CASCADES 3

attribute highp highp vec3 posAttr;
attribute highp highp vec2 texAttr;
attribute highp highp vec3 norAttr;
attribute highp highp vec4 bAttr;
attribute highp highp vec4 wAttr;
varying vec3 v_posAttr;
varying vec2 v_texAttr;
varying vec4 v_norAttr;
varying vec4 v_shadowCoord[NUMBER_OF_CASCADES];
varying float v_posViewZ;
uniform highp mat4 u_modelMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_projectionMatrix;
uniform highp mat4 u_depthBiasMVPMatrix[NUMBER_OF_CASCADES];
#ifdef BONE_ANIMATION
uniform highp mat4 u_bones[100];
#endif


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
  mat4 viewmodel = u_viewMatrix * u_modelMatrix;
  mat4 viewmodelbone = viewmodel * boneTransform;
  vec4 posAttr4 = vec4(viewmodelbone * vec4(posAttr, 1));

  v_posAttr = posAttr4.xyz/posAttr4.w;
  v_texAttr = texAttr;
  v_norAttr = (u_modelMatrix * boneTransform * vec4(norAttr, 0));

  for(int i = 0; i < NUMBER_OF_CASCADES; i++) {
     v_shadowCoord[i] = u_depthBiasMVPMatrix[i] * u_modelMatrix * boneTransform * vec4(posAttr, 1.0);
  }
  //v_shadowCoord[0] = u_depthBiasMVPMatrix[0] * u_modelMatrix * boneTransform * vec4(posAttr, 1.0);
  //v_shadowCoord[1] = u_depthBiasMVPMatrix[1] * u_modelMatrix * boneTransform * vec4(posAttr, 1.0);
  //v_shadowCoord[2] = u_depthBiasMVPMatrix[2] * u_modelMatrix * boneTransform * vec4(posAttr, 1.0);
  vec4 thePos = u_projectionMatrix * viewmodelbone * vec4(posAttr, 1.0);
  //v_posViewZ = thePos.z/thePos.w;
  gl_Position = u_projectionMatrix * viewmodelbone * vec4(posAttr, 1.0);
  v_posViewZ = gl_Position.z;
}
