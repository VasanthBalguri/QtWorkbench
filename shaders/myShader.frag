#version 150

in VertexData{
    vec4 mColor;
} VertexIn;

//out vec4 FragColor;

void main(void)
{

    gl_FragColor = VertexIn.mColor;
}  
