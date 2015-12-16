#version 440

layout(location = 0) out uint ID;
in flat int vertexID;
void main()
{
	ID = uint(vertexID);
}