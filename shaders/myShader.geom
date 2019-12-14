#version 330

in VertexData{
    vec4 mColor;
} VertexIn[];

out VertexData{
    vec4 mColor;
     // to pass in case if we use fog effect
} VertexOut; 

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;


void main(void)
{
    float fac = dot(normalize(cross(gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz,gl_in[0].gl_Position.xyz - gl_in[2].gl_Position.xyz)),vec3(0.f,0.f,1.f));
    
    VertexOut.mColor = fac*VertexIn[0].mColor;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    
    VertexOut.mColor = fac*VertexIn[1].mColor;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    
    VertexOut.mColor = fac*VertexIn[2].mColor;
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    
    EndPrimitive();
}
