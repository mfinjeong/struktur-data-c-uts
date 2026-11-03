"""Regresi Modul 5 dan 6; diimpor runner melalui register(case)."""
def register(case):
    case('queue/queue_linear', 'false overflow dan reset', ''.join(f'enqueue {i}\n' for i in range(1, 6))+'dequeue\ndequeue\nenqueue 6\nstatus\nprint\nreset\nenqueue 9\npeek\n', ['DEQUEUE 1','DEQUEUE 2','ERROR penuh','STATUS front=2 rear=4 size=3 physical_full=1 logical_full=0','DATA [3 4 5]','FRONT 9'])
    case('queue/queue_linear', 'kosong tidak reset otomatis', ''.join(f'enqueue {i}\n' for i in range(5))+'dequeue\n'*5+'enqueue 8\nstatus\n', ['ERROR penuh','STATUS front=5 rear=4 size=0 physical_full=1 logical_full=0'])
    for source, add, remove, peek, values in [('stack/stack_linked_list','push','pop','TOP',[3,2,1]),('queue/queue_linked_list','enqueue','dequeue','FRONT',[1,2,3])]:
        case(source, 'urutan hapus akhir reuse clear', remove+'\n'+''.join(f'{add} {i}\n' for i in (1,2,3))+'peek\nprint\n'+(remove+'\n')*3+add+' -1\npeek\nclear\nprint\nclear\n', ['ERROR kosong',f'{peek} {values[0]}','DATA ['+' '.join(map(str,values))+']']+[f'{remove.upper()} {i}' for i in values]+[f'{peek} -1','DATA []'])
    for expr, expected in [('{[a+(b*c)]-d}','SEIMBANG'),('(a+b]*c','TIDAK SEIMBANG'),('((a+b)*c','TIDAK SEIMBANG'),('','SEIMBANG'),(')','TIDAK SEIMBANG'),('('*101,'ERROR kapasitas')]:
        case('stack/cek_tanda_kurung',repr(expr),expr+'\n',[expected])
    case('stack/cek_tanda_kurung','panjang pulih','x'*600+'\n[]\n',['ERROR baris panjang','SEIMBANG'])
    case('stack/infix_postfix','digit modul','2+3*4\n12+3\na+2\n',['POSTFIX 2 3 4 * +','ERROR ekspresi','POSTFIX a 2 +'])
    for expr, pf, result, depth in [('2+3*4','2 3 4 * +',14,3),('(2+3)*4','2 3 + 4 *',20,2),('8-3-2','8 3 - 2 -',3,2),('((1+2)*(3+4))-5','1 2 + 3 4 + * 5 -',16,4),('2+3*(4-1)/3','2 3 4 1 - * 3 / +',5,4)]:
        case('stack/notasi_ekspresi',expr,expr+'\n',['POSTFIX '+pf,f'HASIL {result}',f'DEPTH {depth}'])
    for expr in ['','12+3','A+2','1/0','1/(2-2)','9*9*9*9*9*9*9*9*9*9','()','2(3)','2++3','(1+2','1+2)']:
        case('stack/notasi_ekspresi','invalid '+expr,expr+'\n',['ERROR ekspresi'])
    case('stack/notasi_ekspresi','panjang pulih','1'*600+'\n8/3\n',['ERROR ekspresi','POSTFIX 8 3 /','HASIL 2','DEPTH 2'])
    case('queue/perbandingan_queue','bukti aktual','',['Linear enqueue(6): DITOLAK','Circular enqueue(6): diterima','Linear front=2 rear=4 size=3','Circular front=2 rear=0 size=4','Linear DATA [3 4 5]','Circular DATA [3 4 5 6]'])
    case('queue/simulasi_antrean','tiga skenario','',['arrival=3 service=2 arrived=40 served=40 rejected=0 remaining=0 peak_instant=1 peak_end_tick=1','arrival=2 service=2 arrived=60 served=60 rejected=0 remaining=0 peak_instant=1 peak_end_tick=0','arrival=2 service=3 arrived=60 served=40 rejected=1 remaining=19 peak_instant=20 peak_end_tick=20'])
    for source, command in [('queue/queue_linear','enqueue 7'),('queue/queue_linked_list','enqueue 7'),('stack/stack_linked_list','push 7')]:
        case(source,'input ketat','oops\n'+command+' extra\n'+command+'999999999999\n'+'x'*600+'\nprint extra\nprint\n',['ERROR perintah']*3+['ERROR baris panjang','ERROR perintah','DATA []'])
        for end in ('\n',''):
            case(source,'batas sah '+repr(end),command+' '*(511-len(command))+end,['OK'])
            case(source,'batas lebih '+repr(end),command+' '*(512-len(command))+end,['ERROR baris panjang'])
