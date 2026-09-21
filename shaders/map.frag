
#version 330

in vec2 fragTexCoord;

uniform sampler2D textureSampler;

out vec4 fragmentColor;

void main()
{
  fragmentColor = texture(textureSampler, fragTexCoord);
}
