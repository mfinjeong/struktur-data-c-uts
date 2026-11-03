# Struktur Data C — latihan UTS modul 1–6

Kumpulan **13 program C11 mandiri** untuk belajar, membaca kode, melakukan dry run, dan latihan UTS. Penjelasan serta contoh implementasi ditulis khusus untuk latihan ini; tidak menyertakan salinan slide/PDF kuliah. Ini bahan belajar pribadi, bukan pengganti penjelasan dosen atau jawaban untuk diserahkan sebagai tugas.

**Batas materi:** pengantar struktur data dan kompleksitas, array **satu dimensi** statis/dinamis, pencarian, lima sorting, **singly linked list**, stack dan aplikasi notasi, queue linear/circular/priority. Tidak ada array multidimensi, doubly/circular linked list, tree, graph, atau proyek modul 7–9.

## 1. Mulai menjalankan

Tidak ada dependensi pustaka tambahan. Setiap `.c` mempunyai `main` sendiri: **kompilasi satu berkas**, bukan semua `main` sekaligus. Gunakan GCC, Clang, atau Zig dengan dukungan C11. Python diperlukan hanya untuk tes otomatis.

Dari direktori repositori, buat folder `build` lebih dahulu (`mkdir build` di PowerShell/cmd; `mkdir -p build` di bash).

```sh
# GCC; ganti gcc dengan clang jika menggunakan Clang
gcc -std=c11 -Wall -Wextra -Werror -pedantic modul02/array_statis.c -o build/array_statis
# Windows: tambahkan .exe pada nama keluaran bila diperlukan
./build/array_statis

# Zig (pastikan zig tersedia di PATH)
zig cc -std=c11 -Wall -Wextra -Werror -pedantic modul03/sorting.c -o build/sorting
./build/sorting
```

Untuk menjalankan di PowerShell: `./build/array_statis.exe`. Jika compiler tidak di PATH, gunakan path compiler milik Anda; jangan menyalin binary compiler ke repositori.

### Tes integrasi

```sh
python tests/run_tests.py --cc gcc
python tests/run_tests.py --cc clang
python tests/run_tests.py --cc 'zig cc'
# Atau env CC; quoting contoh berikut untuk bash:
CC='zig cc' python tests/run_tests.py
# Filter satu sumber/modul:
python tests/run_tests.py --cc 'zig cc' --only modul05
```

`--cc` menerima satu string perintah, termasuk argumen compiler. Gunakan **forward slash** untuk path Windows. Untuk path yang mengandung spasi, sertakan kutip ganda DI DALAM nilai argumen, misalnya di bash/PowerShell:

```sh
python tests/run_tests.py --cc '"C:/Program Files/zig/zig.exe" cc'
```

Runner stdlib Python memanggil compiler dan executable sungguhan (bukan simulasi), memakai flag `-std=c11 -Wall -Wextra -Werror -pedantic`, batas waktu kompilasi/eksekusi, dan membandingkan baris output yang diharapkan secara berurutan. Build dan laporan JSON lokal berada di `build/` dan diabaikan Git. MSVC bukan target flag runner ini.

**Verifikasi aktual:** Zig 0.14.1 di Windows, **172/172 kasus lolos**, seluruh **13 program** terkompilasi tanpa warning dengan flag di atas. Tes mencakup CRUD/batas, array tumbuh/penuh, kosong, negatif/nol/duplikat, pencarian gagal, seluruh sorting dua arah dan data terurut/terbalik, penghapusan head/tail linked list, LIFO/FIFO, reuse/wraparound, FIFO prioritas sama, kesalahan notasi, pembagian nol, overflow, perintah salah, dan pemulihan setelah baris terlalu panjang. Jalankan ulang pada mesin sendiri; laporan aktual tersimpan di `build/test-report.json`.

