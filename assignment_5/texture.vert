
// vertex shader:
void main()
{

	gl_TexCoord[0] = gl_MultiTexCoord0; // gl_TexCoord[] is varying.
	gl_Position = ftransform();
}
