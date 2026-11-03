/* Contoh mandiri C11. Baca kontrak input dan kompleksitas di README.md. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

/* 1 = baris utuh, 0 = EOF, -1 = terlalu panjang; sisa baris dibuang.
   Tidak ada loop gagal scanf: setiap percobaan menghabiskan satu baris. */
static int read_line(char *s, size_t cap) {
    int c;
    size_t n;
    if (fgets(s, (int)cap, stdin) == NULL) return 0;
    n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') { s[n - 1] = '\0'; return 1; }
    c = getchar();
    if (c == '\n' || c == EOF) return 1;
    while (c != '\n' && c != EOF) c = getchar();
    return -1;
}

/* strtol mendeteksi overflow sebelum konversi ke int. */
static int integer(const char *s, int *value) {
    char *end;
    long v;
    if (*s == '\0') return 0;
    errno = 0;
    v = strtol(s, &end, 10);
    if (errno == ERANGE || *end != '\0' || v < INT_MIN || v > INT_MAX) return 0;
    *value = (int)v;
    return 1;
}
/* Token dipisahkan spasi; -1 berarti jumlah token melewati kapasitas. */
static int tokens(char *line, char **out, int cap) {
    int n = 0;
    char *p = strtok(line, " \t\r");
    while (p != NULL) {
        if (n == cap) return -1;
        out[n++] = p;
        p = strtok(NULL, " \t\r");
    }
    return n;
}

/* Hanya singly: next satu arah, tail->next selalu NULL (bukan circular).
   Head/tail NULL iff n=0. Front/back O(1); posisi/search/delete O(n).
   Best posisi awal/search head O(1), avg/worst O(n). Ruang total O(n).
   Setiap malloc berhasil punya satu free pada erase, clear, atau EOF. */
typedef struct Node { int value; struct Node *next; } Node;
static void clear(Node **head, Node **tail, size_t *n) {
    while (*head != NULL) {
        Node *old = *head;
        *head = old->next;
        free(old);
    }
    *tail = NULL; *n = 0;
}
int main(void) {
    Node *head = NULL, *tail = NULL;
    size_t n = 0;
    char line[512], *t[4];
    int status;
    while ((status = read_line(line, sizeof line)) != 0) {
        int k, x = 0, y = 0;
        if (status < 0) { puts("ERROR baris panjang"); continue; }
        k = tokens(line, t, 4);
        if (k == 0) continue;
        if (k < 0 || (k > 1 && !integer(t[1], &x)) ||
            (k > 2 && !integer(t[2], &y))) { puts("ERROR perintah"); continue; }
        if (k == 1 && strcmp(t[0], "quit") == 0) break;

        if (k == 1 && strcmp(t[0], "print") == 0) {
            Node *p = head;
            printf("DATA [");
            while (p != NULL) { printf("%s%d", p == head ? "" : " ", p->value); p = p->next; }
            puts("]");
        } else if (k == 1 && strcmp(t[0], "clear") == 0) {
            clear(&head, &tail, &n); puts("OK");
        } else if (k == 2 && strcmp(t[0], "find") == 0) {
            Node *p = head;
            int index = 0;
            while (p != NULL && p->value != x) { p = p->next; ++index; }
            printf("INDEX %d\n", p == NULL ? -1 : index);
        } else if ((k == 2 && (!strcmp(t[0], "front") || !strcmp(t[0], "back"))) ||
                   (k == 3 && !strcmp(t[0], "insert"))) {
            size_t pos, i;
            int value;
            Node *node;
            if (k == 3) {
                if (x < 0 || (size_t)x > n) { puts("ERROR indeks"); continue; }
                pos = (size_t)x; value = y;
            } else { pos = !strcmp(t[0], "front") ? 0 : n; value = x; }
            /* Batas praktis juga mencegah overflow penghitung indeks int. */
            if (n >= 1000000) { puts("ERROR kapasitas"); continue; }
            node = malloc(sizeof *node);
            if (node == NULL) { puts("ERROR memori"); continue; }
            node->value = value;
            if (pos == 0) {
                node->next = head; head = node;
                if (n == 0) tail = node;
            } else if (pos == n) {
                node->next = NULL; tail->next = node; tail = node;
            } else {
                Node *previous = head;
                for (i = 1; i < pos; ++i) previous = previous->next;
                node->next = previous->next; previous->next = node;
            }
            ++n; puts("OK");
        } else if (k == 2 && !strcmp(t[0], "erase")) {
            Node *old;
            size_t i;
            if (x < 0 || (size_t)x >= n) { puts("ERROR indeks"); continue; }
            if (x == 0) { old = head; head = head->next; }
            else {
                Node *previous = head;
                for (i = 1; i < (size_t)x; ++i) previous = previous->next;
                old = previous->next; previous->next = old->next;
                if (old == tail) tail = previous;
            }
            free(old); --n;
            if (n == 0) tail = NULL;
            puts("OK");
        } else puts("ERROR perintah");
    }
    clear(&head, &tail, &n);
    return 0;
}
