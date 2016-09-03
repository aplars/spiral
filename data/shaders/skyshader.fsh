varying vec2 v_texAttr;
varying vec4 v_fragmentPosition;
varying vec3 v_sunPosition;

uniform sampler2D u_texture;
uniform sampler2D u_fogColor;

void main(void)
{
  vec3 fragmentPos = v_fragmentPosition.xyz;
  float dist = distance(fragmentPos, v_sunPosition);
  float falloff = dist;

  falloff = 200/dist;
  if(falloff < 0) falloff = 0;
  if(falloff > 1) falloff = 1;

  vec4 tex = texture2D(u_texture, v_texAttr);

  vec4 frag = vec4(1.0, 1.0, 1.0, falloff);
  //vec4 r = falloff * vec4(frag.rgb, 1) + vec4(1.0 - falloff) * vec4(tex);
  vec4 r = vec4(tex);

  gl_FragColor = r;
}

