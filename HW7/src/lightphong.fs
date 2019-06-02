#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float reflectExponent;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    //齐次坐标转普通坐标
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //归一化
    projCoords = projCoords * 0.5 + 0.5;
    //得到最近光照透视的深度值
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    //得到目前片段的深度值
    float currentDepth = projCoords.z;
    //计算偏差
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    //检查目前片段位置是否在阴影中
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
       for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
	float shadow = ShadowCalculation(FragPosLightSpace);

    vec3 ambient = ambientStrength * lightColor;
  	
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (1 - shadow) * diffuseStrength * diff * lightColor;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), reflectExponent);
    vec3 specular = (1 - shadow) * specularStrength * spec * lightColor;  
    
	vec3 lighting = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(lighting, 1.0);
} 