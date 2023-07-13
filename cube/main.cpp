#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <iostream>

const GLuint SCRDIM[2] = {1080, 1080};
const GLfloat SCRRAT = (float)SCRDIM[0]/(float)SCRDIM[1];

float deltaTime, prevFrameTime;

const char *cubeVertSource =
  "#version 430 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "uniform mat4 model;\n"
  "uniform mat4 view;\n"
  "uniform mat4 projection;\n"
  "void main() {\n"
  "  gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);\n"
  "}\0";

const char *cubeFragSource =
  "#version 430 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);}\0";

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(SCRDIM[0], SCRDIM[1],
                                        "cube", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glewInit();
  glEnable(GL_DEPTH_TEST);

  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertShader, 1, &cubeVertSource, NULL);
  glCompileShader(vertShader);

  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &cubeFragSource, NULL);
  glCompileShader(fragShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertShader);
  glAttachShader(shaderProgram, fragShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,

      -0.5f, -0.5f,  0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,

      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,

       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,

      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f, -0.5f,

      -0.5f,  0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
  };

  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glm::vec3 cameraPos    = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 cameraUp     = glm::vec3(0.0f, 1.0f, 0.0f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    float currentFrameTime = static_cast<float>(glfwGetTime());
    deltaTime = currentFrameTime - prevFrameTime;
    prevFrameTime = currentFrameTime;

    // render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.f);
    const float radius = 10.0f;
    float camX = radius * sin(currentFrameTime);
    float camZ = radius * cos(currentFrameTime);
    glm::mat4 view = glm::lookAt(glm::vec3(camX, 4.0, camZ), glm::vec3(0.0, 0.0, 0.0), cameraUp);
    glm::mat4 model = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

    // render the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &VBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}
