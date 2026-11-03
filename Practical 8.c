/* Practical 8: Disk Scheduling - SSTF, SCAN and C-LOOK */
#include <stdio.h>
#include <stdlib.h>

#define MAX 100

void sort(int a[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
            }
}

int main(void) {
    int req[MAX], sorted[MAX], n, head, disk;
    printf("Enter number of requests: ");
    scanf("%d", &n);
    if (n < 1 || n > MAX) return 1;

    printf("Enter request queue: ");
    for (int i = 0; i < n; i++) scanf("%d", &req[i]);
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter disk size (number of cylinders): ");
    scanf("%d", &disk);

    if (disk < 1 || head < 0 || head >= disk) return 1;
    for (int i = 0; i < n; i++)
        if (req[i] < 0 || req[i] >= disk) {
            printf("Invalid request cylinder.\n"); return 1;
        }

    /* SSTF */
    int done[MAX] = {0}, cur = head, total = 0;
    printf("\nSSTF order: ");
    for (int k = 0; k < n; k++) {
        int best = -1, dist = disk + 1;
        for (int i = 0; i < n; i++)
            if (!done[i] && abs(cur - req[i]) < dist) {
                dist = abs(cur - req[i]); best = i;
            }
        done[best] = 1;
        total += dist; cur = req[best];
        printf("%d ", cur);
    }
    printf("\nTotal SSTF seek movement = %d\n", total);

    /* Sort requests */
    for (int i = 0; i < n; i++) sorted[i] = req[i];
    sort(sorted, n);

    /* SCAN: initial direction is toward higher cylinder numbers */
    cur = head; total = 0;
    printf("\nSCAN order (toward higher cylinders first): ");
    int split = 0;
    while (split < n && sorted[split] < head) split++;
    for (int i = split; i < n; i++) {
        printf("%d ", sorted[i]); total += abs(cur - sorted[i]); cur = sorted[i];
    }
    if (cur != disk - 1) { total += abs(cur - (disk - 1)); cur = disk - 1; }
    for (int i = split - 1; i >= 0; i--) {
        printf("%d ", sorted[i]); total += abs(cur - sorted[i]); cur = sorted[i];
    }
    printf("\nTotal SCAN seek movement = %d\n", total);

    /* C-LOOK: go higher, then jump to lowest pending request */
    cur = head; total = 0;
    printf("\nC-LOOK order (toward higher cylinders first): ");
    for (int i = split; i < n; i++) {
        printf("%d ", sorted[i]); total += abs(cur - sorted[i]); cur = sorted[i];
    }
    if (split > 0) {
        total += abs(cur - sorted[0]);
        cur = sorted[0];
        printf("%d ", cur);
        for (int i = 1; i < split; i++) {
            printf("%d ", sorted[i]); total += abs(cur - sorted[i]); cur = sorted[i];
        }
    }
    printf("\nTotal C-LOOK seek movement = %d\n", total);
    return 0;
}
