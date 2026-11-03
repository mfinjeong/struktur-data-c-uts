/* Konsep 4.1-4.2, Kode 4.1-4.3. Hanya head, bukan tail.
   Perintah baris: front x, back x, after acuan x, delete x, find x,
   print, clear, quit. after/delete/find memilih kemunculan PERTAMA.
   back O(n); front O(1); pencarian O(n). Semua simpul dibebaskan. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
typedef struct Node { int value; struct Node *next; } Node;
static void clear(Node **head) {
    while (*head) { Node *old = *head; *head = old->next; free(old); }
}
static int read_line(char line[], size_t cap) {
    if (!fgets(line, (int)cap, stdin)) return 0;
    if (strchr(line, '\n')) return 1;
    int c = getchar();
    if (c == '\n' || c == EOF) return 1;
    while ((c = getchar()) != '\n' && c != EOF) { }
    return -1;
}
static int integer(char **p, int *out) {
    while (isspace((unsigned char)**p)) ++*p;
    if (!**p) return 0;
    char *end; errno = 0; long value = strtol(*p, &end, 10);
    if (end == *p || errno || value < INT_MIN || value > INT_MAX ||
        (*end && !isspace((unsigned char)*end))) return 0;
    *p = end; *out = (int)value; return 1;
}
int main(void) {
    Node *head = NULL; char line[512]; int status; size_t count = 0;
    while ((status = read_line(line, sizeof line)) != 0) {
        if (status < 0) { puts("ERROR baris panjang"); continue; }
        char *p = line;
        while (isspace((unsigned char)*p)) ++p;
        char *cmd = p;
        while (*p && !isspace((unsigned char)*p)) ++p;
        if (*p) *p++ = '\0';
        int x = 0, y = 0;
        int args = !strcmp(cmd,"after") ? 2 :
            (!strcmp(cmd,"front") || !strcmp(cmd,"back") || !strcmp(cmd,"delete") || !strcmp(cmd,"find")) ? 1 :
            (!strcmp(cmd,"print") || !strcmp(cmd,"clear") || !strcmp(cmd,"quit")) ? 0 : -1;
        int valid = args >= 0;
        if (args >= 1 && !integer(&p, &x)) valid = 0;
        if (args == 2 && !integer(&p, &y)) valid = 0;
        while (isspace((unsigned char)*p)) ++p;
        if (!valid || *p) { puts("ERROR perintah"); continue; }
        if (!strcmp(cmd,"quit")) break;
        if (!strcmp(cmd,"print")) {
            printf("DATA [");
            for (Node *n = head; n; n = n->next) printf("%s%d", n == head ? "" : " ", n->value);
            puts("]"); continue;
        }
        if (!strcmp(cmd,"clear")) { clear(&head); count = 0; puts("OK"); continue; }
        Node *prev = NULL, *found = head; int index = 0;
        if (!strcmp(cmd,"after") || !strcmp(cmd,"delete") || !strcmp(cmd,"find")) {
            while (found && found->value != x) { prev = found; found = found->next; ++index; }
            if (!strcmp(cmd,"find")) { printf("INDEX %d\n", found ? index : -1); continue; }
            if (!found) { puts("ERROR tidak ditemukan"); continue; }
        }
        if (!strcmp(cmd,"delete")) {
            if (prev) prev->next = found->next; else head = found->next;
            free(found); --count; puts("OK"); continue;
        }
        if (count >= 5000) { puts("ERROR penuh"); continue; }
        Node *node = malloc(sizeof *node);
        if (!node) { puts("ERROR alokasi"); clear(&head); return 1; }
        node->value = args == 2 ? y : x; node->next = NULL;
        if (!strcmp(cmd,"front")) { node->next = head; head = node; }
        else if (!strcmp(cmd,"after")) { node->next = found->next; found->next = node; }
        else {
            if (!head) head = node;
            else { Node *last = head; while (last->next) last = last->next; last->next = node; }
        }
        ++count; puts("OK");
    }
    clear(&head); return 0;
}
