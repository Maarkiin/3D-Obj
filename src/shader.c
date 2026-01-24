#include <glad/glad.h>
#ifdef _WIN32
#define uint unsigned int
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *openShader(char *fpath)
{
  FILE *fp; fopen_s(&fp, fpath, "r");
  if(!fp)
  {
    char *errstr = "can not open file";
    printf("err: %s '%s'\n", errstr, fpath);
    return NULL;
  }

  char *r; uint ri = 0;
  char buf[256];
  while (fgets(buf, 256, fp)) {++ri;}
  fseek(fp, 0, SEEK_SET);
  r = calloc(ri ,256 *sizeof(char));
  while (fgets(buf, 256, fp)) {
      strcat_s(r, ri*256, buf);
  }
  //printf("%s\n", r);
  fclose(fp);
  return r;
}

uint createShader(const char *fpath)
{
  int success; char log[512];
  const uint fpathl = strlen(fpath)+1;
  char vt_path[fpathl + 12];
  snprintf(vt_path, fpathl + 12, "%s/vertex.glsl", fpath);
  char fm_path[fpathl + 14];
  snprintf(fm_path, fpathl+14, "%s/fragment.glsl", fpath);
  const char *vt_shader_s = openShader(vt_path);
  const char *fm_shader_s = openShader(fm_path);

  uint vt_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vt_shader, 1, &vt_shader_s, NULL);
  glCompileShader(vt_shader);
  free((void*)vt_shader_s);

  glGetShaderiv(vt_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vt_shader, 256, NULL, log);
    char *errstr = "vertex shader failed to compile";
    printf("err: %s\n%s\n", errstr, log);
  }

  uint fm_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fm_shader, 1, &fm_shader_s, NULL);
  glCompileShader(fm_shader);
  free((void*)fm_shader_s);

  glGetShaderiv(fm_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fm_shader, 256, NULL, log);
    char *errstr = "vertex shader failed to compile";
    printf("err: %s\n%s\n", errstr, log);
  }

  uint shader_program = glCreateProgram();
  glAttachShader(shader_program, vt_shader);
  glAttachShader(shader_program, fm_shader);
  glLinkProgram(shader_program);

  glGetShaderiv(fm_shader, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader_program, 256, NULL, log);
    char *errstr = "shaders failed to link";
    printf("err: %s\n%s\n", errstr, log);
  }

  glDeleteShader(vt_shader);
  glDeleteShader(fm_shader);
  
  return shader_program;
}
