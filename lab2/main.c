// NKU COS OS
// Author: zsh
#include "main.h"

char org_src_path[ID_LENGTH];
char org_dst_path[ID_LENGTH];

void handleOptions(int argc, char* argv[]) {
    // No options.
    if (argc == 1) {
        fprintf(stderr, "usage: dircopy [-h] [--help] [-d dir_name] [--directory dir_name] ...\n");
        exit(FAILURE);
    }
    // -h, --help.
    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        printf("MAN INFO\n");
        exit(SUCCESS);
    }
    // -d dir_name, --directory dir_name.
    if (!strcmp(argv[1], "-d") || !strcmp(argv[1], "--directory")) {
        strcpy(org_src_path, argv[2]);
        strcpy(org_dst_path, org_src_path); strcat(org_dst_path, "bak");
#ifdef DEBUG
        printf("Original src-dir path: %s; Original dst-dir path: %s\n", org_src_path, org_dst_path);
#endif
        return;
    }
    fprintf(stderr, "\x1b[31mError: inputing wrong options!!!\x1b[0m\n");
    exit(FAILURE);
}

int isCreated(const char* path) {
    if (access(path, F_OK) == 0 && opendir(path) != NULL)
        return SUCCESS;
    else
        return FAILURE;
}

// Copy file from src to dst.
void copyFile(char* dst_path, char* src_path) {
    FILE* fp_dst = fopen(dst_path, "w"); if (fp_dst == NULL) {
        fprintf(stderr, "\x1b[31mFAILURE: open dst_path: %s; errno = %d\n\x1b[0m", dst_path, errno);
        exit(FAILURE);
    }
    FILE* fp_src = fopen(src_path, "r"); if (fp_src == NULL) {
        fprintf(stderr, "\x1b[31mFAILURE: open src_path: %s; errno = %d\n\x1b[0m", src_path, errno);
        exit(FAILURE);
    }

    char buf = fgetc(fp_src);
    int cnt = 0;
    while (!feof(fp_src)) {
        cnt++;
        fputc(buf, fp_dst);
        buf = fgetc(fp_src);
    }

#ifdef DEBUG
    printf("SUCCESS: copyFile([dst_path]%s, [src_path]%s); copy %d bytes.\n", dst_path, src_path, cnt);
#endif
    fclose(fp_dst);
    fclose(fp_src);
}

// Copy directory from src to dst.
void copyDir(char* dst_path, char* src_path) {
    if (!isCreated(dst_path))
        fprintf(stderr, "\x1b[31m[\n Destination directory is already existed!!!\n dst = %s; src = %s\n]\n\x1b[0m", dst_path, src_path);
    
    if (!mkdir(dst_path, 0775)) {
#ifdef DEBUG
        if (access(dst_path, F_OK) == 0)
            printf("SUCCESS: mkdir([dst_path]%s, 0775)\n", dst_path);
        else
            printf("FAILURE: mkdir([dst_path]%s, 0775)\n", dst_path);
#endif
    } else {
        fprintf(stderr, "\x1b[31m[\n Error: failed to create directory!!!\n dst = %s; src = %s\n]\n\x1b[0m", dst_path, src_path);
        exit(FAILURE);
    }

    DIR* src = opendir(src_path);
    struct dirent* cur_node;
    while ((cur_node = readdir(src)) != NULL) {
        if (!strcmp(cur_node->d_name, ".") || !strcmp(cur_node->d_name, ".."))
            continue;
        char new_dst_path[ID_LENGTH]; strcpy(new_dst_path, dst_path); strcat(new_dst_path, "/"); strcat(new_dst_path, cur_node->d_name);
        char new_src_path[ID_LENGTH]; strcpy(new_src_path, src_path); strcat(new_src_path, "/"); strcat(new_src_path, cur_node->d_name);
        if (cur_node->d_type == DT_DIR) {
            copyDir(new_dst_path, new_src_path);
        } else if (cur_node->d_type == DT_REG) {
            copyFile(new_dst_path, new_src_path);
        } else {
            printf("\x1b[31m[\n Found: d_type = %d; d_name = %s\n new_dst_path = %s; new_src_path = %s ]\n\x1b[0m",
                    cur_node->d_type, cur_node->d_name, new_dst_path,
                    new_src_path);
        }
    }
}

int main(int argc, char* argv[]) {
    handleOptions(argc, argv);
    copyDir(org_dst_path, org_src_path);
}
