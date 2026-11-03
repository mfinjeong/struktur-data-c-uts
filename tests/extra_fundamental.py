"""Kontrak contoh fundamental tambahan; semua angka berasal dari operasi nyata."""
def register(case):
    case('pengantar_dan_kompleksitas/perbandingan_langkah', 'praktikum 1', '', ['n sequential binary', '1000 1000 10', '10000 10000 14', '100000 100000 17', '1000000 1000000 20'])
    case('array_1_dimensi/dasar_array', 'inisialisasi dan rerata', '', ['DATA [15 27 34 46 58]', 'KUADRAT [1 4 9 16 25]', 'JUMLAH 5', 'RERATA 36.00'])
    case('array_1_dimensi/penggabungan_array', 'gabung bukan sort', '3 4 -1 4\n2 0 8\n', ['DATA [4 -1 4 0 8]'])
    case('array_1_dimensi/penggabungan_array', 'dua kosong', '0\n0\n', ['DATA []'])
    case('array_1_dimensi/penggabungan_array', 'batas', '129\n', ['ERROR input'])
    for alg in ('bubble', 'selection', 'insertion', 'merge', 'quick'):
        source = 'sorting/'+alg+'_sort'
        for name, data, expected in [('campur','5 3 -2 9 0 3\n','DATA [-2 0 3 3 9]'), ('kosong','0\n','DATA []'), ('terbalik','5 5 4 3 2 1\n','DATA [1 2 3 4 5]'), ('sama','4 7 7 7 7','DATA [7 7 7 7]'), ('ekstrem','3 2147483647 0 -2147483648\n','DATA [-2147483648 0 2147483647]'), ('invalid','2 1\n','ERROR input'), ('overflow','1 2147483648\n','ERROR input'), ('tambahan','1 3 4\n','ERROR input'), ('batas','129\n','ERROR input')]:
            case(source, name, data, [expected])
    case('sorting/perbandingan_sorting', 'lima algoritme tiga pola', '', ['POLA acak n=128', 'POLA terurut n=128', 'bubble banding=127 tukar=0 geser=0 benar', 'selection banding=8128 tukar=0 geser=0 benar', 'insertion banding=127 tukar=0 geser=0 benar', 'quick banding=8128 tukar=8255 geser=0 benar', 'POLA terbalik n=128', 'bubble banding=8128 tukar=8128 geser=0 benar', 'insertion banding=8128 tukar=0 geser=8128 benar'])
    case('single_linked_list/operasi_single_linked_list', 'nilai pertama dan semua posisi', 'print\ndelete 9\nafter 9 3\nfront 2\nback 4\nafter 2 3\nfront 2\nprint\nfind 2\ndelete 2\nprint\ndelete 3\ndelete 4\ndelete 2\nprint\nclear\nquit\n', ['DATA []','ERROR tidak ditemukan','ERROR tidak ditemukan','DATA [2 2 3 4]','INDEX 0','DATA [2 3 4]','DATA []'])
    case('single_linked_list/operasi_single_linked_list', 'input robust', 'front 2147483648\nprint extra\n'+'x'*512+'\nprint\n', ['ERROR perintah','ERROR perintah','ERROR baris panjang','DATA []'])
    for ending in ('\n',''):
        case('single_linked_list/operasi_single_linked_list', 'batas baris '+repr(ending), 'front 7'+' '*504+ending, ['OK'])
    case('single_linked_list/perbandingan_lompatan', 'praktikum 4', '', ['n depan belakang','100 0 4851','1000 0 498501','5000 0 12492501'])
