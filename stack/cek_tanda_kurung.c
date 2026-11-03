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

/* Modul 5.5: stack maksimal 100 kurung buka, O(L) waktu, O(100) ruang.
   Nonkurung diabaikan; baris kosong seimbang. Input: ekspresi per baris.
   Overflow stack berbeda dari mismatch; baris >511 karakter dibuang. */
static int balanced(const char *s) {
    char stack[100]; size_t top=0;
    for (;*s;++s) {
        char c=*s;
        if (c=='(' || c=='[' || c=='{') {
            if (top==sizeof stack) return -1;
            stack[top++]=c;
        } else if (c==')' || c==']' || c=='}') {
            char open;
            if (!top) return 0;
            open=stack[--top];
            if ((c==')' && open!='(') || (c==']' && open!='[') || (c=='}' && open!='{')) return 0;
        }
    }
    return top==0;
}
int main(void) {
    char line[512]; int status;
    while ((status=read_line(line,sizeof line))!=0) {
        int result;
        if (status<0) { puts("ERROR baris panjang"); continue; }
        result=balanced(line);
        puts(result<0?"ERROR kapasitas":result?"SEIMBANG":"TIDAK SEIMBANG");
    }
    return 0;
}
