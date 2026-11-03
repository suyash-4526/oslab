/* Practical 6: Full-duplex communication using two named FIFOs.
   Parent sends text; child counts characters, words and lines,
   saves the result in fifo_result.txt and sends it back. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

#define F1 "fifo_parent_to_child"
#define F2 "fifo_child_to_parent"
#define BUF 4096

int main(void) {
    char text[BUF], result[256];
    if (mkfifo(F1, 0666) == -1 && errno != EEXIST) { perror("mkfifo"); return 1; }
    if (mkfifo(F2, 0666) == -1 && errno != EEXIST) { perror("mkfifo"); unlink(F1); return 1; }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid > 0) {
        int w = open(F1, O_WRONLY);
        if (w < 0) { perror("open"); return 1; }
        printf("Enter text (finish with an empty line):\n");
        text[0] = '\0';
        char line[512];
        while (fgets(line, sizeof(line), stdin)) {
            if (strcmp(line, "\n") == 0) break;
            if (strlen(text) + strlen(line) >= sizeof(text) - 1) break;
            strcat(text, line);
        }
        size_t len = strlen(text);
        if (write(w, text, len) != (ssize_t)len) perror("write");
        close(w);

        int r = open(F2, O_RDONLY);
        if (r < 0) { perror("open"); return 1; }
        ssize_t n = read(r, result, sizeof(result) - 1);
        if (n > 0) { result[n] = '\0'; printf("\nResult from child:\n%s", result); }
        close(r);
        wait(NULL);
    } else {
        int r = open(F1, O_RDONLY);
        if (r < 0) _exit(1);
        ssize_t n = read(r, text, sizeof(text) - 1);
        if (n < 0) n = 0;
        text[n] = '\0';
        close(r);

        int chars = (int)strlen(text), words = 0, lines = 0, inword = 0;
        for (int i = 0; i < chars; i++) {
            if (text[i] == '\n') lines++;
            if (text[i] == ' ' || text[i] == '\n' || text[i] == '\t')
                inword = 0;
            else if (!inword) { words++; inword = 1; }
        }
        if (chars > 0 && text[chars - 1] != '\n') lines++;

        snprintf(result, sizeof(result),
                 "Characters: %d\nWords: %d\nLines: %d\n", chars, words, lines);
        FILE *fp = fopen("fifo_result.txt", "w");
        if (fp) { fputs(result, fp); fclose(fp); }

        int w = open(F2, O_WRONLY);
        if (w >= 0) { write(w, result, strlen(result)); close(w); }
        _exit(0);
    }

    unlink(F1);
    unlink(F2);
    return 0;
}
