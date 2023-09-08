#version 410 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;
in vec4 fragmentEyePosition;
in vec3 fragPos;
in vec4 fragPosLightSpace;

out vec4 fColor;

//matrices
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMatrix;
//lighting
uniform mat3 lightDirMatrix;
uniform vec3 lightDir;
uniform vec3 lightColor;
// textures
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;
uniform int modeSelect;
uniform float alpha;
uniform vec3 lightPoint;
//fog
uniform float fogDensity;

//components
vec3 ambient;
float ambientStrength = 0.5f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.7f;
float shininess = 32.0f;


struct PointLight {    
    vec3 position;
    float constant;
    float linear;
    float quadratic;  
};

PointLight pointLight;

vec3 o;


void CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
	vec3 cameraPosEye = vec3(0.0f);
	vec3 lighrDir=normalize(light.position-fragPos);
	vec3 lightDirN = normalize(lightDirMatrix * lightDir);	
	vec3 viewDirN = normalize(cameraPosEye - fragmentEyePosition.xyz);
	vec3 halfVector = normalize(lightDirN + viewDirN);
	vec3 normalEye = normalize(normalMatrix * normal);	
    float distance    = length(light.position - fragPos);
    float att = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));   
	ambient = att * ambientStrength * lightColor; 
	diffuse = att * max(dot(normalEye, lightDirN), 0.0f) * lightColor * 0.96f; 

	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininess);
	specular = att * specularStrength * specCoeff * lightColor * 0.96f;
} 


void computeDirLight()
{
    //compute eye space coordinates
    vec4 fPosEye = view * model * vec4(fPosition, 1.0f);
    vec3 normalEye = normalize(normalMatrix * fNormal);

    //normalize light direction
    vec3 lightDirN = vec3(normalize(view * vec4(lightDir, 0.0f)));

    //compute view direction (in eye coordinates, the viewer is situated at the origin
    vec3 viewDir = normalize(- fPosEye.xyz);

    //compute ambient light
    ambient = 0.2f * lightColor;

    //compute diffuse light
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;

    //compute specular light
    vec3 reflectDir = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 2);
    specular = 0.5f * specCoeff * lightColor;
}


float computeFog()
{
 float fragmentDistance = length(fragmentEyePosition);
 float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));
 return clamp(fogFactor, 0.0f, 1.0f);
}

void computeLights()
{
	ambient *= vec3(texture(diffuseTexture, fTexCoords)) ;
	diffuse *= vec3(texture(diffuseTexture, fTexCoords));
	specular *= vec3(texture(specularTexture, fTexCoords));
}


float computeShadow()
{ 
  // perform perspective divide
  vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  
  // Transform to [0,1] range
  normalizedCoords = normalizedCoords * 0.5 + 0.5;
  
  // Get closest depth value from light's perspective
  float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
  
  // Get depth of current fragment from light's perspective
  float currentDepth = normalizedCoords.z;
  
  // Check whether current frag pos is in shadow
	float bias = max(0.05f * (1.0f - dot(fNormal, lightDir)), 0.005f);
	float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
  
	if (normalizedCoords.z > 1.0f) {
		return 0.0f;
	}

  return shadow;

}
void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fragmentEyePosition.xyz);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
}
void main() 
{
    
    if(modeSelect != 1){
     computeDirLight();
    computeLights();
     vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
     //compute final vertex color
     vec3 color = min((ambient + diffuse) + specular, 1.0f);
     float fogFactor = computeFog();
     vec4 f = fogColor * (1.0f - fogFactor) + vec4(color,0.0f) * fogFactor;
     f.w = alpha;
     fColor = f;
    }


    //pointLight.position=vec3(-20.429f,1.7038f,-22.926f);
    pointLight.position=lightPoint;
	pointLight.constant=0.5f;
	pointLight.linear=0.09f;
	pointLight.quadratic=0.032f;

	 //point light
    if(modeSelect == 1){
	vec3 viewD = vec3(0.0f) - fragmentEyePosition.xyz;	
	CalcPointLight(pointLight,fNormal,fragPos,viewD);
	computeLights();
         vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
       float fogFactor = computeFog();
	vec3 color = min((ambient + diffuse) + specular, 1.0f);
    vec4 f = fogColor * (1.0f - fogFactor) + vec4(color,0.0f) * fogFactor;
     f.w = alpha;
     fColor = f;
    }   


	if(modeSelect == 2){
    computeLightComponents();
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;

	//vec3 color = min((ambient + diffuse) + specular, 1.0f);

	//modulate with shadow
    float shadow = computeShadow();
    vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
	fColor = vec4(color, alpha);
    }

    }
