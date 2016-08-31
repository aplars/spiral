//uniform float exposure;
//uniform float decay;
//uniform float density;
//uniform float weight;
//uniform vec2 lightPositionOnScreen;
uniform sampler2D u_texture;
varying vec2 v_texAttr;

//const int NUM_SAMPLES = 100 ;
void main()
{	
	/*
	vec2 deltaTextCoord = vec2( v_texAttr.st - lightPositionOnScreen.xy );
	vec2 textCoo = v_texAttr.st;
	deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;
	float illuminationDecay = 1.0;
		
	for(int i=0; i < NUM_SAMPLES ; i++)
	{
			textCoo -= deltaTextCoord;
			vec4 samp = texture2D(myTexture, textCoo );
			
			samp *= illuminationDecay * weight;
			
			gl_FragColor += samp;
			
			illuminationDecay *= decay;
	}
	
	gl_FragColor *= exposure;
	*/
	//gl_FragColor = vec4(1,1,1,1);//texture2D(u_lightShaftTexture, v_texAttr);
	gl_FragColor = texture2D(u_texture, v_texAttr);

}
