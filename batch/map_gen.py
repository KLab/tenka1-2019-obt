from random import Random
import sys
from pathlib import Path

V = 400
E = 760
maps_dir = Path(sys.argv[1])
num_maps = 240
random = Random(20191221)


def generate(out_path: Path):
    with out_path.open('w') as f:
        num_s = random.randint(10, 20)
        num_t = random.randint(30, 50)
        v_list = list(range(V))
        random.shuffle(v_list)
        s = sorted(v_list[i] for i in range(num_s))
        t = sorted(v_list[i] for i in range(num_s, num_s + num_t))
        f.write(f'{len(s)} {len(t)}\n')
        f.write(' '.join(str(v) for v in s))
        f.write('\n')
        f.write(' '.join(str(v) for v in t))
        f.write('\n')
        f.write(' '.join(str(random.randint(1, 10000)) for _ in range(E)))
        f.write('\n')


for i in range(num_maps):
    generate(maps_dir / str(i + 1))
