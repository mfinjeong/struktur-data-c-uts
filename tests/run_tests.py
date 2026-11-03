"""Tes integrasi stdlib: kompilasi C asli, kirim stdin, periksa stdout."""
import argparse
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
CASES = []
def case(file, name, data, expected):
    CASES.append((file, name, data, expected))

case('modul01/kompleksitas', 'hitungan', '5\n0\n-1\nx\n', ['n=5 konstan=1 linear=5 kuadrat=25', 'n=0 konstan=1 linear=0 kuadrat=0', 'ERROR input', 'ERROR input'])
for file in ('modul02/array_statis', 'modul02/array_dinamis'):
    case(file, 'CRUD dan batas', 'print\nappend -2\nappend 0\ninsert 1 7\nset 0 9\nprint\nerase 1\nprint\ninsert -1 8\nset 8 1\nerase 9\nwat\nquit\n', ['DATA []', 'DATA [9 7 0]', 'DATA [9 0]', 'ERROR indeks', 'ERROR indeks', 'ERROR indeks', 'ERROR perintah'])
    case(file, 'penuh atau tumbuh', ''.join(f'append {i}\n' for i in range(10))+'print\n', ['ERROR penuh', 'DATA [0 1 2 3 4 5 6 7]'] if 'statis' in file else ['DATA [0 1 2 3 4 5 6 7 8 9]'])
for file in ('modul03/linear_search', 'modul03/binary_search'):
    case(file, 'duplikat negatif nol', '5 -3 0 0 7 9\n0\n8\n-3\n', ['INDEX 1', 'INDEX -1', 'INDEX 0'])
    case(file, 'kosong', '0\n0\n', ['INDEX -1'])
    case(file, 'input salah', '3 1 2\n', ['ERROR input'])
case('modul03/binary_search', 'harus terurut', '3 2 1 3\n1\n', ['ERROR urutan'])
for alg in ('bubble', 'selection', 'insertion', 'merge', 'quick'):
    for direction in ('asc', 'desc'):
        expected = '-2 0 3 3 9' if direction == 'asc' else '9 3 3 0 -2'
        case('modul03/sorting', alg+' '+direction, f'{alg} {direction}\n5 3 -2 9 0 3\n', [f'DATA [{expected}]'])
    case('modul03/sorting', alg+' kosong', f'{alg} asc\n0\n', ['DATA []'])
    case('modul03/sorting', alg+' terurut', f'{alg} asc\n5 -2 0 3 3 9\n', ['DATA [-2 0 3 3 9]'])
    case('modul03/sorting', alg+' terbalik', f'{alg} asc\n5 9 3 3 0 -2\n', ['DATA [-2 0 3 3 9]'])
case('modul03/sorting', 'algoritme salah', 'bogus asc\n0\n', ['ERROR algoritme'])
case('modul04/singly_linked_list', 'semua operasi', 'print\nerase 0\nfront 2\nback 4\ninsert 1 3\ninsert 0 1\ninsert 4 5\nprint\nfind 3\nfind 99\nerase 0\nerase 3\nprint\nerase 1\nprint\nclear\nprint\ninsert -1 7\nquit\n', ['DATA []', 'ERROR indeks', 'DATA [1 2 3 4 5]', 'INDEX 2', 'INDEX -1', 'DATA [2 3 4]', 'DATA [2 4]', 'DATA []', 'ERROR indeks'])
case('modul04/singly_linked_list', 'satu simpul', 'back 0\nerase 0\nfront -1\nfind -1\nclear\nclear\nprint\n', ['INDEX 0', 'DATA []'])
case('modul05/stack_array', 'LIFO penuh kosong', 'pop\npeek\n'+''.join(f'push {i}\n' for i in range(9))+'peek\n'+('pop\n'*9), ['ERROR kosong', 'ERROR kosong', 'ERROR penuh', 'TOP 7'] + [f'POP {i}' for i in reversed(range(8))]+['ERROR kosong'])
case('modul05/stack_array', 'negatif nol', 'push -2\npush 0\nprint\npop\npop\n', ['DATA [-2 0]', 'POP 0', 'POP -2'])
for file in ('modul06/queue_linear', 'modul06/queue_circular'):
    case(file, 'FIFO dan batas', 'dequeue\npeek\n'+''.join(f'enqueue {i}\n' for i in range(9))+'peek\n'+('dequeue\n'*9), ['ERROR kosong', 'ERROR kosong', 'ERROR penuh', 'FRONT 0']+[f'DEQUEUE {i}' for i in range(8)]+['ERROR kosong'])
    case(file, 'reuse wraparound', ''.join(f'enqueue {i}\n' for i in range(8))+'dequeue\ndequeue\nenqueue -1\nenqueue 0\nprint\n', ['DEQUEUE 0', 'DEQUEUE 1', 'DATA [2 3 4 5 6 7 -1 0]'])
