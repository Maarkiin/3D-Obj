#ifndef utilsH
#define utilsH

#include <stdint.h>

#define UNUSED __attribute__((unused))
#define uint unsigned int

#define DEF_VEC2(T, n) \
typedef struct vec2##n { \
    union { \
        struct { \
            T x; \
            T y; \
        }; \
        T value[2]; \
    }; \
} vec2##n##_t; \
static inline vec2##n##_t vec2##n##_c(T x, T y) { return ((vec2##n##_t){ .value = {x, y}}); }

#define DEF_VEC3(T, n) \
typedef struct vec3##n { \
    union { \
        struct { \
            T x; \
            T y; \
            T z; \
        }; \
        T value[3]; \
    }; \
} vec3##n##_t; \
static inline vec3##n##_t vec3##n##_c(T x, T y, T z) { return ((vec3##n##_t){ .value = {x, y, z}}); }

DEF_VEC2(int,   i);
DEF_VEC2(float, f);
DEF_VEC3(int,   i);
DEF_VEC3(float, f);
DEF_VEC3(uint32_t, u32);

typedef struct {
    union {
        struct {
            int x;
            int y;
            int z;
            int w;
        };
        int value[4];
    };
} vec4i_t;
static inline vec4i_t vec4i_c(int x, int y, int z, int w) { return ((vec4i_t){ .value = {x, y, z, w}}); }

static inline void printargs(int argc, char** argv, char* title)
{
    int titlelen = 0;
    if (title) titlelen = strlen(title);
    const unsigned char t = 205; /*═*/
    const unsigned char s = 186; /*║*/
    int max = 0, m;
    for (int i = 0; i < argc; ++i)
    {
        int slen = strlen(argv[i]);
        if (slen > max) max = slen; 
    }

    if ((max % 2) == 0)
    {
        /*                 header                 */
        m = (max-titlelen)/2;
        printf("%c", 201); /*╔*/
        for (int i = 0; i < m; i++) printf("%c", t);
        printf("%s", title);
        if (titlelen % 2 != 0) m++;
        for (int i = 0; i < m; i++) printf("%c", t);
        printf("%c", 187); /*╗*/
        /*                                        */

        /*                contents                */
        printf("\n");
        for (int i = 0; i < argc; i++)
        {
            m = (max - strlen(argv[i])) / 2;
            printf("%c", s);
            for (int i = 0; i < m; i++) printf(" ");
            printf("%s", argv[i]);
            if (strlen(argv[i]) % 2 != 0) m++;
            for (int i = 0; i < m; i++) printf(" ");
            printf("%c\n", s);
        }
        /*                                        */
    } else
    {
        /*                 header                 */
        m = (max-titlelen)/2;
        printf("%c", 201); /*╔*/
        for (int i = 0; i < m; i++) printf("%c", t);
        printf("%s", title);
        if (titlelen % 2 == 0) m++;
        for (int i = 0; i < m; i++) printf("%c", t);
        printf("%c", 187); /*╗*/
        /*                                        */

        /*                contents                */
        printf("\n");
        for (int i = 0; i < argc; i++)
        {
            m = (max - strlen(argv[i])) / 2;
            printf("%c", s);
            for (int i = 0; i < m; i++) printf(" ");
            printf("%s", argv[i]);
            if (strlen(argv[i]) % 2 == 0) m++;
            for (int i = 0; i < m; i++) printf(" ");
            printf("%c\n", s);
        }
        /*                                        */
    }

    

    /*                 footer                 */
    m = max;
    printf("%c", 200); /*╚*/
    for (int i = 0; i < m; i++) printf("%c", t);
    printf("%c", 188); /*╝*/
    /*                                        */
    printf("\n");
}


#endif