Urutan kerja awal: kasus tes ditulis sebelum sumber C; eksekusi pertama terblokir karena compiler belum tersedia. Setelah compiler tersedia, kompilasi sebelum sumber ada gagal, lalu sumber dibuat dan seluruh kasus dieksekusi hingga hijau. Warm-up compiler Zig sempat melewati batas waktu awal; runner memberi batas kompilasi 360 detik agar build cache pertama dapat selesai. Kegagalan sebelum implementasi itu bukan klaim bahwa tes perilaku sempat berjalan pada program yang belum ada.

## 2. Peta berkas dan cara membaca

| Modul | Berkas | Fokus |
|---|---|---|
| 01 | `modul01/kompleksitas.c` | menghitung model operasi O(1), O(n), O(n²) |
| 02 | `modul02/array_statis.c` | CRUD array 1D kapasitas delapan |
| 02 | `modul02/array_dinamis.c` | CRUD, `malloc` melalui `realloc(NULL, ...)`, pertumbuhan kapasitas, `free` |
| 03 | `modul03/linear_search.c` | pencarian sekuensial, indeks pertama |
| 03 | `modul03/binary_search.c` | lower bound pada array naik, indeks pertama |
| 03 | `modul03/sorting.c` | bubble, selection, insertion, merge, quick; naik/turun |
| 04 | `modul04/singly_linked_list.c` | node satu arah, head/tail, insert/delete/find/free |
| 05 | `modul05/stack_array.c` | push/pop/peek, LIFO, overflow/underflow kapasitas |
| 05 | `modul05/infix_postfix.c` | konversi notasi dengan stack operator |
| 05 | `modul05/postfix_evaluation.c` | evaluasi dengan stack operand bertipe int |
| 06 | `modul06/queue_linear.c` | FIFO array, dequeue menggeser isi |
| 06 | `modul06/queue_circular.c` | FIFO array, indeks modulo kapasitas |
| 06 | `modul06/priority_queue.c` | array terurut, prioritas kecil lebih dahulu |

Baca berurutan: kontrak input → komentar invarian → cabang operasi → tes. Helper input sengaja diulang agar masing-masing berkas bisa dikompilasi tanpa header proyek. Algoritme tidak disembunyikan di framework generik.

## 3. Kontrak input/output umum

- Input berupa teks, **satu perintah per baris**; huruf perintah harus sama persis (lowercase).
- Tidak ada prompt/menu berulang yang mengotori output otomatis. Tabel perintah di bawah adalah menu yang dapat digunakan secara interaktif.
- Program berperintah berakhir dengan `quit` atau EOF (Ctrl+D di terminal Unix, Ctrl+Z lalu Enter di banyak terminal Windows). Memori list/array dinamis dibebaskan juga saat EOF.
- Perintah perubahan yang berhasil mencetak `OK`. `print` mencetak `DATA [...]`. Tidak ada koma antara nilai. Indeks selalu **berbasis nol**.
- Bilangan adalah desimal bertanda dalam rentang `int` compiler (`INT_MIN..INT_MAX`; biasanya −2147483648..2147483647). Overflow parsing ditolak. `+3` dan `-3` sah.
- Untuk program berperintah, maksimal 511 karakter per baris; baris lebih panjang dibuang dan dilaporkan `ERROR baris panjang`, lalu input berikutnya dapat dibaca. Baris kosong diabaikan.
- Jumlah argumen harus tepat. Perintah tidak dikenal/argumen bukan bilangan/argumen ekstra menghasilkan `ERROR perintah`. Indeks negatif/di luar rentang menghasilkan `ERROR indeks`.
- Kesalahan input yang ditangani adalah kejadian latihan yang dicetak ke stdout, bukan crash: program mengembalikan status 0. Runner memeriksa isi pesan, bukan menganggap exit 0 saja cukup.
- Tidak memakai `gets`, `fflush(stdin)`, `conio.h`, `system("clear")`, scanf tanpa batas, VLA, atau array berukuran nol.