case('modul06/priority_queue', 'prioritas stabil', 'dequeue\nenqueue 10 2\nenqueue 20 1\nenqueue 30 1\nenqueue -1 -2\npeek\nprint\n'+('dequeue\n'*5), ['ERROR kosong', 'FRONT -1 PRIORITY -2', 'DATA [-1:-2 20:1 30:1 10:2]', 'DEQUEUE -1 PRIORITY -2', 'DEQUEUE 20 PRIORITY 1', 'DEQUEUE 30 PRIORITY 1', 'DEQUEUE 10 PRIORITY 2', 'ERROR kosong'])
case('modul06/priority_queue', 'penuh', 'enqueue 0 0\n'*9, ['ERROR penuh'])
for expr, result in [('A+B*C','A B C * +'), ('(A+B)*C','A B + C *'), ('A-B-C','A B - C -'), ('a/(b-c)','a b c - /')]:
    case('modul05/infix_postfix', expr, expr+'\n', ['POSTFIX '+result])
for expr in ('', '(A+B', 'A+B)', 'A++B', 'AB', 'A B', '-A', 'A$', '()', 'A(B)', 'A+'):
    case('modul05/infix_postfix', 'invalid '+expr, expr+'\n', ['ERROR ekspresi'])
for expr, result in [('12 -3 * 2 +', '-34'), ('0 5 +', '5'), ('8 3 /', '2'), ('-8 3 /', '-2')]:
    case('modul05/postfix_evaluation', expr, expr+'\n', ['HASIL '+result])
for expr in ('', '1 +', '1 2', '1 0 /', '1 x +', '999999999999999999999999', '2147483647 1 +', '-2147483648 -1 /', '2147483647 2 *', '-2147483648 1 -'):
    case('modul05/postfix_evaluation', 'invalid '+expr, expr+'\n', ['ERROR ekspresi'])
# Semua antarmuka baris harus menolak sampah, baris terlalu panjang, lalu pulih.
for file in ('modul02/array_statis','modul02/array_dinamis','modul04/singly_linked_list','modul05/stack_array','modul06/queue_linear','modul06/queue_circular','modul06/priority_queue'):
    case(file, 'input robust', 'oops\n'+'x'*600+'\nprint extra\nprint\n', ['ERROR perintah','ERROR baris panjang','ERROR perintah','DATA []'])

# Tambahan regresi: batas int target 32-bit, semua-sama, EOF, dan reuse.
for file in sorted({c[0] for c in CASES}):
    case(file, 'EOF tanpa input', '', [] if file not in ('modul03/linear_search', 'modul03/binary_search', 'modul03/sorting') else ['ERROR input'])
for alg in ('bubble', 'selection', 'insertion', 'merge', 'quick'):
    case('modul03/sorting', alg+' semua sama', f'{alg} asc\n5 7 7 7 7 7\n', ['DATA [7 7 7 7 7]'])
    case('modul03/sorting', alg+' int extremes', f'{alg} asc\n4 2147483647 -2147483648 0 -1\n', ['DATA [-2147483648 -1 0 2147483647]'])
for data in ('-1\n', '129\n', '1 2147483648\n', '2 1 2 3\n'):
    for file in ('modul03/linear_search','modul03/binary_search'):
        case(file, 'array invalid '+repr(data), data, ['ERROR input'])
case('modul03/sorting', 'arah salah', 'bubble sideways\n0\n', ['ERROR arah'])
case('modul03/sorting', 'angka bukan int', 'bubble asc\n1 1.5\n', ['ERROR input'])
case('modul04/singly_linked_list', 'duplikat dan batas', 'back -1\nback -1\nfind -1\ninsert 3 2\nerase 2\nerase 0\nfind -1\nprint\n', ['INDEX 0','ERROR indeks','ERROR indeks','INDEX 0','DATA [-1]'])
for file in ('modul06/queue_linear','modul06/queue_circular'):
    case(file, 'drain lalu reuse', 'enqueue 1\ndequeue\nenqueue 0\ndequeue\nprint\n', ['DEQUEUE 1','DEQUEUE 0','DATA []'])
for expr in ('1 2147483647 +', '-2147483648 0 +', '-2147483648 1 *', '0 -2147483648 *', '-2147483648 -2147483648 -', '-1 -1 *'):
    value = {'1 2147483647 +':None, '-2147483648 0 +':-2147483648, '-2147483648 1 *':-2147483648, '0 -2147483648 *':0, '-2147483648 -2147483648 -':0, '-1 -1 *':1}[expr]
    case('modul05/postfix_evaluation', 'batas '+expr, expr+'\n', ['ERROR ekspresi' if value is None else 'HASIL '+str(value)])
