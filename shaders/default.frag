#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;



// Gets the Texture Unit from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

vec4 pointLight(){

	// vec3 lightVec = lightPos - crntPos;
	// float dist = length(lightVec);
	// float a = 0.05f;
	// float b = 0.07f;
	// float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// // ambient lighting
	// float ambient = 0.20f;

	// // diffuse lighting
	// vec3 normal = normalize(Normal);
	// vec3 lightDirection = normalize(lightVec);
	// float diffuse = max(dot(normal, lightDirection), 0.0f);

	// // specular lighting
	// float specularLight = 0.50f;
	// vec3 viewDirection = normalize(camPos - crntPos);
	// vec3 reflectionDirection = reflect(-lightDirection, normal);
	// float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	// float specular = specAmount * specularLight;

	// // outputs final color
	// return texture(diffuse0, texCoord) * lightColor * (ambient + diffuse * inten) + texture(specular0, texCoord).r * specular * lightColor * inten;

	vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);

    // Standard attenuation coefficients (adjust as needed)
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;
    float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

    // ambient lighting
    float ambientStrength = 0.20f;
    vec3 ambient = ambientStrength * lightColor.rgb * texture(diffuse0, texCoord).rgb;

    // diffuse lighting
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor.rgb * texture(diffuse0, texCoord).rgb;

    // specular lighting
    float specularStrength = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 16);
    float specMap = texture(specular0, texCoord).r;
    vec3 specular = specularStrength * spec * specMap * lightColor.rgb;

    // Combine results with attenuation
    vec3 result = (ambient + attenuation * (diffuse + specular));
    return vec4(result, 1.0);
}

vec4 direcLight(){
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(-5.0f, 0.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// outputs final color
	return texture(diffuse0, texCoord) * lightColor * (ambient + diffuse) + texture(specular0, texCoord).r * specular * lightColor;
}

vec4 spotLight(){ // not working properly (need to gigure out angle of light)

	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	// outputs final color
	return texture(diffuse0, texCoord) * lightColor * (ambient + diffuse * inten) + texture(specular0, texCoord).r * specular * lightColor * inten;
}



void main()
{	
	// outputs final color
	FragColor = pointLight(); // Change to direcLight() or spotLight() to use different lighting models
}