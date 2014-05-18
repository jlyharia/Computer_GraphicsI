//Fragment Shader - Existing normal map
varying vec4 diffuse,ambient, specular;
varying vec3 normal, light_dir, eye_dir;
uniform sampler2D diffuse_tex;
uniform sampler2D normal_map;
void main()
{
    vec3 tex_color = texture2D(diffuse_tex, gl_TexCoord[0].st).rgb;
    // RGB vectors have no negative numbers so rescale from 0..1 to -1.0..1.0
    vec3 bump_normal = vec3(texture2D(normal_map, gl_TexCoord[0].st))*2.0 - 1.0;
    bump_normal = normalize(vec3(bump_normal.xy*2.0f, bump_normal.z));
    // Perform relatively normal per-pixel lighting (except do two-sided for quad)
    vec3 lit_color = 1.5*tex_color.rgb * abs(dot(bump_normal, light_dir));
    vec3 reflect_dir = reflect(light_dir, bump_normal);

    float spec = max(dot(eye_dir, reflect_dir), 0.0);
    spec = pow(spec, gl_FrontMaterial.shininess);
    lit_color = min(lit_color+spec, vec3(1.0));

    gl_FragColor = vec4(tex_color*lit_color, 1.0);
}
