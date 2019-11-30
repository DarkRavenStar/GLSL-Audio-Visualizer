#define GLFW_INCLUDE_ES2 1
#define GLFW_DLL 1
//#define GLFW_EXPOSE_NATIVE_WIN32 1
//#define GLFW_EXPOSE_NATIVE_EGL 1

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream> 
#include "bitmap.h"
#include "FmodClass.h"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

#define TEXTURE_COUNT 3

GLint GprogramID = -1;
GLuint GtextureID[TEXTURE_COUNT];


GLuint Gframebuffer;
GLuint GdepthRenderbuffer;

GLuint GfullscreenTexture1;
GLuint GfullscreenTexture2;

GLFWwindow* window;


static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE)
	{
		PlayOrPause(action);
	}

	if (key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3)
	{
		int musicID = key - 49;
		ChangeMusic(action, musicID);
	}
}

void OnWindowResized(GLFWwindow* windows, int width, int height)
{
	if (height == 0) height = 1;						// Prevent A Divide By Zero By making Height Equal One

	WINDOW_HEIGHT = height;
	WINDOW_WIDTH = width;

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);		// Reset The Current Viewport

	//glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	//glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	//glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
	//glLoadIdentity();									// Reset The Modelview Matrix
}

GLuint LoadShader(GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   	return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
		 char infoLog[4096];
         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         printf ( "Error compiling shader:\n%s\n", infoLog );            
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;
}

GLuint LoadShaderFromFile(GLenum shaderType, std::string path)
{
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile( path.c_str() );

    if( sourceFile )
    {
        shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
        const GLchar* shaderSource = shaderString.c_str();

		return LoadShader(shaderType, shaderSource);
    }
    else
        printf( "Unable to open file %s\n", path.c_str() );

    return shaderID;
}

void loadTexture(const char* path, GLuint textureID)
{
	CBitmap bitmap(path);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

	// bilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());
}

int Init ( void )
{
   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   //load textures
   glGenTextures(TEXTURE_COUNT, GtextureID);
   loadTexture("../Media/DarkGreen.bmp", GtextureID[0]);
   //====

   //==================== set up frame buffer, render buffer, and create an empty texture for blurring purpose
   // create a new FBO
   glGenFramebuffers(1, &Gframebuffer);

   // create a new empty texture for rendering original scene (screenshot)
   glGenTextures(1, &GfullscreenTexture1);
   glBindTexture(GL_TEXTURE_2D, GfullscreenTexture1);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glGenTextures(1, &GfullscreenTexture2);
   glBindTexture(GL_TEXTURE_2D, GfullscreenTexture2);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   // create and bind renderbuffer, and create a 16-bit depth buffer
   glGenRenderbuffers(1, &GdepthRenderbuffer);
   glBindRenderbuffer(GL_RENDERBUFFER, GdepthRenderbuffer);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, WINDOW_WIDTH, WINDOW_HEIGHT);
   //=================================

   fragmentShader = LoadShaderFromFile(GL_VERTEX_SHADER, "../_Shader/vertexShaderAudioVisualizer.vert" );
   vertexShader = LoadShaderFromFile(GL_FRAGMENT_SHADER, "../_Shader/fragmentShaderAudioVisualizer.frag" );

   // Create the program object
   programObject = glCreateProgram ( );
   
   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   glBindAttribLocation ( programObject, 0, "vPosition" );
   glBindAttribLocation ( programObject, 1, "vColor" );
   glBindAttribLocation ( programObject, 2, "vTexCoord" );

   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked ) 
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
		 char infoLog[1024];
         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         printf ( "Error linking program:\n%s\n", infoLog );            
      }

      glDeleteProgram ( programObject );
      return 0;
   }

   // Store the program object
   GprogramID = programObject;

   glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   return 1;
}

