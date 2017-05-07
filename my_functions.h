#include <ctype.h>
#include <assert.h>

unsigned long FileSize(FILE *file) {
    assert(file);
    fseek(file, 0, SEEK_END);
    unsigned long Size = (unsigned long) ftell(file);
    rewind(file);
    return Size;
}

unsigned char * File_To_Buf_u(FILE * file){
    assert(file);
    unsigned long Size = FileSize(file);
    unsigned char *buf = (unsigned char *) calloc(Size + 1, 1);
    unsigned long New_Size = fread(buf, sizeof(char), Size + 1, file);
    unsigned char * new_buf = (unsigned char *)calloc(New_Size + 1, sizeof(unsigned char));
    for (int i = 0; i < New_Size; i++){
        new_buf[i] = buf[i];
    }
    new_buf[New_Size] = '\0';
    free(buf);
    fclose(file);
    return new_buf;
}


Buf <char> File_To_Buf_s(FILE * file){
    assert(file);
    unsigned long Size = FileSize(file);
    char *buf = (char *) calloc(Size + 1, 1);
    unsigned long New_Size = fread(buf, sizeof(char), Size + 1, file);
    char * new_buf = (char *)calloc(New_Size + 1, sizeof(char));
    for (int i = 0; i < New_Size; i++){
        new_buf[i] = buf[i];
    }
    new_buf[New_Size] = '\0';
    free(buf);
    Buf <char> Buffer = Buf<char>((size_t )New_Size);
    Buffer.set_buf(new_buf);
    return Buffer;
}


unsigned Number_Of_Lines(char * buf){
    assert(buf);
    unsigned i = 0;
    char * a = buf;
    while (1){
        a = strchr(a, '\n');
        if (!a) break;
        a++;
        i++;
    }
    return i;
}

char * The_End_Of_The_Word(char * source){
    assert(source);
    char * ptr = source;
    while (1){
        if (!isalnum(*ptr) && *ptr != '_') break;
        if (*ptr == '\n') break;
        if (*ptr == '\r') break;
        if (*ptr == '\0') break;
        ptr++;
    }
    return ptr;
}

char * Skip_Different_Shit(char * source){
    assert(source);
    char * ptr = source;
    while (1){
        if (*ptr == '/' && *(ptr + 1) == '/'){//Comments
            ptr++;
            while (1){
                if (*ptr == '/' && *(ptr + 1) == '/')
                    break;
                ptr++;
            }
        }
        if (*ptr == '\0')//The end of the file
            break;
        if (isalpha(*ptr) || *ptr == '{' || *ptr == '}')//Not the shit
            break;
        ptr++;
    }
    return ptr;
}

char * New_String_Lessmem(char * new_person){
    assert(new_person);
    char * end = strchr(new_person, '\0');
    char * new_person_lessmem = (char *) calloc(end - new_person + 1, sizeof(char));
    assert(new_person_lessmem);
    strcpy(new_person_lessmem, new_person);
    free(new_person);
    return new_person_lessmem;
}

void To_The_Only_One_Line(char * buf){//replace all the \n to spaces
    assert(buf);
    char * ptr = buf;
    while(1){
        ptr = strchr(ptr, '\n');
        if (!ptr)
            break;
        *ptr = ' ';
    }
    ptr = buf;
    while(1){
        ptr = strchr(ptr, '\r');
        if (!ptr)
            break;
        *ptr = ' ';
    }
}