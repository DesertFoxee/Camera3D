#version 430
in vec4 frag_color_in;
out vec4 color;


uniform mat4 mv_mat;
uniform mat4 proj_mat;
void main(void){ 
	
	color = frag_color_in ;
}