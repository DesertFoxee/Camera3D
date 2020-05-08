#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>

#define PI 3.14159265359


#define X_AXIS 0 
#define Y_AXIS 1
#define Z_AXIS 2

#define CUR_HIDDEN 0
#define CUR_NOMAL 1

typedef bool CursorState;

typedef	unsigned int Axis;


using namespace std;


class WindowSetting
{
public:
	unsigned int antialiaLevel =0;
	CursorState cursor = CUR_NOMAL;
	bool displayErrors = false;
};




class Utils
{
private:
	static void showErrorCallBack(int err_code, const char* description){
		cout << "[Error <<" << err_code << "] : " << description << endl;
	}

public:

	/* ======================= <Window> ==================== */

	static GLFWwindow* createWindow(const char* title, unsigned int width, unsigned int height , WindowSetting setting ){
		if (glfwInit() == GLFW_FALSE) return NULL;

		if (setting.displayErrors){
			glfwSetErrorCallback(showErrorCallBack);
		}

		if (setting.antialiaLevel > 0){
			glfwWindowHint(GLFW_SAMPLES, 8);
			glEnable(GL_MULTISAMPLE);
		}
		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

		if (setting.cursor == CUR_HIDDEN){
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		return window;
	}
	static GLFWwindow* createWindow(const char* title, unsigned int width, unsigned int height){
		if (glfwInit() == GLFW_FALSE) return NULL;
		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
		
		return window;
	}

	static void destroyWindow(GLFWwindow* window){
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	static void setKeyboardFunc(GLFWwindow* window, GLFWkeyfun key_func){
		glfwSetKeyCallback(window, key_func);
	}

	static void setResizeFunc(GLFWwindow* window, GLFWwindowsizefun resize_func){
		glfwSetWindowSizeCallback(window, resize_func);
	}

	static void setMouseFunc(GLFWwindow* window, GLFWmousebuttonfun mouse_func){
		glfwSetMouseButtonCallback(window, mouse_func);
	}

	static void setScrollFunc(GLFWwindow* window, GLFWscrollfun scroll_func){
		glfwSetScrollCallback(window, scroll_func);
	}

	static void setCurrsorPosFunc(GLFWwindow* window, GLFWcursorposfun curpos_func){
		glfwSetCursorPosCallback(window, curpos_func);
	}

	static void setActiveWindow(GLFWwindow* window){
		glfwMakeContextCurrent(window);
		if (glewInit()){
			cout << "[Error: glew] : Load glew failed !" << endl;
		}
	}

	// build perspective matrix window
	static glm::mat4 getMatrixProj(GLFWwindow* window, float degree, float near, float far){
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		float aspect_ratio = float(width) / float(height);

		glm::mat4 proj_mat = glm::perspective(cvDegreeToRadius(degree), aspect_ratio, near, far);
		return proj_mat;
	}
	/* ======================= <Vector3D> ==================== */

	static glm::vec3 rotateVec3(glm::vec3 vec , float angle , Axis axis){
		glm::mat4 rotate_mat(1.0);
		glm::vec3 vec_dir;

		if (axis == X_AXIS){
			vec_dir = glm::vec3(1.0, 0.0, 0.0);
		}
		if (axis == Y_AXIS){
			vec_dir = glm::vec3(0.0, 1.0, 0.0);
		}
		if (axis == Z_AXIS){
			vec_dir = glm::vec3(0.0, 0.0, 1.0);
		}
		else{
			return vec;
		}
		rotate_mat = glm::rotate(rotate_mat, angle, vec_dir);

		return glm::vec3(rotate_mat * glm::vec4(vec, 1.0));
	}


	/* ======================= <Shader> ==================== */

	// Read code from file 
	static string readShaderSource(string path){
		string content;
		ifstream file(path, ios::in);

		while (!file.eof()){
			string line;
			getline(file, line);
			content.append(line + "\n");
		}
		file.close();
		return content;
	}

	static GLuint createShaderProgram(const char* vp , const char* fp){
		// get shader source from path file 
		string vertex_str = readShaderSource(vp);
		string frag_str = readShaderSource(fp);

		const char* vertex_source = vertex_str.c_str();
		const char* frag_source = frag_str.c_str();

		// generate id shader with shader type 
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);


		// Load source for shader + show error 	
		glShaderSource(vertex_shader, 1, &vertex_source, NULL);
		glShaderSource(frag_shader, 1, &frag_source, NULL);

		// Compile shader vertex + show error
		glCompileShader(vertex_shader);
		checkOpenGLError();
		GLint vertex_compiled;
		glGetShaderiv(vertex_shader,GL_COMPILE_STATUS, &vertex_compiled);
		if (vertex_compiled != GL_TRUE){
			cout << "Vertex compilation failed !" << endl;
			printShaderLog(vertex_shader);
		}

		// Compile shader fragment + show error
		glCompileShader(frag_shader);
		checkOpenGLError();
		GLint frag_compiled;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &frag_compiled);
		if (frag_compiled != GL_TRUE){
			cout << "Fragment compilation failed !" << endl;
			printShaderLog(frag_shader);
		}

		// Compile link program + show error
		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, frag_shader);
		glLinkProgram(program);
		checkOpenGLError();
		GLint link_compiled;
		glGetProgramiv(program, GL_LINK_STATUS, &link_compiled);
		if (link_compiled != GL_TRUE){
			cout << "Linking failed !" << endl;
			printProgramLog(program);
		}

		return program;
	}

	/* ====================== <Err + Log> =================== */

	static bool checkOpenGLError(){
		bool isErro = false;
		int erro = glGetError();
		while (erro != GL_NO_ERROR){
			std::cout << "[Error]- OpenGL : " << erro << endl;
			isErro = true;
			erro = glGetError();
		}
		return isErro;
	}

	static void  printShaderLog(GLuint shader){
		int length_log, ch_writtn;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length_log);

		if (length_log > 0){
			char *log = new char[length_log];
			glGetShaderInfoLog(shader, length_log, &ch_writtn, log);
			std::cout << "--> Shader Infor log :  " << log << endl;
			delete[] log;
		}
	}

	static void  printProgramLog(GLuint program){
		int length_log, ch_writtn;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length_log);

		if (length_log > 0){
			char *log = new char[length_log];
			glGetProgramInfoLog(program, length_log, &ch_writtn, log);
			std::cout << "--> Program Infor log : " << log << endl;
			delete[] log;
		}
	}
	/* ====================== <Convert > =================== */

	static float cvDegreeToRadius(float degree){
		return (degree * float(PI / 180));
	}
	static float cvRadiusToDegree(float radius){
		return (radius * float(180 / PI));
	}

	static float map(float value, float smin, float smax, float dmin, float dmax){
		if (value <= smin) return dmin;
		if (value >= smax) return dmax;

		float a1 = value - smin;
		float a2 = dmax - dmin;
		float b1 = smax - smin;
		return dmin + (a1 * a2) / b1;
 	}

	/* ====================== <Test > =================== */
	static void showVec3(glm::vec3 vec){
		cout << vec.x << "," << vec.y << "," << vec.z << endl;
	}
};

#endif  // !UTILS_H
