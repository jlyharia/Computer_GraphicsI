varying float intensity;
varying vec3 normal, eyedir;
uniform samplerCube cube_tex;
void main()
{
    vec3 n = normalize(normal);
    vec3 ed = normalize(eyedir);
    vec3 rd = reflect(ed, n);
    // Look up cube map reflection from 6 textures:
    //vec4 textureCube(samplerCube s, vec3 coord [, float bias])
    vec3 env_color = vec3(textureCube(cube_tex, rd));

    // Mix in the base (underlying) color, if any:
    vec3 base = vec3(intensity*gl_FrontMaterial.diffuse);
    env_color = mix(env_color, base, 0.2);
    gl_FragColor = vec4(env_color, 1.0);
}
