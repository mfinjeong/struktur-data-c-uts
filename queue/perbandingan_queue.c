/* PRD - Tujuan: latihan asli Modul 5/6, C11, array 1D atau SLL saja.
 * PRD - Input: satu perintah/ekspresi per baris; EOF mengakhiri program.
 * PRD - Keamanan: validasi batas/input sebelum mutasi; memori selalu dibebaskan.
 */
#include <stdio.h>
/* Praktikum 6 langkah 2; tanpa stdin. Array 1D kapasitas 5.
   Linear tidak memutar indeks; circular memutar indeks dan menghitung size.
   Enqueue/dequeue O(1), demonstrasi O(KAPASITAS), ruang O(KAPASITAS). */
#define KAPASITAS 5
typedef struct { int data[KAPASITAS], front,rear,size; } Queue;
static int enqueue(Queue *q,int x,int circular) {
    if (circular?q->size==KAPASITAS:q->rear==KAPASITAS-1) return 0;
    q->rear=circular?(q->rear+1)%KAPASITAS:q->rear+1;
    q->data[q->rear]=x; ++q->size; return 1;
}
static int dequeue(Queue *q,int *x,int circular) {
    if (!q->size) return 0;
    *x=q->data[q->front];
    q->front=circular?(q->front+1)%KAPASITAS:q->front+1;
    --q->size; return 1;
}
static void print(const Queue *q,const char *name,int circular) {
    printf("%s DATA [",name);
    for (int i=0;i<q->size;++i) printf("%s%d",i?" ":"",q->data[circular?(q->front+i)%KAPASITAS:q->front+i]);
    puts("]");
}
int main(void) {
    Queue linear={{0},0,-1,0}, circular={{0},0,-1,0}; int x;
    for (int i=1;i<=5;++i) { enqueue(&linear,i,0); enqueue(&circular,i,1); }
    for (int i=0;i<2;++i) { dequeue(&linear,&x,0); dequeue(&circular,&x,1); }
    printf("Linear enqueue(6): %s\n",enqueue(&linear,6,0)?"diterima":"DITOLAK");
    printf("Circular enqueue(6): %s\n",enqueue(&circular,6,1)?"diterima":"DITOLAK");
    printf("Linear front=%d rear=%d size=%d\n",linear.front,linear.rear,linear.size);
    printf("Circular front=%d rear=%d size=%d\n",circular.front,circular.rear,circular.size);
    print(&linear,"Linear",0); print(&circular,"Circular",1);
    return 0;
}
