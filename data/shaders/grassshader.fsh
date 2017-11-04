struct DirectionalLight
{
  vec3 direction;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

varying vec4 v_norAttr;
varying vec4 v_shadowCoord[NUMBER_OF_CASCADES];
varying float v_posViewZ;

uniform sampler2D u_texture;
uniform DirectionalLight u_directionalLight;
uniform float u_shadowMapCascadeDistance[NUMBER_OF_CASCADES];
uniform sampler2D u_shadowMap[NUMBER_OF_CASCADES];


varying vec2 v_texAttr;

void main()
{
  vec4 shadowCoordDivW;

  float visibility = 1.0;
  float distanceFromLight = 10000;

   vec4 cascadeColor = vec4(1,1,1,0.5);
   float bias = 0.005;// + 0.001 * tan(acos(dot(normalize(v_norAttr.xyz), normalize(u_directionalLight.direction))));
   for(int i = 0; i < NUMBER_OF_CASCADES; i++) {
     if(v_posViewZ < u_shadowMapCascadeDistance[i]){
       shadowCoordDivW = v_shadowCoord[i]/v_shadowCoord[i].w;
       distanceFromLight = texture2D(u_shadowMap[i], shadowCoordDivW.xy).z;
       if(v_shadowCoord[i].w > 0 )
       {
         visibility = 1.0;
         if(distanceFromLight < shadowCoordDivW.z - bias) {
           visibility = 0.0;
         }
       }
       break;
     }
   }

   vec3 theLightingNormal = v_norAttr.xyz;

   float nDotL =   visibility * max(dot(normalize(theLightingNormal), normalize(u_directionalLight.direction)), 0);

   vec4 diffuse = texture2D(u_texture, v_texAttr);

   vec4 finalColor = vec4(u_directionalLight.ambient.rgb * diffuse.rgb + u_directionalLight.diffuse.rgb * diffuse.rgb * nDotL, diffuse.a);

   gl_FragColor =  finalColor;
}
