// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 05 - Textured Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shads
	GLuint programID = LoadShaders( "res/shaders/TransformVertexShader.vertexshader", "res/shaders/TextureFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// ------------------------------------------------------------------------------------
	// Load the texture using any two methods
	// GLuint Texture = loadDDS("res/imgs/uvtemplate.DDS");
	// GLuint Texture = loadBMP_custom("res/imgs/uvtemplate.bmp");
	
	// Get a handle for our "myTextureSampler" uniform
	// GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	// static const GLfloat g_vertexuv_indexed_data[] = {
	// 	-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 		// 0	
	// 	-1.0f, -1.0f,  1.0f, 0.0f, 0.66f,		// 1
	// 	 1.0f, -1.0f,  1.0f, 0.33f, 0.66f,		// 2
	// 	 1.0f, -1.0f, -1.0f, 0.66f, 0.66f,		// 3
	// 	-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,		// 4
	// 	-1.0f,  1.0f,  1.0f, 0.0f, 0.33f,		// 5
	// 	 1.0f,  1.0f,  1.0f, 0.33f, 0.33f,		// 6
	// 	 1.0f,  1.0f, -1.0f, 0.66f, 0.33f		// 7
	// };
	// ------------------------------------------------------------------------------------

	// ------------------------------------------------------------------------------------
	// GLuint Texture = loadDDS("res/imgs/uvmap.DDS");
	
	// // Get a handle for our "myTextureSampler" uniform
	// GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	// static const GLfloat g_vertexuv_indexed_data[] = {
	// 	-1.0f, -1.0f, -1.0f, 1.0f, 0.0f,		// 0
	// 	-1.0f, -1.0f,  1.0f, 0.50f, 0.50f,		// 1
	// 	 1.0f, -1.0f,  1.0f, 0.75f, 0.50f,		// 2
	// 	 1.0f, -1.0f, -1.0f, 1.0f, 0.50f,		// 3
	// 	-1.0f,  1.0f, -1.0f, 0.50f, 0.0f,		// 4
	// 	-1.0f,  1.0f,  1.0f, 0.50f, 0.25f,		// 5
	// 	 1.0f,  1.0f,  1.0f, 0.75, 0.25f,		// 6
	// 	 1.0f,  1.0f, -1.0f, 0.75f, 0.0f		// 7
	// };

	// static const unsigned short g_vertex_index_data[] = {
	// 	0, 1, 2,
	// 	3, 5, 2,
	// 	4, 5, 6,
	// 	7, 4, 6,
	// 	5, 0, 1,
	// 	4, 5, 0,
	// 	6, 2, 3,
	// 	7, 6, 3,
	// 	4, 0, 3,
	// 	7, 4, 3,
	// 	5, 1, 2,
	// 	5, 2, 6
	// };

	// GLuint vertex_uv_buffer;
	// glGenBuffers(1, &vertex_uv_buffer);
	// glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexuv_indexed_data), g_vertexuv_indexed_data, GL_STATIC_DRAW);

	// GLuint elementbuffer;
	// glGenBuffers(1, &elementbuffer);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_vertex_index_data), g_vertex_index_data, GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------
	GLuint Texture = loadDDS("res/imgs/uvtemplate.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	static const GLfloat g_vertexuv_indexed_data[] = {
		-1.0f, -1.0f,  1.0f, 0.3333f, 0.6666f,			// 1
		 1.0f, -1.0f,  1.0f, 0.6666f, 0.6666f,			// 2
		-1.0f,  1.0f,  1.0f, 0.3333f, 0.3333f,			// 5

		 1.0f, -1.0f,  1.0f, 0.6666f, 0.6666f,			// 2
		 1.0f,  1.0f,  1.0f, 0.6666f, 0.3333f,			// 6
		-1.0f,  1.0f,  1.0f, 0.3333f, 0.3333f,			// 5

		-1.0f,  1.0f, -1.0f, 0.3333f, 0.0000f,			// 4
		-1.0f,  1.0f,  1.0f, 0.3333f, 0.3333f,			// 5
		 1.0f,  1.0f,  1.0f, 0.6666f, 0.3333f,			// 6

		-1.0f,  1.0f, -1.0f, 0.3333f, 0.0000f,			// 4
		 1.0f,  1.0f,  1.0f, 0.6666f, 0.3333f,			// 6
		 1.0f,  1.0f, -1.0f, 0.6666f, 0.0000f,			// 7

		1.0f,  1.0f,  1.0f,  0.6666f, 0.3333f,			// 6
		1.0f, -1.0f,  1.0f,  0.6666f, 0.6666f,			// 2
		1.0f, -1.0f, -1.0f,  1.0000f, 0.6666f,			// 3

		1.0f, -1.0f, -1.0f,  1.0000f, 0.6666f,			// 3
		1.0f,  1.0f, -1.0f,  1.0000f, 0.3333f,			// 7
		1.0f,  1.0f,  1.0f,  0.6666f, 0.3333f,			// 6
	};

	GLuint vertex_uv_buffer;
	glGenBuffers(1, &vertex_uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexuv_indexed_data), g_vertexuv_indexed_data, GL_STATIC_DRAW);
	
	// ------------------------------------------------------------------------------------

	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(float),
			(void*)0
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer);
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(float),
			(void*)(3 * sizeof(float))
		);

		// glDrawElements(
		// 	GL_TRIANGLES,
		// 	36,
		// 	GL_UNSIGNED_SHORT,
		// 	(void*)0
		// );

		glDrawArrays(GL_TRIANGLES, 0, 18);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertex_uv_buffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

