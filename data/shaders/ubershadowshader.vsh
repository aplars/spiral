attribute highp highp vec3 posAttr;
attribute highp highp vec4 bAttr;
attribute highp highp vec4 wAttr;
uniform highp mat4 u_modelMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_projectionMatrix;
uniform highp mat4 u_bones[100];

void main() {
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
  gl_Position = u_projectionMatrix * viewmodelbone * vec4(posAttr, 1.0);
}
