#version 130

out vec4 gradientColor;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    gl_FrontColor = gl_Color;

    // Colour palette taken from Portal
    vec4 lightBlue = vec4(0.0, 0.64, 1.0, 1.0);
    vec4 darkBlue = vec4(0.0, 0.4, 1.0, 1.0);

    float alpha = (gl_Position.y + 1.0) * 2.0;

    // Mix colours with alpha to achieve gradient
    gradientColor = mix(darkBlue, lightBlue, alpha);
}