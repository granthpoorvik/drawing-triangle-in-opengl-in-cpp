#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

static int CompileShader( unsigned int type,const std::string& source)
/*to compile the created shader attrib: type refers to shader type,source referes to the actual code*/
{
	
	unsigned int id = glCreateShader(type);//takes the id
	const char* src = source.c_str();//points to the first lien of the string reffer line 99 (note its a pointer any changes in source will reflect)
	glShaderSource(id, 1, &src, nullptr);//attrib :id ,how many source code,address of code,nulptr to say each string is null terminated
	glCompileShader(id);
	
	
	
	//Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);//glGetShaderiv=integer vector
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv (id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "failed to compile"<<(type==GL_VERTEX_SHADER ? "Vertex": "fragemet") <<std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;

	}
	return id;  
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragementShader)
/*here we are creating  shader functions so we can actually implement 
the attributs  at line 92 to 111 we have decleared a cpp code for shaders of vertex and fragements they are in the string format*/
{
	unsigned int program = glCreateProgram();//creating id for shaders objects to returns unsigned int 
	unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragementShader);
	glAttachShader(program,vs);//attaching the shaders to programs
	glAttachShader(program,fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);//its not necessary after linking the program
	glDeleteShader(fs);
	return program;

}
int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	//glewInit()should be defined after glfwMakeCOntestcurrent as it is mentioned in the documentation
	if (glewInit() != GLEW_OK)
	{
		std::cout << "error";
	}
	std::cout << glGetString(GL_VERSION);
	float positions[6] = {
	 
	  0.5f, 0,//vertex 1 with x,y
	 -0.5f, 0,//v2
	  0   , 0.6f//v3
		
		
	};
	unsigned int buffer;
	glGenBuffers(1, &buffer);//just createing a buffer and assigning it to a variable "buffer"
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//selecting the buffer what we created using the id=buffer we are specifing the which type of data GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float),positions,GL_STATIC_DRAW );
	/*positions are the list of verticis we have decleared in array
	6 elements so 6* size of float
	*/
	glEnableVertexAttribArray(0);//this is to specify starting postion
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	/*
		index,size of each vertex (x,y),type of data,normalization,ofset btw the vertex in bytes,first vertex
	*/
	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location=0) in vec4 position;"
		"\n"
		"void main()\n"
		"{\n"
		"gl_Position=position;\n "
		"}\n";
	/*vertex shaders are used to shade the vertex points ie put a dot*/
	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location=0) out vec4 color ;"
		"\n"
		"void main()\n"
		"{\n"
		"color=vec4(1.0,0.0,0.0,1.0);\n "

		"}\n";
	/* fragement shader or pixl shader it runs for each pixls 
	to fill the pixls it depends on the area of the fig*/
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);//attributs: mode ,start index,num of index



		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}