#version 430 core
layout (location = 0) in vec2 vPos;
uniform mat4 camera;
uniform mat4 projection;
uniform vec2 translation;
void main(){
    gl_Position = projection * camera * vec4(vPos.x + translation.x, vPos.y + translation.y, 0, 1);
}
