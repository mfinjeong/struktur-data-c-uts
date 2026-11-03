/* PRD - Tujuan: latihan asli Modul 5/6, C11, array 1D atau SLL saja.
 * PRD - Input: satu perintah/ekspresi per baris; EOF mengakhiri program.
 * PRD - Keamanan: validasi batas/input sebelum mutasi; memori selalu dibebaskan.
 */
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

/* INVARIAN: LIFO; head adalah TOP; next menunjuk elemen yang lebih lama.
   Kosong iff head=NULL; tail=NULL setelah simpul terakhir dihapus.
   Tambah/hapus/peek O(1), print/clear O(n), ruang O(n), tanpa batas tetap.
   Input: push INT, pop, peek, print, clear, quit. Gagal malloc: ERROR memori. */
typedef struct Node { int value; struct Node *next; } Node;
static void clear(Node **head, Node **tail) {
    while (*head) { Node *old=*head; *head=old->next; free(old); }
    *tail=NULL;
}
int main(void) {
    Node *head=NULL, *tail=NULL;
    char line[512], *t[3]; int status;
    while ((status=read_line(line,sizeof line)) != 0) {
        int k,x=0;
        if (status<0) { puts("ERROR baris panjang"); continue; }
        k=tokens(line,t,3);
        if (k==0) continue;
        if (k<0 || (k>1 && !integer(t[1],&x))) { puts("ERROR perintah"); continue; }
        if (k==1 && !strcmp(t[0],"quit")) break;
        if (k==2 && !strcmp(t[0],"push")) {
            Node *node=malloc(sizeof *node);
            if (!node) { puts("ERROR memori"); continue; }
            node->value=x;
            node->next=head; head=node; if (!tail) tail=node;
            puts("OK");
        } else if (k==1 && !strcmp(t[0],"pop")) {
            if (!head) puts("ERROR kosong");
            else {
                Node *old=head;
                printf("POP %d\n",old->value);
                head=old->next;
                if (!head) tail=NULL;
                free(old);
            }
        } else if (k==1 && !strcmp(t[0],"peek")) {
            if (!head) puts("ERROR kosong");
            else printf("TOP %d\n",head->value);
        } else if (k==1 && !strcmp(t[0],"clear")) { clear(&head,&tail); puts("OK");
        } else if (k==1 && !strcmp(t[0],"print")) {
            printf("DATA [");
            for (Node *p=head;p;p=p->next) printf("%s%d",p==head?"":" ",p->value);
            puts("]");
        } else puts("ERROR perintah");
    }
    clear(&head,&tail);
    return 0;
}