### Modul 01 — model operasi

Masukkan satu `n` per baris, `0 <= n <= 1000000`. Keluaran:

```text
Input:
5
0
Output:
n=5 konstan=1 linear=5 kuadrat=25
n=0 konstan=1 linear=0 kuadrat=0
```

Model: satu akses = 1, satu lintasan = n, dua loop masing-masing n = n². Program **menghitung rumus** dengan `long long`, bukan benar-benar menjalankan n² iterasi: runtime program demonstrasinya O(1) per baris, model algoritme yang dipelajari berbeda. Tidak mengukur waktu wall-clock. Big-O menjelaskan pertumbuhan, bukan detik pasti. Best/average/worst selalu harus menyebut kondisi input; biaya membaca/menampilkan data juga perlu dipisahkan dari inti algoritme.

### Modul 02 — array statis dan dinamis

| Perintah | Arti |
|---|---|
| `append nilai` | tambah di belakang |
| `insert posisi nilai` | sisip, posisi sah `0..n` |
| `set posisi nilai` | ubah nilai, posisi sah `0..n-1` |
| `erase posisi` | hapus dan rapatkan |
| `print` | tampilkan isi logis |
| `quit` | selesai |

Statis: `cap=8`, penuh → `ERROR penuh`. Dinamis: awal `a=NULL,n=cap=0`, kapasitas 2,4,8,…; kegagalan `realloc` → `ERROR memori` tanpa kehilangan pointer/isi lama. Pertumbuhan berikutnya melebihi 1.000.000 ditolak (`ERROR kapasitas`); karena doubling, kapasitas terbesar yang tercapai adalah 524288. Batas praktis ini bukan batas teori dynamic array. Kapasitas tidak dikecilkan saat erase.

```text
Input:
append 10
append 30
insert 1 20
set 0 -5
print
erase 1
print
quit
Output:
OK
OK
OK
OK
DATA [-5 20 30]
OK
DATA [-5 30]
```

**Invarian:** `0<=n<=cap`, hanya `a[0..n)` berisi elemen logis. Insert dari kanan ke kiri; erase dari kiri ke kanan. Alamat elemen array dinamis dapat berubah setelah realloc, sehingga pointer ke elemen lama tidak boleh disimpan dan dipakai lagi.

### Modul 03 — pencarian

Baris pertama: `n nilai0 nilai1 ...`, tepat n nilai, `0<=n<=128`. Baris berikutnya satu target per baris; lanjut sampai EOF. Baris array maksimal 4095 karakter. `ERROR input` untuk format/rentang salah; format array salah mengakhiri program. Target salah dilaporkan, lalu dapat membaca target berikutnya.

```text
Input:
5 -3 0 0 7 9
0
8
-3
Output (linear atau binary):
INDEX 1
INDEX -1
INDEX 0
```

`INDEX -1` berarti tidak ditemukan, bukan nilai data −1. Keduanya mengembalikan indeks duplikat **pertama**. Binary mewajibkan data nondecreasing/naik (duplikat diizinkan), menolak data tidak terurut dengan `ERROR urutan`, **tidak** mengurutkan diam-diam.

Binary memakai interval `[lo,hi)` dengan `hi=n`. Jika `a[mid]<target`, buang sisi kiri melalui `lo=mid+1`; selain itu `hi=mid`. Setelah interval kosong, cek kandidat `lo`. Cara ini berbeda dari versi binary search yang langsung return ketika sama: versi tersebut belum tentu mendapatkan duplikat pertama.

### Modul 03 — sorting

Baris pertama: `algoritme arah`. Algoritme: `bubble`, `selection`, `insertion`, `merge`, `quick`; arah: `asc` atau `desc`. Baris kedua mengikuti format array pencarian. Program mengurutkan sekali lalu selesai. Arah/algoritme salah ditolak, bukan fallback diam-diam.

