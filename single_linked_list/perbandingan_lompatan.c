/* Praktikum 4 langkah 4: eksperimen nyata tanpa input.
   Lompatan = maju ke next yang tidak NULL saat mencari simpul terakhir.
   Pemasangan link dan free tidak dihitung. Inilah sebab belakang totalnya
   (n-1)(n-2)/2, bukan n(n-1)/2; tabel dicetak dari traversal nyata. */
#include <stdio.h>
#include <stdlib.h>
typedef struct Node { int value; struct Node *next; } Node;
static void clear(Node *head) {
    while (head) { Node *old = head; head = head->next; free(old); }
}
static int build(int n, int back, size_t *hops) {
    Node *head = NULL; *hops = 0;
    for (int i = 0; i < n; ++i) {
        Node *node = malloc(sizeof *node);
        if (!node) { clear(head); return 0; }
        node->value = i; node->next = NULL;
        if (!back) { node->next = head; head = node; }
        else if (!head) head = node;
        else {
            Node *last = head;
            while (last->next) { last = last->next; ++*hops; }
            last->next = node;
        }
    }
    /* Validasi panjang dan urutan, di luar penghitung lompatan. */
    int index = 0;
    for (Node *p = head; p; p = p->next, ++index) {
        if (p->value != (back ? index : n-1-index)) { clear(head); return 0; }
    }
    clear(head); return index == n;
}
int main(void) {
    const int sizes[] = {100, 1000, 5000};
    puts("n depan belakang");
    for (size_t i = 0; i < sizeof sizes / sizeof sizes[0]; ++i) {
        size_t front, back;
        if (!build(sizes[i], 0, &front) || !build(sizes[i], 1, &back)) { puts("ERROR eksperimen"); return 1; }
        printf("%d %zu %zu\n", sizes[i], front, back);
    }
    return 0;
}
