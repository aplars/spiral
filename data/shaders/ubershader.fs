varying vec2 v_texAttr;
#ifdef DIFFUSE_TEXTURE
uniform sampler2D u_diffuseTexture;
#else
uniform highp vec4 u_diffuseMaterial;
#endif


vec4 diffuseMaterial() {
#ifdef DIFFUSE_TEXTURE
   return texture2D(u_diffuseTexture, v_texAttr);
#else
   return u_diffuseMaterial;
#endif
}
      
void main() {
   

   gl_FragColor = diffuseMaterial();
}

