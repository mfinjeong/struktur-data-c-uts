/* Konsep 2.4e: konkatenasi (bukan merge terurut), O(n+m) waktu/ruang.
   Input: n nilai... lalu m nilai...; masing-masing 0..128. */
#include "../sorting/input_array.h"
int main(void) {
    int a[128], b[128], hasil[256], n, m;
    if (!baca_array(a, &n) || !baca_array(b, &m) || !selesai_input()) {
        puts("ERROR input"); return 0;
    }
    for (int i = 0; i < n; ++i) hasil[i] = a[i];
    for (int i = 0; i < m; ++i) hasil[n+i] = b[i];
    tampil_array(hasil, n+m);
    return 0;
}
