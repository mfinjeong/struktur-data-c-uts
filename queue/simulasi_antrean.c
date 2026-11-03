/* PRD - Tujuan: latihan asli Modul 5/6, C11, array 1D atau SLL saja.
 * PRD - Input: satu perintah/ekspresi per baris; EOF mengakhiri program.
 * PRD - Keamanan: validasi batas/input sebelum mutasi; memori selalu dibebaskan.
 */
#include <stdio.h>
/* Praktikum 6 langkah 3/4; tanpa stdin, 120 menit, ruang tunggu 20.
   Arrival/service adalah INTERVAL menit, bukan jumlah orang per menit.
   Kedatangan diproses sebelum pelayanan pada tick yang sama.
   peak_instant: setelah kedatangan sebelum pelayanan; peak_end_tick:
   sesudah pelayanan, sesuai AntreanMaks modul. Selisih ini penting.
   FIFO circular array 1D: 0<=size<=20; O(menit), ruang O(20).
   Konservasi: arrived=served+rejected+remaining diperiksa saat runtime.
   Modul menyiratkan penolakan nol pada skenario pertama; skenario ketiga
   tetap harus dihitung, bukan diasumsikan, karena urutan event berpengaruh. */
#define KAPASITAS 20
typedef struct { int data[KAPASITAS],front,rear,size; } Queue;
static int enqueue(Queue *q,int value) {
    if (q->size==KAPASITAS) return 0;
    q->rear=(q->rear+1)%KAPASITAS; q->data[q->rear]=value; ++q->size; return 1;
}
static int dequeue(Queue *q,int *out) {
    if (!q->size) return 0;
    *out=q->data[q->front]; q->front=(q->front+1)%KAPASITAS; --q->size; return 1;
}
static int simulate(int arrival,int service) {
    Queue q={{0},0,-1,0};
    int arrived=0,served=0,rejected=0,peak=0,end_peak=0,x;
    for (int t=1;t<=120;++t) {
        if (t%arrival==0) { ++arrived; if (!enqueue(&q,t)) ++rejected; }
        if (q.size>peak) peak=q.size;
        if (t%service==0 && dequeue(&q,&x)) ++served;
        if (q.size>end_peak) end_peak=q.size;
    }
    printf("arrival=%d service=%d arrived=%d served=%d rejected=%d remaining=%d peak_instant=%d peak_end_tick=%d\n",arrival,service,arrived,served,rejected,q.size,peak,end_peak);
    return arrived==served+rejected+q.size;
}
int main(void) {
    int ok=simulate(3,2); ok=simulate(2,2)&&ok; ok=simulate(2,3)&&ok;
    if (!ok) { fputs("ERROR konservasi\n",stderr); return 1; }
    return 0;
}
