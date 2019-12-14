#version 330


uniform mat4 osg_ModelViewProjectionMatrix; 

layout(location = 0) in vec3 osg_Vertex;
layout(location = 1) in vec3 osg_Normal;
layout(location = 2) in vec4 osg_Color; 

out VertexData{
    vec4 mColor;

} VertexOut;

void main(void)
{
    VertexOut.mColor = osg_Color;
    //VertexOut.mVertex = vec4(osg_Vertex,1);
    gl_Position = osg_ModelViewProjectionMatrix * vec4(osg_Vertex,1);
}  
