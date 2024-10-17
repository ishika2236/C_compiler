#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
#include <limits.h>
#include <unistd.h>
// char* get_absolute_path(const char* path) {
//     char* abs_path = (char*)malloc(PATH_MAX); // Allocate memory for the absolute path
//     if (abs_path == NULL) {
//         perror("Error allocating memory for absolute path");
//         return NULL;
//     }

//     if (realpath(path, abs_path) == NULL) { // Get the absolute path
//         perror("Error resolving absolute path");
//         free(abs_path);
//         return NULL;
//     }

//     return abs_path; // Return the dynamically allocated absolute path
// }

struct compile_process* compile_process_create(const char* filename, const char* out_filename, int flags){

    FILE* file = fopen(filename, "r");
    if(!file)
    {
        return NULL;
    }   

    FILE* out_file = NULL;
    if(out_filename)
    {
        out_file = fopen(out_filename, "w");
        if(!out_file)
        {
            fclose(file);
            return NULL;
        }
    }
    
     struct compile_process* process = (struct compile_process*)calloc(1, sizeof(struct compile_process));
    if (!process)
    {
        fclose(file);
        if (out_file)
        {
            fclose(out_file);
        }
        return NULL;
    }
    process -> flags = flags;
    process -> cfile.fp = file;
    process -> ofile = out_file;
     process->cfile.abs_path = filename;
    printf("Process Flags: %d\n", process->flags);
    printf("Input File Pointer: %p\n", (void*)process->cfile.fp);
    printf("Output File Pointer: %p\n", (void*)process->ofile);
    printf("Input File Absolute Path: %s\n", process->cfile.abs_path);
    return process;
}   
char compile_process_next_char(struct lex_process* process)
{
    struct compile_process* compiler = process -> compiler;
    compiler -> pos.col += 1;
    char c = getc(compiler->cfile.fp);
    if(c=='\n')
    {
        compiler->pos.line += 1;
        compiler ->pos.col = 0;
    }
    return c;
}

char compile_process_peek_char(struct lex_process* process)
{
    struct compile_process* compiler = process -> compiler;
    char c = getc(compiler->cfile.fp);
    ungetc(c, compiler->cfile.fp);
    return c;
}

void compile_process_push_char(struct lex_process* process, char c)
{
    struct compile_process* compiler = process -> compiler;
    ungetc(c, compiler->cfile.fp);
}