#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

uniform vec3 LightColor;

uniform vec3 CameraPosition;

uniform bool IsLamp;

uniform vec3 LightPosition;

// The vertex shader will feed this input
in vec2 texCoord;

// World space normal passed from vertex shader
in vec3 normal;

in vec3 fragPos;

// The final color
out vec4 FragmentColor;


void main() {
  // Diffuse lighting
  vec3 lightDir = normalize(LightPosition - fragPos);
  float diffuse = max(dot(normal, normalize(lightDir)), 0.0f);
  vec3 Diffuse = LightColor * diffuse;

  // Ambient lighting
  float ambient = 0.75f;
  vec3 Ambient = LightColor * ambient;

  // Specular lighting
  vec3 viewDir = normalize(CameraPosition-fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 Specular = LightColor * specular * 0.9;

  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * vec4((Diffuse + Ambient + Specular), 1.f);
  if (IsLamp) {
    float d = length(fragPos - LightPosition);
    float attenuation = .1f + 0.07 * d + 0.01 * d * d;
    FragmentColor = FragmentColor * max(dot(-lightDir, vec3(0, -1, 0)), 0.2f) / attenuation;  // spotlight
  }
  else {
    float d = length(fragPos - LightPosition);
    float attenuation = .0001f + 0.0007 * d + 0.000015 * d * d;
    FragmentColor = FragmentColor * max(dot(-lightDir, vec3(0, -1, 0)), 0.2f) / attenuation;  // spotlight
  }
  FragmentColor.a = Transparency;
}