```text
Input:
merge asc
5 3 -2 9 0 3
Output:
DATA [-2 0 3 3 9]
```

| Algoritme | Best | Average | Worst | Ruang bantu algoritme | Stabil? |
|---|---|---|---|---|---|
| Bubble + early stop | O(n) | O(n²) | O(n²) | O(1) | ya |
| Selection | O(n²) | O(n²) | O(n²) | O(1) | tidak |
| Insertion | O(n) | O(n²) | O(n²) | O(1) | ya |
| Merge | O(n log n) | O(n log n) | O(n log n) | O(n), stack O(log n) | ya |
| Quick Lomuto, pivot terakhir | O(n log n) | O(n log n) | O(n²) | stack average O(log n), worst O(n) | tidak |

Tabel untuk n yang bertumbuh; n=0/1 merupakan kasus konstan. Source menyediakan buffer merge tetap 128 elemen karena batas input; analisis O(n) menggambarkan algoritme jika kapasitas mengikuti n. Semua pilihan berbagi buffer demonstrasi ini, sehingga bedakan ruang algoritme dari total penyimpanan lokal program. Stabil berarti urutan relatif item berkunci sama terjaga; pada output integer saja identitas item sama tidak terlihat. Quick pivot terakhir sengaja sederhana, bukan pilihan tahan kasus terburuk pada input terurut atau semua duplikat.

### Modul 04 — singly linked list

| Perintah | Arti |
|---|---|
| `front nilai`, `back nilai` | tambah node depan/belakang |
| `insert posisi nilai` | sisip pada `0..n` |
| `erase posisi` | hapus pada `0..n-1` |
| `find nilai` | indeks kecocokan pertama atau −1 |
| `print`, `clear`, `quit` | traversal, bebaskan semua, keluar |

`head` dan `tail` adalah NULL ketika kosong; node terakhir selalu `next=NULL`. Insert back memakai tail O(1), tetapi **delete tail tetap O(n)** karena perlu mencari pendahulunya. Menghapus satu-satunya node harus membuat head **dan** tail NULL. Setiap node diperoleh dengan malloc, diperiksa kegagalannya, lalu dibebaskan tepat sekali. Jumlah node dibatasi satu juta agar penghitung aman. `clear` aman diulang pada list kosong.

```text
Input:
front 2
back 4
insert 1 3
print
erase 0
erase 1
print
clear
print
Output:
OK
OK
OK
DATA [2 3 4]
OK
OK
DATA [3]
OK
DATA []
```

### Modul 05 — stack

Kapasitas delapan. Perintah: `push nilai`, `pop`, `peek`, `print`, `quit`. `print` berurutan **bottom ke top**. Pop mencetak `POP nilai`, peek `TOP nilai`; kosong `ERROR kosong`, penuh `ERROR penuh`. Invarian: ukuran `n`; top berada di `a[n-1]` hanya bila `n>0`.

```text
Input:                 Output:
push 10                OK
push -2                OK
peek                   TOP -2
pop                    POP -2
pop                    POP 10
pop                    ERROR kosong
```

### Modul 05 — konversi infix → postfix

Satu ekspresi per baris hingga EOF. **Grammar dibatasi:** operand tepat satu huruf ASCII `A..Z` atau `a..z`; operator **biner** `+ - * /`, tanda kurung dan whitespace. Tidak mendukung angka, nama panjang `total`, perkalian implisit `A(B)`, unary `-A`, pangkat, ataupun assignment. `A B` juga salah, bukan operand `AB`.

Precedence: `* /` di atas `+ -`; seluruh operator left-associative. Pop operator dengan precedence **lebih besar atau sama** sebelum push operator berikutnya. Kurung tidak ikut output. State `need_operand` memeriksa urutan operand/operator. Stack overflow/underflow, kurung tidak seimbang, dan bentuk tidak sah → `ERROR ekspresi`; tidak mencetak output parsial.

