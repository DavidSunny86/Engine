#version 440
struct VertexData_s 
{
	vec4 wsPosition;
	vec4 normal;
};

layout(points) in;
layout(points, max_vertices = 1) out;

in VertexData_s vertexData[];
out VertexData_s outData;

void main()
{
	outData = vertexData[0];
	EmitVertex();
}