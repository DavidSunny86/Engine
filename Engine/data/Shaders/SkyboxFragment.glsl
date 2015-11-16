#version 440

in vec3 texcoords;
uniform samplerCube Texture;
out vec4 color;

void main () {
  color = texture (Texture, texcoords);
}