```text
Input:
(A+B)*C
A-B-C
A++B
Output:
POSTFIX A B + C *
POSTFIX A B - C -
ERROR ekspresi
```

### Modul 05 — evaluasi postfix

Terpisah dari konverter simbolik di atas. Operand berupa **integer bertanda, dapat multidigit**; **setiap token dipisahkan whitespace**. Contoh sah: `12 -3 * 2 +`, bukan `12-3*2+`. Operator biner `+ - * /`. Pop **kanan dahulu**, lalu kiri; hitung `kiri operator kanan`. Di akhir harus tepat satu nilai tersisa. Tidak mendukung variabel, pecahan, pangkat, atau notasi scientific.

```text
Input:
12 -3 * 2 +
8 3 /
1 0 /
1 2
Output:
HASIL -34
HASIL 2
ERROR ekspresi
ERROR ekspresi
```

Pembagian C11 dipotong menuju nol (`-8/3=-2`). Pembagian nol, `INT_MIN / -1`, overflow tambah/kurang/kali, operand kurang, hasil berlebih, token tidak sah, ekspresi kosong, atau baris lebih panjang dari 511 karakter menghasilkan `ERROR ekspresi`. Pemeriksaan dilakukan **sebelum** operasi yang bisa overflow; mengganti tipe hasil setelah operasi tidak akan memperbaiki undefined behavior.

### Modul 06 — queue linear dan circular

Kapasitas delapan. Perintah: `enqueue nilai`, `dequeue`, `peek`, `print`, `quit`. Dequeue mencetak `DEQUEUE nilai`; peek `FRONT nilai`; print urutan layanan front ke rear. Penuh/kosong tidak mengubah isi.

- **Linear:** isi aktif di `a[0..n)`; dequeue menggeser n−1 elemen agar slot bekas dapat dipakai lagi. Bukan versi linear yang menyia-nyiakan slot depan sampai reset.
- **Circular:** `front` dan ukuran `n`; posisi enqueue `(front+n)%8`, dequeue memajukan `front=(front+1)%8`. `n=0` kosong, `n=8` penuh. Semua delapan slot dapat digunakan, tanpa menyisakan satu slot pembeda.

```text
Input:                 Output:
enqueue 5              OK
enqueue 0              OK
dequeue                DEQUEUE 5
enqueue -1             OK
print                  DATA [0 -1]
```

#### Mengapa circular queue bukan circular linked list?

Circular queue di sini menggunakan **array satu dimensi**, dengan indeks integer yang berputar memakai modulo. Tidak ada node, alokasi per node, atau pointer `next` yang menyambung ke head. Larangan circular **linked list** pada cakupan belajar tidak melarang circular **queue array**. Singly linked list modul 04 tetap linear, `tail->next=NULL`.

### Modul 06 — priority queue

Perintah `enqueue nilai prioritas`, `dequeue`, `peek`, `print`, `quit`. **Angka prioritas lebih kecil dilayani lebih dahulu**; prioritas boleh nol/negatif. Prioritas sama dilayani FIFO berdasarkan urutan enqueue. Bukan heap/tree: implementasi array terurut sederhana. Insert hanya menggeser prioritas **lebih besar**, bukan sama, agar stabil tanpa penghitung timestamp yang bisa overflow.

```text
Input:
enqueue 10 2
enqueue 20 1
enqueue 30 1
print
dequeue
dequeue
Output:
OK
OK
OK
DATA [20:1 30:1 10:2]
DEQUEUE 20 PRIORITY 1
DEQUEUE 30 PRIORITY 1
```

## 4. Ringkasan kompleksitas operasi

Analisis inti, tidak termasuk input/output atau allocator. Untuk array/list berukuran logis n:

