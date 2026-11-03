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

/* Modul 6.3: front hanya maju, rear hanya maju; tidak reset saat kosong.
   Aktif data[front..rear]. Enqueue/dequeue/peek/reset O(1), print O(n).
   Kapasitas fisik habis bila rear=4, walaupun size<5 (false overflow).
   Input: enqueue INT, dequeue, peek, print, status, reset, quit. */
#define KAPASITAS 5
int main(void) {
    int data[KAPASITAS], front=0, rear=-1, status;
    char line[512], *t[3];
    while ((status=read_line(line,sizeof line)) != 0) {
        int k, x=0;
        if (status<0) { puts("ERROR baris panjang"); continue; }
        k=tokens(line,t,3);
        if (k==0) continue;
        if (k<0 || (k>1 && !integer(t[1],&x))) { puts("ERROR perintah"); continue; }
        if (k==1 && !strcmp(t[0],"quit")) break;
        if (k==2 && !strcmp(t[0],"enqueue")) {
            if (rear==KAPASITAS-1) puts("ERROR penuh");
            else { data[++rear]=x; puts("OK"); }
        } else if (k==1 && !strcmp(t[0],"dequeue")) {
            if (front>rear) puts("ERROR kosong");
            else printf("DEQUEUE %d\n",data[front++]);
        } else if (k==1 && !strcmp(t[0],"peek")) {
            if (front>rear) puts("ERROR kosong");
            else printf("FRONT %d\n",data[front]);
        } else if (k==1 && !strcmp(t[0],"reset")) { front=0; rear=-1; puts("OK");
        } else if (k==1 && !strcmp(t[0],"status")) {
            printf("STATUS front=%d rear=%d size=%d physical_full=%d logical_full=%d\n",front,rear,rear-front+1,rear==KAPASITAS-1,rear-front+1==KAPASITAS);
        } else if (k==1 && !strcmp(t[0],"print")) {
            printf("DATA [");
            for (int i=front;i<=rear;++i) printf("%s%d",i==front?"":" ",data[i]);
            puts("]");
        } else puts("ERROR perintah");
    }
    return 0;
}
