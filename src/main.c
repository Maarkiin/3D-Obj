#include "include.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

typedef struct {
    /*vertices*/
    size_t  size_vs;
    float  *vs;
    /*indices*/
    size_t  size_indices;
    int    *indices;
    /*uv coords*/
    size_t  size_uvs;
    float  *uvs;
    size_t  size_uvsi;
    uint   *uvsi;
    /*normals*/
    size_t  size_ns;
    float  *ns;
    size_t  size_nsi;
    uint   *nsi;

    /*translation/rotation/scale*/
    vec3f_t tr;
    vec3f_t ro;
    vec3f_t sc;
} object_t;

void createObject(object_t *o, char* FILEPATH)
{
    o->size_vs      = 0;
    o->size_indices = 0;
    o->size_ns      = 0;
    o->size_nsi     = 0;
    o->size_uvs     = 0;
    o->size_uvsi    = 0;
    //
    const uint SIZE = 4096;
    FILE *fp;
    fp = fopen(FILEPATH, "r");
    char buf[SIZE];
    while(fgets(buf, SIZE, fp))
    {
        if (strstar(buf, "v "))
        {
            char *p = strtok(buf, " ");
            while (p!=NULL)
            {
                if (!strstar(p, "v"))
                {
                    o->size_vs++; 
                }
                p = strtok(NULL, " ");
            }
        }
        if (strstar(buf, "vn "))
        {
            char *p = strtok(buf, " ");
            while (p!=NULL)
            {
                if (!strstar(p, "vn"))
                {
                    o->size_ns++;
                }
                p = strtok(NULL, " ");
            }
        }
        if (strstar(buf, "vt "))
        {
            char *p = strtok(buf, " ");
            while (p!=NULL)
            {
                if (!strstar(p, "vt"))
                {
                    o->size_uvs++;
                }
                p = strtok(NULL, " ");
            }
        }
        if (strstar(buf, "f "))
        {
            char *p = strtok(buf, " ");
            while (p!=NULL)
            {
                if (!strstar(p, "f"))
                {
                    char *tok = strtok_r(p, "/", &p);
                    int i = 0;
                    while (tok!=NULL)
                    {
                        switch (i)
                        {
                        case 0:
                            o->size_indices++;
                        break;
                        case 1:
                            o->size_uvsi++;
                        break;
                        case 2:
                            o->size_nsi++;
                        break;  
                        }
                        tok = strtok_r(p, "/", &p);
                        i++;
                    }
                }
                p = strtok(NULL, " ");
            }
        }
    }

    o->vs           = malloc(o->size_vs*sizeof(float));
    o->size_vs      = 0;

    o->indices      = malloc(o->size_indices*sizeof(float));
    o->size_indices = 0;
    
    o->ns           = malloc(o->size_ns*sizeof(uint));
    o->size_ns      = 0;

    o->nsi          = malloc(o->size_nsi*sizeof(float));
    o->size_nsi     = 0;

    o->uvs          = malloc(o->size_uvs*sizeof(uint));
    o->size_uvs     = 0;

    o->uvsi         = malloc(o->size_uvsi*sizeof(uint));
    o->size_uvsi    = 0;

    fseek(fp, 0, SEEK_SET);
    while(fgets(buf, SIZE, fp))
    {
        if (strstar(buf, "v "))
        {
            char *p = strtok(buf, " ");
            while (p!=NULL)
            {
                if (!strstar(p, "v"))
                {
                    //printf("%s ", p);
                    o->vs[o->size_vs++] = (float)atof(p);
                    
                }
                p = strtok(NULL, " ");
            }
        }
        if (strstar(buf, "vn "))
        {
            char *p = strtok(buf, " ");
            while (p!=NULL)
            {
                if (!strstar(p, "vn"))
                {
                    //printf("%s ", p);
                    o->ns[o->size_ns++] = (float)atof(p);
                    
                }
                p = strtok(NULL, " ");
            }
        }
        if (strstar(buf, "f "))
        {
            char *p = strtok(buf, " ");
            while (p!=NULL)
            {
                if (!strstar(p, "f"))
                {
                    char *tok = strtok_r(p, "/", &p);
                    int i = 0;
                    while (tok!=NULL)
                    {
                        switch (i)
                        {
                        case 0:
                            o->indices[o->size_indices++] = (uint)atoi(tok);
                        break;
                        case 1:
                            o->uvsi[o->size_uvsi++] = (uint)atoi(tok);
                        break;
                        case 2:
                            o->nsi[o->size_nsi++]   = (uint)atoi(tok);
                        break;  
                        }
                        tok = strtok_r(p, "/", &p);
                        i++;
                    }
                }
                p = strtok(NULL, " ");
            }
        }
    }
    fclose(fp);
}

const float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};

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

    uint shaderprogram = createShader("res/shaders/default");  

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // glBindBuffer(GL_ARRAY_BUFFER, 0); 
    //    glBindVertexArray(0); 
    

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

        glUseProgram(shaderprogram);
        glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);        
        //glDrawElements( GL_TRIANGLES, sizeof(vertices)/sizeof(*vertices), GL_UNSIGNED_INT, 0 );
	
	/*       */
	glfwSwapBuffers(w);
	glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
