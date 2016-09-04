varying vec2 v_texAttr;
varying vec4 v_fragmentPosition;
varying vec3 v_sunPosition;

uniform sampler2D u_texture;
uniform sampler2D u_fogColor;

void main(void)
{
  vec3 fragmentPos = v_fragmentPosition.xyz;
  float dist = distance(fragmentPos, v_sunPosition);

  vec4 tex = texture2D(u_texture, v_texAttr);

  if(dist > 300) {
    //gl_FragColor =  vec4(tex.r/20.0, tex.g/20.0, tex.b/20.0, 1.0);// vec4(0,0,0.0,1);
    gl_FragColor =  vec4(0, 0, 0, 1.0);// vec4(0,0,0.0,1);
  }
  else
    gl_FragColor =  vec4(1,1,1,1);

}

