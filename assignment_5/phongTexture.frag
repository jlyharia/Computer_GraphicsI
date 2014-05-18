// fragment shader:
varying vec4 diffuse,ambient;
varying vec3 normal,vertex_eye;
uniform sampler2D diffuse_tex;
void main()
{
    vec3 tex_color = texture2D(diffuse_tex, gl_TexCoord[0].st).rgb;
    vec3 pixel = tex_color*ambient.rgb;
    // the normal is linearly interpolated across the poly so we must renormalize
    vec3 n = normalize(normal);
    vec3 ldir = normalize(gl_LightSource[0].position.xyz - vertex_eye);

    // compute the dot product between normal and light dir
    float NdotL = max(dot(n,ldir),0.0);
    if (NdotL > 0.0)
    {
        pixel += tex_color * diffuse.rgb * NdotL;
        vec3 r = reflect(-ldir, n);
        float VdotR = max(dot(normalize(-vertex_eye), r), 0.0);
        vec4 specular = gl_FrontMaterial.specular * gl_LightSource[0].specular *
                        pow(VdotR, gl_FrontMaterial.shininess);
        pixel += specular;
    }

     gl_FragColor = vec4(pixel, 1.0);
}
