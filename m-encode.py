import os,sys

if len(sys.argv) == 1:
    sys.stderr.write('Usage: {} [files...]\n'.format(sys.argv[0]))
    sys.exit(1)

for i in sys.argv[1:]:
    if os.path.exists(i):
        ext = i.split('.')[-1]
        filename = '.'.join(i.split('.')[:-1])
        extensions = dict(exe='me', dll='ml')
        if ext in extensions.keys():
            filename += '.{}'.format(extensions[ext])
        else:
            filename += '.mf'
        with open(filename,'wb') as f:
            f.write(''.join(map(lambda x: '\\x{:02x}'.format(ord(x)), list(open(i,'rb').read()))))
        print '{} saved.'.format(filename)
