#version 120

varying vec4  color;

void main()
{
	vec2 scale = vec2(5.0, 5.0);
	vec2 threshold = vec2(0.38, 0.08);
		
	float ss = fract(gl_TexCoord[0].s * scale.s);
	float tt = fract(gl_TexCoord[0].t * scale.t);
		
	if ((ss>threshold.s) && (tt>threshold.t)) discard;
    //discard;
	
	gl_FragColor = color;
}