#ifndef utilsH
#define utilsH

#define UNUSED __attribute__((unused))
#define uint unsigned int

static inline void printargs(int argc, char** argv, char* title)
{
    int titlelen = 0;
    if (title) titlelen = strlen(title);
    const char t = 205; /*═*/
    const char s = 186; /*║*/
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
