// vertex shader:!
varying vec4 diffuse, ambient;
varying vec3 normal, vertex_eye;
void main()
{
    // transform the normal into eye space and normalize result
    normal = normalize(gl_NormalMatrix * gl_Normal);

    // Get vertex position in eye (camera) coordinates
    vertex_eye = vec3(gl_ModelViewMatrix * gl_Vertex);

    // Compute the diffuse, ambient and globalAmbient terms
    diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;

    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
}
