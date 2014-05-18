varying float intensity;
varying vec3 normal, eyedir;
uniform vec3 camera_pos;

void main()
{
    // transform the normal into eye space and normalize result
    normal = normalize(gl_NormalMatrix * gl_Normal);

    // get direction from eye position to vertex
    eyedir = normalize(gl_Vertex.xyz - camera_pos);
    vec3 vertex_pos = vec3(gl_ModelViewMatrix * gl_Vertex);

    // Get direction to light
    vec3 light_dir = normalize(gl_LightSource[0].position.xyz - vertex_pos);
    intensity = max(dot(light_dir, normal), 0.0);

    gl_Position = ftransform();
}
