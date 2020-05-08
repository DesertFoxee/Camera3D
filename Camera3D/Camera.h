#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace std;

class Camera
{

	/*
		Position  : setup
		Direction : normal(cameraPos - cameraTarget );
		Right     : normal (cross (up , direction ));
		Up        : normal (cross (direction , right));
	*/

private:
	static glm::vec3 target;
	static float pitch;
	static float yaw;

	static glm::vec3 up;
	static glm::vec3 position;

	static void calculateTarget(){
		float x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		float y = sin(glm::radians(pitch));
		float z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		target = glm::normalize(glm::vec3(x, y, z));
	}
public:

	static void setPosition(glm::vec3 pos){
		Camera::position = pos;
	}
	static void setAngle(float yaw, float pit){
		Camera::pitch = pit;
		Camera::yaw = yaw;
		Camera::calculateTarget();
	}

	// move camera
	static void moveUp(float delta_move){
		Camera::position += Camera::target * delta_move;
	}

	static void moveDown(float delta_move){
		Camera::position -= Camera::target * delta_move;
	}
	static void moveRight(float delta_move){

		glm::vec3 vec_dir;
		vec_dir = glm::cross(Camera::target, glm::vec3(0.0, 1.0, 0.0));

		vec_dir = glm::normalize(vec_dir);

		Camera::position += vec_dir * delta_move;
		
	}
	static void moveLeft(float delta_move){
		glm::vec3 vec_dir;
		vec_dir = glm::cross(Camera::target, glm::vec3(0.0 , 1.0 , 0.0));
		
		vec_dir = glm::normalize(vec_dir);

		Camera::position -= vec_dir * delta_move;
	}

	// rotate camera 
	static void rotate(float delt_yaw, float delt_pitch){
		Camera::pitch += delt_pitch;
		Camera::yaw += delt_yaw;

		Camera::calculateTarget();
	}

	static void rotateHorizontal(float delt_angle){
		Camera::yaw += delt_angle;

		Camera::calculateTarget();
	}
	static void rotateVertical(float delt_angle){
		Camera::pitch += delt_angle;

		Camera::calculateTarget();
	}
	static void setUp(glm::vec3 pos, float pitch = 0.f, float yaw = -90.f, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)){
		Camera::position = pos;
		Camera::pitch = pitch;
		Camera::yaw = yaw;
		Camera::up = up;

		Camera::calculateTarget();
	}

	static glm::mat4 getView(){
		return glm::lookAt(position, position + target, up);
	}
};

#endif // !CAMERA_H