for file in ('modul05/infix_postfix','modul05/postfix_evaluation'):
    valid = 'A+B' if file.endswith('infix_postfix') else '1 2 +'
    expected = 'POSTFIX A B +' if file.endswith('infix_postfix') else 'HASIL 3'
    case(file, 'panjang lalu pulih', 'x'*600+'\n'+valid+'\n', ['ERROR ekspresi',expected])

# Batas fgets: cap-1 karakter sah, baik diakhiri newline maupun EOF.
for file, command in [
    ('modul02/array_statis', 'append 7'),
    ('modul02/array_dinamis', 'append 7'),
    ('modul04/singly_linked_list', 'back 7'),
    ('modul05/stack_array', 'push 7'),
    ('modul06/queue_linear', 'enqueue 7'),
    ('modul06/queue_circular', 'enqueue 7'),
    ('modul06/priority_queue', 'enqueue 7 1'),
]:
    for ending in ('\n', ''):
        case(file, 'tepat 511 karakter '+repr(ending), command+' '*(511-len(command))+ending, ['OK'])
        case(file, 'lebih batas 512 karakter '+repr(ending), command+' '*(512-len(command))+ending, ['ERROR baris panjang'])
for file, command, expected in [
    ('modul01/kompleksitas', '5', 'n=5 konstan=1 linear=5 kuadrat=25'),
    ('modul05/infix_postfix', 'A+B', 'POSTFIX A B +'),
    ('modul05/postfix_evaluation', '1 2 +', 'HASIL 3'),
]:
    for ending in ('\n', ''):
        case(file, 'tepat 511 karakter '+repr(ending), command+' '*(511-len(command))+ending, [expected])
for file in ('modul03/linear_search', 'modul03/binary_search'):
    for ending in ('\n', ''):
        case(file, 'target tepat 511 karakter '+repr(ending), '1 7\n7'+' '*510+ending, ['INDEX 0'])
        case(file, 'array tepat 4095 karakter '+repr(ending), '1 7'+' '*4092+ending, [])
for ending in ('\n', ''):
    case('modul03/sorting', 'array tepat 4095 karakter '+repr(ending), 'bubble asc\n1 7'+' '*4092+ending, ['DATA [7]'])

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cc', default=os.environ.get('CC', 'cc'), help='compiler command; quote a path containing spaces inside this string')
    parser.add_argument('--only', help='substring filter on source path')
    args = parser.parse_args()
    selected = [c for c in CASES if not args.only or args.only in c[0]]
    if not selected:
        parser.error('no matching cases')
    compiler = shlex.split(args.cc, posix=True)
    if not compiler or not shutil.which(compiler[0]):
        print('BLOCKED: compiler tidak ditemukan: '+args.cc, file=sys.stderr)
        return 2
    build = ROOT / 'build'
    build.mkdir(exist_ok=True)
    programs = sorted({c[0] for c in selected})
    binaries = {}
    for source in programs:
        binary = build / (source.replace('/', '_') + ('.exe' if os.name == 'nt' else ''))
        cmd = compiler + ['-std=c11','-Wall','-Wextra','-Werror','-pedantic',str(ROOT/(source+'.c')),'-o',str(binary)]
        try:
            result = subprocess.run(cmd, text=True, capture_output=True, timeout=360)
        except (OSError, subprocess.TimeoutExpired) as error:
            print(f'COMPILE FAILED {source}: {error}', file=sys.stderr)
            return 1
        if result.returncode:
            print('COMPILE FAILED '+source+'\n'+result.stdout+result.stderr, file=sys.stderr)
            return 1
        binaries[source] = binary
    results = []
    for source, name, data, expected in selected:
        try:
            p = subprocess.run([str(binaries[source])],input=data,text=True,capture_output=True,timeout=5)
            # Ordered matching, including repeated messages; no fake expected output.
            lines = p.stdout.splitlines()
            cursor = 0
            if not expected:
                assert p.stdout == '', f'unexpected EOF output: {p.stdout!r}'
            for text in expected:
                cursor = lines.index(text, cursor) + 1
            assert p.returncode == 0, f'exit={p.returncode}'
            assert not p.stderr, p.stderr
            results.append({'source':source,'name':name,'passed':True})
        except (ValueError, AssertionError, subprocess.TimeoutExpired) as e:
            print(f'FAIL {source}: {name}: {e}\nstdout={getattr(locals().get("p"),"stdout","")!r}')
            results.append({'source':source,'name':name,'passed':False})
    passed = sum(r['passed'] for r in results)
    report = {'programs':len(programs),'passed':passed,'total':len(results),'compiler':args.cc,'results':results}
    (build/'test-report.json').write_text(json.dumps(report,indent=2),encoding='utf-8')
    print(f'{passed}/{len(results)} passed; {len(programs)} C programs compiled')
    return int(passed != len(results))

if __name__ == '__main__':
    sys.exit(main())
