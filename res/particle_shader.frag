#version 130

uniform sampler2D texture;

in vec4 gradientColor;

void main()
{
    // Find pixel within the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // Change value to affect metaball 'squishyness'
    if (pixel.r > 0.75f)
    {
        gl_FragColor = gradientColor;
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}