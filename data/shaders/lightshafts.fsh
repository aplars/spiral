uniform float u_exposure;
uniform float u_decay;
uniform float u_density;
uniform float u_weight;
uniform vec2 u_sunPositionOnScreen;
uniform sampler2D u_texture;
varying vec2 v_texAttr;

const int NUM_SAMPLES = 100 ;
void main()
{	
	vec3 color = vec3(0,0,0);
	vec2 deltaTextCoord = vec2( v_texAttr.st - u_sunPositionOnScreen.xy );
	vec2 textCoo = v_texAttr.st;
	deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * u_density;
	float illuminationDecay = 1.0;
		
	for(int i=0; i < NUM_SAMPLES ; i++)
	{
		  textCoo -= deltaTextCoord;
			vec4 samp = texture2D(u_texture, textCoo);
			
			samp *= illuminationDecay * u_weight;
			
			color += samp.xyz;
			
			illuminationDecay *= u_decay;
	}

	gl_FragColor = vec4(color*u_exposure, 1);

	//gl_FragColor = texture2D(u_texture, v_texAttr);

}