| Struktur/operasi | Best | Average/worst | Ruang penyimpanan |
|---|---|---|---|
| Array akses/set | O(1) | O(1) | statis kapasitas tetap; dinamis O(cap) |
| Array insert/erase | O(1) di ujung | O(n) | O(1) bantu di luar kapasitas |
| Array statis append | O(1) | O(1) | delapan int |
| Array dinamis append | O(1) | amortized O(1), worst O(n) | O(cap), realloc bisa perlu blok sementara |
| Linear search | O(1) | O(n) | O(1) bantu |
| Binary lower bound | O(log n) untuk n>1 | O(log n) | O(1) bantu; validasi awal O(n) |
| SLL front/back | O(1) | O(1) | O(n) total node |
| SLL insert/erase posisi, find | O(1) awal | O(n) | O(1) bantu per operasi |
| SLL clear/print | O(n) | O(n) | clear O(1) bantu, iteratif |
| Stack push/pop/peek | O(1) | O(1) | delapan int |
| Queue linear enqueue/peek | O(1) | O(1) | delapan int |
| Queue linear dequeue | O(1) satu elemen | O(n) | O(1) bantu |
| Queue circular enqueue/dequeue/peek | O(1) | O(1) | delapan int |
| Priority enqueue/dequeue | O(1) | O(n) | delapan pasangan |
| Priority peek | O(1) | O(1) | O(1) bantu |
| Infix/postfix dengan stack | O(L) | O(L) | O(L), dibatasi panjang baris |

Secara literal program dengan batas kapasitas tetap memiliki batas runtime/ruang tetap. Notasi n di sini mengajarkan biaya ketika ukuran/kapasitas diizinkan bertumbuh. **Average** mengandaikan distribusi input; **amortized** merata-ratakan biaya suatu rangkaian operasi, bukan menyatakan setiap operasi murah. Print semua struktur O(n).

## 5. Dry run untuk UTS

### A. Insert array

Awal `[10,30,40]`, `n=3`, sisip nilai 20 pada posisi 1:

1. `i=3`: salin `a[2]` ke `a[3]` → `[10,30,40,40]`.
2. `i=2`: salin `a[1]` ke `a[2]` → `[10,30,30,40]`.
3. Simpan `a[1]=20`, naikkan n → `[10,20,30,40]`.

Jika geser dari kiri ke kanan, nilai yang belum disalin dapat tertimpa.

### B. Binary lower bound

Array `[-3,0,0,7,9]`, target 0:

| lo | hi eksklusif | mid | keputusan |
|---:|---:|---:|---|
| 0 | 5 | 2 | a[2]=0, hi=2 |
| 0 | 2 | 1 | a[1]=0, hi=1 |
| 0 | 1 | 0 | a[0]=-3, lo=1 |

Berhenti `lo=hi=1`; `a[1]==0`, hasil indeks 1.

### C. Insertion sort

`[4,2,3,1]` naik: setelah key 2 → `[2,4,3,1]`; key 3 → `[2,3,4,1]`; key 1 → `[1,2,3,4]`. Prefix sebelum key selalu terurut; jumlah pergeseran 1+1+3=5.

### D. Stack operator dan postfix

`A+B*C`: output `A`, push `+`, output `B`, push `*` (lebih tinggi dari `+`), output `C`; akhir pop `*` lalu `+`. Hasil `A B C * +`.

Evaluasi `12 -3 * 2 +`: stack `[] → [12] → [12,-3] → [-36] → [-36,2] → [-34]`. Sisa satu nilai sah. Untuk `8 2 -`, kanan=2, kiri=8, bukan sebaliknya.

### E. Circular queue wraparound

Enqueue 0..7: `front=0,n=8`. Dequeue dua kali: `front=2,n=6`. Enqueue −1 menulis slot `(2+6)%8=0`, lalu enqueue 0 menulis slot `(2+7)%8=1`. Urutan logis `[2,3,4,5,6,7,-1,0]`, bukan urutan fisik mulai slot nol.

### F. Pointer singly linked list

