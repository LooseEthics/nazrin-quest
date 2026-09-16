
#version 330 core

in vec2 fragTexCoord;

uniform sampler2D textureSampler;

out vec4 fragmentColor;

uniform vec3 color;

void main(){
  vec4 color = texture(textureSampler, fragTexCoord);
  if (color.a < 0.01)
    discard;

  fragmentColor = color;
}
