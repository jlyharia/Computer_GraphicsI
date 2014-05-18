uniform sampler2D diffuse_tex;

void main()
{
    vec4 tex_color = texture2D(diffuse_tex, gl_TexCoord[0].st);
    gl_FragColor = tex_color; // lazy programmer - no lighting.
}
