struct DirectionalLight
{
  vec3 direction;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};


vec4 blinnPhong(
    in vec4 La,
    in vec4 Ld,
    in vec4 Ls,
    in vec4 Ma,
    in vec4 Md,
    in vec4 Ms,
    in vec3 L,
    in vec3 N,
    in vec3 E,
    in float s,
    in float ss)
{
  float NdotL = dot(N, normalize(L));
  vec3 H = normalize(E+L);
  vec4 amb = La*Ma;
  vec4 dif = Ld*Md*max(0, NdotL);

  vec4 spe = vec4(0.0, 0.0, 0.0, 1.0);
  if(NdotL >= 0)
  {
    spe = Ls*Ms*max(0, pow(dot(N, H), s)*ss);
  }

  return vec4((spe+dif+amb).xyz, 1);
}


varying vec3 v_posAttr;

#ifdef AMBIENT_TEXTURE
uniform sampler2D u_ambientTexture;

#else
uniform highp vec4 u_ambientMaterial;
#endif
#ifdef DIFFUSE_TEXTURE
uniform sampler2D u_diffuseTexture;
#else
uniform highp vec4 u_diffuseMaterial;
#endif
#ifdef SPECULAR_TEXTURE
uniform sampler2D u_specularTexture;
#else
uniform highp vec4 u_specularMaterial;
#endif
uniform vec3 u_eyePosition;
uniform DirectionalLight u_directionalLight;




void main()
{
  vec3 apa;
#ifdef AMBIENT_TEXTURE
  vec4 ambient = texture2D(u_ambientTexture, apa);
#else
  vec4 ambient = u_ambientMaterial;
#endif
#ifdef DIFFUSE_TEXTURE
  vec4 diffuse = texture2D(u_diffuseTexture, apa);
#else
  vec4 diffuse = u_diffuseMaterial;
#endif
#ifdef SPECULAR_TEXTURE
  vec4 specular = texture2D(u_specularTexture, apa);
#else
  vec4 specular = u_specularMaterial;
#endif

   vec4 o = blinnPhong(
      u_directionalLight.ambient,
      u_directionalLight.diffuse,
      u_directionalLight.specular,
      ambient,
      diffuse,
      specular,
      u_directionalLight.direction,
      u_directionalLight.direction,
      u_eyePosition - v_posAttr,
      1,
      1);

   vec4 oo = o*vec4(0.0,0.0,0.0,1.0);
   gl_FragColor =  oo;
 }
