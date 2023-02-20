#define EOF -1
#define NULL 0

void c4_exit(int x);

void c4_assert(int x);

void *c4_malloc(int sz);

void *c4_calloc(int num, int sz);

void *c4_realloc(void* p, int sz);

void c4_free(void *p);

void c4_print_str(char* str);

void c4_sprintf(char* buf, char* str, int x);

void c4_print_int(int x);

void c4_error_str(char* str);

void c4_error_int(int x);

int c4_strcmp(char *a, char *b);

int c4_strncmp(char *a, char *b, int n);

char* c4_strcpy(char *a, char *b);

void* c4_memcpy(void *a, void *b, int size);

void c4_qsort(void *base, int nmemb, int size, int (*compar)(void *, void *));

int c4_strlen(char *a);

int c4_file_open(char *fname, char *mode);

void c4_file_close(void);

int c4_file_getc(void);

int c4_abs(int x);

int c4_atoi(char* str);

