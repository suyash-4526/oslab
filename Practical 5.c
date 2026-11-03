/* Practical 5: Banker's Algorithm for deadlock avoidance */
#include <stdio.h>

int main(void) {
    int n, m, alloc[10][10], max[10][10], need[10][10];
    int avail[10], finish[10] = {0}, safe[10], count = 0;

    printf("Enter number of processes and resources: ");
    scanf("%d%d", &n, &m);
    if (n < 1 || n > 10 || m < 1 || m > 10) return 1;

    printf("Enter Allocation matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) scanf("%d", &alloc[i][j]);

    printf("Enter Maximum matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) scanf("%d", &max[i][j]);

    printf("Enter Available resources:\n");
    for (int j = 0; j < m; j++) scanf("%d", &avail[j]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
            if (need[i][j] < 0) {
                printf("Invalid input: allocation exceeds maximum.\n");
                return 1;
            }
        }

    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                int possible = 1;
                for (int j = 0; j < m; j++)
                    if (need[i][j] > avail[j]) possible = 0;

                if (possible) {
                    for (int j = 0; j < m; j++) avail[j] += alloc[i][j];
                    safe[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) break;
    }

    if (count == n) {
        printf("System is in SAFE state.\nSafe sequence: ");
        for (int i = 0; i < n; i++) printf("P%d%s", safe[i], i == n - 1 ? "\n" : " -> ");
    } else {
        printf("System is in UNSAFE state. No safe sequence exists.\n");
    }
    return 0;
}
