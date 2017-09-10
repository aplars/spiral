attribute highp vec3 posAttr;
attribute highp vec2 texAttr;
attribute highp float windAttr;
varying vec2 v_texAttr;

uniform highp mat4 u_modelMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_projectionMatrix;

uniform vec3 u_positions[500];
uniform float u_windStrenght;
uniform vec3 u_windDirection;

uniform float u_time;

void main()
{
  float windPower = 0.5f+sin(u_positions[gl_InstanceID].x/30+u_positions[gl_InstanceID].z/30+u_time*(1.2f+u_windStrenght/20.0f));
  if(windPower < 0.0f)
     windPower = windPower*0.2f;
  else
    windPower = windPower*0.3f;

  windPower *= u_windStrenght;
  mat4 viewmodel = u_viewMatrix * u_modelMatrix;
  v_texAttr = texAttr;
  gl_Position = u_projectionMatrix * viewmodel * vec4(posAttr+u_positions[gl_InstanceID]+(u_windDirection*windAttr*windPower), 1.0);
}
