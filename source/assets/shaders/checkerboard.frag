#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the bottom-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){

    // Assuming that the checkboards will always have an even number of tiles, we can conclude that
    // the tiles with an even sum of indices will have the same color of the bottom left most tile. And the tiles
    // with odd sum of indices will have the other color. Our index will start from 0 to the [NumberOfTiles - 1].
    // The origin of our screen (0,0) is at the bottom-left. that's why our indices start from there
    // Here is a simple illustration for a checkboard with a size of 4:
    //         -------------
    //        |      |      |
    //     1  | c[1] | c[0] |
    //        ---------------
    //     0  | c[0] | c[1] |
    //        |      |      |
    //        ---------------
    //           0      1

    uint tileXindex = uint(gl_FragCoord.x / float(size));
    uint tileYindex = uint(gl_FragCoord.y / float(size));
    bool isSumOfIndicesEven = mod((tileXindex + tileYindex), 2) == 0;
    if (isSumOfIndicesEven)
        frag_color = vec4(colors[0], 1.0);
    else
        frag_color = vec4(colors[1], 1.0);
        
}