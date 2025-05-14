#version 330

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

uniform vec3 lightDir;     // direction of the sun (e.g., vec3(-0.5, -1.0, -0.3))
uniform vec3 viewPos;      // camera position
uniform sampler2D texture0; // albedo/diffuse texture

out vec4 finalColor;

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 lightColor = vec3(1.0);
    vec3 objectColor = texture(texture0, fragTexCoord).rgb;

    // Ambient
    vec3 ambient = 0.1 * objectColor;

    // Diffuse
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor * objectColor;

    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = vec3(0.4) * spec;

    vec3 result = ambient + diffuse + specular;
    finalColor = vec4(result, 1.0);
}
