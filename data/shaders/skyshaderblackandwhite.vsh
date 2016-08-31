in highp vec3 posAttr;
in highp vec2 texAttr;

varying vec2 v_texAttr;
varying vec4 v_fragmentPosition;
varying vec3 v_sunPosition;


uniform highp mat4 u_skyProjectionMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp vec3 u_cameraPosition;
uniform highp vec3 u_sunPosition;


float getTexUCoord(float time) {
  return time;
}

void main(void)
{
  vec3 thePos = vec3(posAttr.x + u_cameraPosition.x, posAttr.y, posAttr.z + u_cameraPosition.z);
  vec3 sunDir = normalize(u_sunPosition);
  float elevation = dot(sunDir, vec3(0, -1, 0));
  elevation = (elevation) * 0.5 + 0.5;

  v_texAttr = texAttr;
  v_texAttr.x = getTexUCoord(elevation);

  v_sunPosition = u_sunPosition;
  v_fragmentPosition = vec4(posAttr, 1);
  gl_Position = u_skyProjectionMatrix * u_viewMatrix * vec4(thePos, 1);
}

