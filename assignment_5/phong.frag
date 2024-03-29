// fragment shader-interpolate normals instead of color (phong illumination)
varying vec4 diffuse, ambient;
varying vec3 normal, vertex_eye;
void main(void)
{
    // The ambient term will always be present
    vec4 color = ambient;

    // a fragment shader can't write a varying variable, hence we
    // need a new variable to store the normalized interpolated normal
    vec3 n = normalize(normal);
    vec3 ldir = normalize(gl_LightSource[0].position.xyz - vertex_eye);
    float NdotL = max(dot(n,ldir),0.0);
    if (NdotL > 0.0)
    {
        color += diffuse * NdotL;
        vec3 r = reflect(-ldir, n);
        float VdotR = max(dot(normalize(-vertex_eye), r), 0.0);
        vec4 specular = gl_FrontMaterial.specular * gl_LightSource[0].specular *
        pow(VdotR, gl_FrontMaterial.shininess);
        color += specular;
    }
    gl_FragColor = color;
}
