uniform sampler2D u_texture;
varying vec2 v_texAttr;

void main()
{
  vec4 texFrag = texture2D(u_texture, v_texAttr);
  texFrag.rgb = vec3(0,0,0);
   gl_FragColor =  texFrag;
}
