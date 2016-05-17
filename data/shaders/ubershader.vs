attribute highp vec3 posAttr;
attribute highp vec2 texAttr;
varying vec2 v_texAttr;
uniform highp mat4 modelMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 projectionMatrix;

void main() {
   v_texAttr = texAttr;
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(posAttr, 1);
}

