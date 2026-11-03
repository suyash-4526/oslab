/* Practical 2: Process Management - fork(), wait(), zombie and orphan */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void sort_array(int a[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
            }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s sort | zombie | orphan\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "sort") == 0) {
        int a[100], n;
        printf("Enter number of elements (max 100): ");
        scanf("%d", &n);
        if (n < 1 || n > 100) return 1;
        printf("Enter elements: ");
        for (int i = 0; i < n; i++) scanf("%d", &a[i]);

        pid_t pid = fork();
        if (pid < 0) { perror("fork"); return 1; }
        if (pid == 0) {
            sort_array(a, n);
            printf("Child sorted array: ");
            for (int i = 0; i < n; i++) printf("%d ", a[i]);
            printf("\n");
            exit(0);
        } else {
            wait(NULL);
            sort_array(a, n);
            printf("Parent sorted array: ");
            for (int i = 0; i < n; i++) printf("%d ", a[i]);
            printf("\n");
        }
    } else if (strcmp(argv[1], "zombie") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Child exiting. PID = %d\n", getpid());
            exit(0);
        } else if (pid > 0) {
            printf("Parent sleeping for 20 seconds. Child may appear as zombie.\n");
            printf("Check in another terminal: ps -el | grep ' Z '\n");
            sleep(20);
            wait(NULL);
            printf("Parent collected child using wait().\n");
        } else perror("fork");
    } else if (strcmp(argv[1], "orphan") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            sleep(3);
            printf("Child PID = %d, new Parent PID = %d\n", getpid(), getppid());
        } else if (pid > 0) {
            printf("Parent exiting. Child will become an orphan.\n");
        } else perror("fork");
    } else {
        printf("Invalid option. Use sort, zombie, or orphan.\n");
    }
    return 0;
}
