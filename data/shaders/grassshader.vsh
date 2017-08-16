attribute highp highp vec3 posAttr;
attribute highp highp vec2 texAttr;

varying vec2 v_texAttr;

uniform highp mat4 u_modelMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_projectionMatrix;

//uniform vec2 u_positions[500];
void main()
{
  mat4 viewmodel = u_viewMatrix * u_modelMatrix;
  v_texAttr = texAttr;
  gl_Position = u_projectionMatrix * viewmodel * vec4(posAttr, 1.0);
}
