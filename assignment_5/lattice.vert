#version 120

varying vec4 color;

vec4 computeSpecular(in vec3 lightDir, in vec3 norm, in vec3 vertex_eye) {
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    
    // GLSL reflection function: l-2.0(dot(eye,norm)*norm (expects incoming light dir)
    vec3 r = reflect(-lightDir, norm);
    float VdotR = max(dot(normalize(-vertex_eye), r), 0.0);
    specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * 
    pow(VdotR, gl_FrontMaterial.shininess);
    return specular;
}

void main() {
    // transform vertex into clip coords
    gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex; // or: ftransform()
    // then transform the normal into eye space and normalize the result
    vec3 norm = normalize(gl_NormalMatrix * gl_Normal);
    // The transformed vertex is also direction from eye (at (0,0,0)) to the vertex
    vec3 vertex_eye = vec3(gl_ModelViewMatrix * gl_Vertex); 
    
    vec3 lightDir = normalize(gl_LightSource[0].position.xyz - vertex_eye);
    // compute diffuse factor
    float NdotL = max(dot(norm, lightDir), 0.0);
    // Compute the diffuse color
    vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * NdotL;
    // Compute a local ambient term
    vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    if (NdotL > 0)
        specular = computeSpecular(lightDir, norm, vertex_eye);
    color =  diffuse + specular + ambient;	
    gl_TexCoord[0] = gl_MultiTexCoord0;
}


