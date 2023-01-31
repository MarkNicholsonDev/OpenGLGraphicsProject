#version 330 core
uniform sampler2D diffuseTex;

in Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 position;
} IN;

out vec4 fragColour;

void main(void) {
    vec4 white = vec4(1, 1, 1, 1);
    fragColour = texture(diffuseTex, IN.texCoord);
    if (IN.position.y > 0) {
        fragColour *= white * (IN.position.y / 150);
    }
}