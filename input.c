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

int *ARRAY;
int SIZE;
void bubble_sort(void)
{
    int temp;
    int i;
    i = 0;
    while (i < SIZE) {
        int j;
        j = 0;
        while (j < SIZE - i - 1) {
            if (ARRAY[j + 1] < ARRAY[j]) {
                temp = ARRAY[j];
                ARRAY[j] = ARRAY[j + 1];
                ARRAY[j + 1] = temp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}
void print_array(void) {
    int i;
    i = 0;
    while (i < SIZE) {
        c4_print_int(ARRAY[i]);
        c4_print_str(" ");
        i = i + 1;
    }
}
int main(int argc, char **argv) {
    SIZE = argc - 1;
    ARRAY = c4_malloc(SIZE * sizeof(int));
    int i;
    i = 0;
    while (i < SIZE) {
        ARRAY[i] = c4_atoi(argv[i + 1]);
        i = i + 1;
    }
    bubble_sort();
    print_array();
    return 0;
}