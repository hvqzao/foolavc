import os,sys

if len(sys.argv) < 3:
    sys.stderr.write('Usage:\n cat payload | {} <variable-name> <xorbyte>\n'.format(sys.argv[0]))
    sys.exit(1)

variable_name = sys.argv[1]
xorbyte = int(sys.argv[2])

payload = ''.join(map(lambda x: '{:02x}'.format(ord(x) ^ xorbyte), list(sys.stdin.read())))

sys.stdout.write('// xorbyte = {}\n\n'.format(xorbyte))
sys.stdout.write('char* {}[] = {{\n'.format(variable_name))
index = 0
payload_length = len(payload)
step = 70
while index < payload_length:
    sys.stdout.write('"{}",\n'.format(payload[index:index+step]))
    index += step
sys.stdout.write('};\n');
