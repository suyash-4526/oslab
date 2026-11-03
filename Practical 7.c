/* Practical 7: Full-duplex parent-child communication using two pipes.
   Parent sends a file path to child; child returns the file contents. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define PATH_SIZE 512
#define CHUNK 1024

int main(void) {
    int p_to_c[2], c_to_p[2];
    char path[PATH_SIZE];

    if (pipe(p_to_c) == -1 || pipe(c_to_p) == -1) {
        perror("pipe"); return 1;
    }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid > 0) {
        close(p_to_c[0]);
        close(c_to_p[1]);

        printf("Enter file path: ");
        if (!fgets(path, sizeof(path), stdin)) path[0] = '\0';
        path[strcspn(path, "\n")] = '\0';
        write(p_to_c[1], path, strlen(path) + 1);
        close(p_to_c[1]);

        printf("\n----- File contents -----\n");
        char buf[CHUNK];
        ssize_t n;
        while ((n = read(c_to_p[0], buf, sizeof(buf))) > 0)
            write(STDOUT_FILENO, buf, n);
        close(c_to_p[0]);
        wait(NULL);
    } else {
        close(p_to_c[1]);
        close(c_to_p[0]);

        char filename[PATH_SIZE];
        size_t used = 0;
        char ch;
        while (used < sizeof(filename) - 1 && read(p_to_c[0], &ch, 1) == 1) {
            filename[used++] = ch;
            if (ch == '\0') break;
        }
        filename[used ? used - 1 : 0] = '\0';
        close(p_to_c[0]);

        FILE *fp = fopen(filename, "r");
        if (!fp) {
            const char *msg = "Error: Cannot open the specified file.\n";
            write(c_to_p[1], msg, strlen(msg));
        } else {
            char buf[CHUNK];
            size_t n;
            while ((n = fread(buf, 1, sizeof(buf), fp)) > 0)
                if (write(c_to_p[1], buf, n) < 0) break;
            fclose(fp);
        }
        close(c_to_p[1]);
        _exit(0);
    }
    return 0;
}
