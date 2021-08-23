import os,sys

if len(sys.argv) == 1:
    sys.stderr.write(f'Usage: {sys.argv[0]} [files...]\n')
    sys.exit(1)

for i in sys.argv[1:]:
    if os.path.exists(i):
        ext = i.split('.')[-1]
        filename = '.'.join(i.split('.')[:-1])
        extensions = dict(exe='me', dll='ml')
        if ext in extensions.keys():
            filename += f'.{extensions[ext]}'
        else:
            filename += '.mf'
        with open(filename,'w') as f:
            f.write(''.join(map(lambda x: f'\\x{x:02x}', list(open(i,'rb').read()))))
        print(f'{filename} saved.')
