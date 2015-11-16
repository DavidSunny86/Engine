#version 440

in vec3 vertexPosition;
uniform mat4 MVP;
out vec3 texcoords;

void main () {
  vec4 pos = MVP * vec4 (vertexPosition, 1.0);
  gl_Position = pos.xyww;
  texcoords = vertexPosition;
}