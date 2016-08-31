in highp vec3 posAttr;
in highp vec2 texAttr;

varying vec2 v_texAttr;

uniform highp mat4 u_projectionMatrix;

void main() 
{
	v_texAttr = texAttr;
	gl_Position = u_projectionMatrix * vec4(posAttr, 1);
}