void DrawSquare(GLuint texture)
{
	static GLfloat vVertices[] = { -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f };

	static GLfloat vColors[] = { 1.0f,  0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f,  1.0f, 1.0f,
		0.0f,  0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f,  0.0f, 1.0f };

	static GLfloat vTexCoords[] = { 0.0f,  1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f,  0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f };

	glBindTexture(GL_TEXTURE_2D, texture);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vColors);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, vTexCoords);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	glDrawArrays(GL_TRIANGLES, 0, 6);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Draw(void)
{
	// Use the program object
	glUseProgram(GprogramID);

	// set the sampler2D varying variable to the first texture unit(index 0)
	glUniform1i(glGetUniformLocation(GprogramID, "sampler2d"), 0);

	//modify Factor 1 varying variable
	static float factor1 = 0.0f;
	factor1 += 0.01f;
	GLint factor1Loc = glGetUniformLocation(GprogramID, "Factor1");
	if(factor1Loc != -1)
	{
	   glUniform1f(factor1Loc, factor1);
	}

	GLint windowHeightLoc = glGetUniformLocation(GprogramID, "WINDOW_HEIGHT");
	if (windowHeightLoc != -1)
	{
		glUniform1f(windowHeightLoc, WINDOW_HEIGHT);
	}

	GLint windowWidthLoc = glGetUniformLocation(GprogramID, "WINDOW_WIDTH");
	if (windowHeightLoc != -1)
	{
		glUniform1f(windowWidthLoc, WINDOW_WIDTH);
	}

	float leftSpectrum[3];
	leftSpectrum[0] = m_Spectrum_Left[0];
	leftSpectrum[1] = m_Spectrum_Left[1];
	leftSpectrum[2] = m_Spectrum_Left[2];

	GLint leftSpectrumArrayLoc = glGetUniformLocation(GprogramID, "LeftSpectrum");
	if (leftSpectrumArrayLoc != -1)
	{
		glUniform1fv(leftSpectrumArrayLoc, 3, leftSpectrum);
	}

	float rightSpectrum[3];
	rightSpectrum[0] = m_Spectrum_Right[0];
	rightSpectrum[1] = m_Spectrum_Right[1];
	rightSpectrum[2] = m_Spectrum_Right[2];

	GLint rightSpectrumArrayLoc = glGetUniformLocation(GprogramID, "RightSpectrum");
	if (rightSpectrumArrayLoc != -1)
	{
		glUniform1fv(rightSpectrumArrayLoc, 3, rightSpectrum);
	}

	//=================== Render Texture ===================
	// bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, Gframebuffer);
	
	// specify texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GfullscreenTexture1, 0);
	
	// specify depth_renderbufer as depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GdepthRenderbuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUniform1i(glGetUniformLocation(GprogramID, "uBlurDirection"), -1); //set to no blur
	DrawSquare(GtextureID[0]);

	//=================== First pass ===================
	// bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, Gframebuffer);
	
	// specify texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GfullscreenTexture2, 0);
	
	// specify depth_renderbufer as depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GdepthRenderbuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw the texture, apply blurring
	glUniform1i(glGetUniformLocation(GprogramID, "uBlurDirection"), 0);
	DrawSquare(GfullscreenTexture1);

	
	//=================== Second pass ===================
	// bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw the texture, apply blurring
	glUniform1i(glGetUniformLocation(GprogramID, "uBlurDirection"), 1);
	DrawSquare(GfullscreenTexture2);

}

int main(void)
{
  glfwSetErrorCallback(error_callback);

  // Initialize GLFW library
  if (!glfwInit())
    return -1;

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create and open a window
  window = glfwCreateWindow(WINDOW_WIDTH,
                            WINDOW_HEIGHT,
                            "Audio Visualizer By Teena 0118856",
                            NULL,
                            NULL);

  if (!window)
  {
    glfwTerminate();
    printf("glfwCreateWindow Error\n");
    exit(1);
  }
  
  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window, OnWindowResized);
  OnWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

  Init();
  initFMOD();

  //Playlist & Controls
  std::cout << "Play/Pause: Space Bar" << std::endl << std::endl;
  std::cout << "Press 1 for: After Dark - Aimer" << std::endl;
  std::cout << "Press 2 for: From The Earth - Abridged Version" << std::endl;
  std::cout << "Press 3 for: Zaregoto OST - old fashioned fairy tale" << std::endl;
  
  glfwSetKeyCallback(window, KeyInput);
  

  // Repeat
  while (!glfwWindowShouldClose(window))
  {
	Draw();
	UpdateFMOD(window);

	glfwSwapBuffers(window);
	glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
