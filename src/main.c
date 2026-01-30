#include "include.h"

#include <ctype.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>

#define lena(array) sizeof(array)/sizeof(*array)

#include "shader.h"

/*
  https://cs418.cs.illinois.edu/website/text/obj.html
  https://cs418.cs.illinois.edu/website/text/webgl-goemetry.html
  https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_m.h
*/


typedef struct {
    struct {
        float *verts;
        size_t verts_size;

        float *texrs;
        size_t texrs_size;

        float *norms;
        size_t norms_size;

        struct {
            uint v, vt, vn;
        } (*faces)[3];
        size_t faces_size;
    } pre;
    float *verts;
    size_t verts_size;

    uint VBO, VAO, Shader;
    int heap;
} model_t;


int strstar(char *s, char *c)
{
    int r = 1, j = 0;
    uint n = strlen(c);
    for (uint i = 0; i < n; ++i) {r = r && s[j++] == c[i];}
    return r;
}

/*
  example vertices{
   x         y         z         nx      ny      nz      uvx       uvy
  -0.065358, 0.195428, 0.333732, 0.1352, 0.9898, 0.0440, 0.090909, 0.069348,
  -0.065358, 0.195428, 0.333732, 0.1352, 0.9898, 0.0440, 0.090909, 0.069348,
  }
  8 floats per vertex;
*/

void getValuesFromBuff(const char *buf, const size_t buf_size, float *values)
{
    char cbuf[buf_size]; memcpy(cbuf, buf, buf_size*sizeof(char));
    char *tok; char * pcbuf = cbuf; size_t i = 0;
    while ((tok = strtok_r(pcbuf, " ", &pcbuf)), tok)
    {
        if(*tok != 'v') values[i++] = atof(tok);
        //printf("%f, ", atof(tok));
    }
}

void getFacesFromBuff(const char *buf, const size_t buf_size, void *vertex_index)
{
    struct {
        uint v, vt, vn;
    } *vertex_i = vertex_index;
    char cbuf[buf_size]; memcpy(cbuf, buf, buf_size*sizeof(char));
    char *tok; char *pcbuf = cbuf; size_t i = 0;
    // printf("f ");
    while ((tok = strtok_r(pcbuf, " ", &pcbuf)), tok)
    {
        if (*tok != 'f')
        {
            char *p = strtok_r(tok, "/", &tok);
            vertex_i[i].v =  atoi(p)-1;
            p = strtok_r(tok, "/", &tok);
            vertex_i[i].vt = atoi(p)-1;
            p = strtok_r(tok, "/", &tok);
            vertex_i[i].vn = atoi(p)-1;
            // printf("%d/%d/%d ", vertex_i[i].v, vertex_i[i].vt, vertex_i[i].vn);
            i++;
        }
    }
    // printf("\n");
}

