#version 440

layout(points) in;
layout(points, max_vertices = 1) out;

in ivec2 vertexID_slice[];
out flat int vertexID;
void main()
{
	gl_Position = gl_in[0].gl_Position;
	vertexID = vertexID_slice[0].x;
	gl_Layer = vertexID_slice[0].y ;
	EmitVertex();
}