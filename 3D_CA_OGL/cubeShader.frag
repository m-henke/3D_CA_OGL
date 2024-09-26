#version 460 core
out vec4 FragColor;

in vec3 color;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform bool useLighting;

void main() {
    if(useLighting) {
        // Simple lighting calculation
        float ambientStrength = 0.1; // Ambient light strength
        vec3 ambient = ambientStrength * lightColor;

        // Calculate the light direction
        vec3 lightDir = normalize(lightPos - fragPos);
    
        // Diffuse shading
        float diff = max(dot(lightDir, vec3(0.0, 0.0, 1.0)), 0.0); // Assuming normals are (0,0,1)
        vec3 diffuse = diff * lightColor;

        // Combine results
        vec3 result = (ambient + diffuse) * color; // Modulate the base color with lighting
        FragColor = vec4(result, 1.0);
    }else{
        FragColor = vec4(color, 1.0);
    }
}