model_t *createModel(model_t *m, char* FILEPATH)
{
    model_t *model;
    if (!m)
    {
        model = malloc(sizeof(model_t));
        model->heap = 1;
    } else {
        model = m;
        model->heap = 0;
    }

    model->pre.verts_size = 0;
    model->pre.texrs_size = 0;
    model->pre.norms_size = 0;
    model->pre.faces_size = 0;

    FILE *fp = fopen(FILEPATH, "r");
    const size_t buf_size = 512;
    char buf[buf_size];

    while (fgets(buf, buf_size, fp))
    {
        if(strstar(buf, "v "))
        {
            model->pre.verts_size++;
        }
        if(strstar(buf, "vn "))
        {
            model->pre.texrs_size++;
        }
        if(strstar(buf, "vt "))
        {
            model->pre.norms_size++;
        }
        if(strstar(buf, "f "))
        {
            model->pre.faces_size++;
        }
    }

    /* temp alloc for cube */
    // model->pre.verts_size = 8;
    // model->pre.texrs_size = 6;
    // model->pre.norms_size = 14;
    // model->pre.faces_size = 12;
    /*                     */


    model->pre.verts = malloc(3*sizeof(float)*model->pre.verts_size);
    model->pre.norms = malloc(3*sizeof(float)*model->pre.texrs_size);
    model->pre.texrs = malloc(2*sizeof(float)*model->pre.norms_size);
    model->pre.faces = malloc(3*3*sizeof(uint)*model->pre.faces_size);

    model->pre.verts_size = 0;
    model->pre.texrs_size = 0;
    model->pre.norms_size = 0;
    model->pre.faces_size = 0;

    fseek(fp, 0, SEEK_SET);
    while (fgets(buf, buf_size, fp))
    {
        if(strstar(buf, "v "))
        {
            float values[3] = {0};
            getValuesFromBuff(buf, buf_size, values);
            // printf("v %f %f %f\n", values[0], values[1], values[2]);
            for (int i = 0; i < 3; ++i)
                model->pre.verts[model->pre.verts_size++] = values[i];
        }
        if(strstar(buf, "vn "))
        {
            float values[3] = {0};
            getValuesFromBuff(buf, buf_size, values);
            // printf("vn %f %f %f\n", values[0], values[1], values[2]);
            for (int i = 0; i < 3; ++i)
                model->pre.norms[model->pre.norms_size++] = values[i];
        }
        if(strstar(buf, "vt "))
        {
            float values[2] = {0};
            getValuesFromBuff(buf, buf_size, values);
            // printf("vt %f %f\n", values[0], values[1]);
            for (int i = 0; i < 2; ++i)
                model->pre.texrs[model->pre.texrs_size++] = values[i];
        }

        // todo: make sure this is always after all model->pre packing
        if(strstar(buf, "f "))
        {
            // assumes all faces are triangles.
            getFacesFromBuff(buf, buf_size, (void*)model->pre.faces[model->pre.faces_size++]);
            // printf("f %d/%d/%d ", 1+model->pre.faces[model->pre.faces_size-1][0].v, 1+model->pre.faces[model->pre.faces_size-1][0].vt, 1+model->pre.faces[model->pre.faces_size-1][0].vn);
            // printf("%d/%d/%d ", 1+model->pre.faces[model->pre.faces_size-1][1].v, 1+model->pre.faces[model->pre.faces_size-1][1].vt, 1+model->pre.faces[model->pre.faces_size-1][1].vn);
            // printf("%d/%d/%d\n", 1+model->pre.faces[model->pre.faces_size-1][2].v, 1+model->pre.faces[model->pre.faces_size-1][2].vt, 1+model->pre.faces[model->pre.faces_size-1][2].vn);
        }
    }
    fclose(fp);


    model->verts = malloc(sizeof(float)*8*3*12);
    model->verts_size = 0;

    for (int i = 0; i < (int)model->pre.faces_size; ++i)
    {
        // assume all faces are triangles.
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
                model->verts[model->verts_size++] = model->pre.verts[3*model->pre.faces[i][j].v+k];
            for (int k = 0; k < 3; ++k)
                model->verts[model->verts_size++] = model->pre.norms[3*model->pre.faces[i][j].vn+k];
            for (int k = 0; k < 2; ++k)
                model->verts[model->verts_size++] = model->pre.texrs[2*model->pre.faces[i][j].vt+k];
        }
    }
    printf("num vertices: %lld\n", model->verts_size/8);

    free(model->pre.verts);
    free(model->pre.norms);
    free(model->pre.texrs);
    free(model->pre.faces);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model->verts_size, model->verts, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    model->VBO = VBO;
    model->VAO = VAO;

    return model;
}

void drawModel(model_t *m)
{
    glUseProgram(m->Shader);
    glBindVertexArray(m->VAO);
    glUniform1f(glGetUniformLocation(m->Shader, "time"), clock());
    //glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "model"), 1, GL_FALSE, );
    //glUniform1f(glGetUniformLocation(shaderprogram, "time"), clock());
    //glUniform1f(glGetUniformLocation(shaderprogram, "time"), clock());
    glDrawArrays(GL_TRIANGLES, 0, m->verts_size/8);
}

int deleteModel(model_t *m)
{
    if (m->heap)
    {
        free(m);
        return 0;
    }
    glDeleteVertexArrays(1, &m->VAO);
    glDeleteBuffers(1, &m->VBO);
    return 1;
}

void framebuffer(GLFWwindow *window UNUSED, int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(int argc, char **argv)
{
    printargs(argc, argv, "");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *w = glfwCreateWindow(800, 600, "fent", NULL, NULL);
    if (!w)
    {
        char *errstr = "failed to create window";
        printf("err: %s\n", errstr);
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(w);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        char *errstr = "failed to init GLAD";
        printf("err: %s\n", errstr);
        glfwTerminate();
        return 1;
    }

    glfwSetFramebufferSizeCallback(w, framebuffer);
    glViewport(0, 0, 800, 600);
    // glEnable(GL_FRAMEBUFFER_SRGB);

    //model_t cube;
    //createModel(&cube, "res/obj/cube.obj");
    model_t *mill = createModel(NULL, "res/obj/cube.obj");
    mill->Shader = createShader("res/shaders/default");






    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(w))
    {
        if(glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(w, 1);
        }
        /*       */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawModel(mill);

        /*       */
        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    deleteModel(mill);
    glfwTerminate();
    return 0;
}