`head → [2] → [4] → NULL`, tail menunjuk 4. Sisip 3 di posisi 1: buat node baru; `baru->next=previous->next` terlebih dahulu, baru `previous->next=baru`. Hapus tail: cari node sebelumnya (3), ubah `previous->next=NULL`, set tail=previous, free node 4. Jangan membaca `old->next` sesudah free.

## 6. Latihan dan jawaban singkat

Coba jawab tanpa menjalankan program, lalu cocokkan:

1. Apa perbedaan panjang dan kapasitas array dinamis setelah lima append dari kosong?
2. Mengapa insert posisi n sah tetapi set posisi n tidak sah?
3. Linear search pada `[5,2,5]` mencari 5 mengembalikan berapa? Mengapa tidak bisa langsung memakai binary pada data itu?
4. Algoritme sorting mana yang paling mudah bagus untuk data hampir terurut? Apakah selection ikut menjadi O(n)?
5. Apa kasus buruk quick pivot terakhir pada `[1,2,3,4]`? Bagaimana jika semua nilai sama?
6. Setelah `front 1`, `erase 0`, lalu `back 2`, apa head/tail? Apa yang salah bila tail lama tidak diperbarui?
7. Stack: push 3, push 8, pop, push −1, pop, pop. Urutan nilai keluar?
8. Konversi `(A-B)/C+D` dan evaluasi `20 4 / 2 -`.
9. Queue: enqueue 3, enqueue 8, dequeue, enqueue −1, dequeue. Urutan keluar?
10. Priority: enqueue (A,2), (B,0), (C,0). Urutan layanan? Apakah priority queue sama dengan FIFO murni?
11. Mengapa menyimpan hasil `realloc` langsung ke pointer asli berisiko?
12. Mengapa mengecek overflow *setelah* `int c=a*b` bukan solusi?

**Jawaban:**

1. n=5, cap=8; ruang teralokasi tidak sama dengan jumlah elemen sah.
2. Insert menambah slot baru di ujung; set hanya mengganti slot yang sudah ada.
3. Indeks 0. Data tidak terurut; prasyarat binary tidak terpenuhi.
4. Insertion (atau bubble dengan early stop pada data sudah terurut); selection tetap kuadratik.
5. Partisi 3+0 lalu 2+0, dst., waktu O(n²); semua sama juga buruk pada perbandingan ketat Lomuto ini.
6. Head=tail=node 2, next=NULL; tail lama menjadi dangling pointer dan bisa menyebabkan use-after-free.
7. 8, −1, 3 (LIFO).
8. `A B - C / D +`; hasil postfix numerik 3.
9. 3 lalu 8 (FIFO).
10. B,C,A; prioritas boleh mendahului kedatangan, FIFO hanya pada prioritas sama.
11. Jika gagal dan mengembalikan NULL, alamat lama hilang dan bocor; simpan dulu ke temp.
12. Signed overflow sudah undefined behavior sebelum pemeriksaan. Validasi batas sebelum menghitung.

## 7. Batas pengujian dan pengembangan aman

- Tes menjalankan program asli pada input normal dan kesalahan deterministik; bukan pembuktian formal.
- Kegagalan alokasi ditangani di sumber, tetapi tes tidak sengaja menghabiskan RAM atau memaksa allocator gagal.
- Tes batas aritmetika memakai nilai int 32-bit yang sesuai target GCC/Clang/Zig umum; kode menggunakan `INT_MIN/INT_MAX` secara portabel. Compiler dengan int berbeda dapat memerlukan penyesuaian kasus angka batas.
- Input ditujukan untuk teks terminal biasa, bukan stream biner dengan byte NUL tertanam.
- Setiap contoh sengaja kecil dan memakai kapasitas latihan, bukan container produksi/thread-safe.
- Tidak ada binary/SDK di sumber. Tidak ada penetapan lisensi pihak lain atau klaim menyalin bahan kuliah.
