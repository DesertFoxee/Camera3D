#version 430

layout(location =0 ) in vec3 position;

uniform mat4 mv_mat ;
uniform mat4 proj_mat;

out vec4 frag_color_in ;

void main(void){ 
	gl_Position = proj_mat * mv_mat * vec4(position , 1.0); 

	frag_color_in = vec4(position,1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}