# foolavc

This project is [foolav](https://github.com/hvqzao/foolav) continuation.

Original [foolav](https://github.com/hvqzao/foolav) was offered only as x86 executable, used single encoding for externally kept payload file. Once foolav is executed, payload is loaded into memory and executed as a shellcode in separate thread.

foolavc on the other hand supports both x86 and x86_64 architectures, allows use of both internal (built-in) or external payloads. Those can be interpreted in one of three ways: shellcode, DLL and EXEcutable.

## Functionality

Once started binary checks for content of `xl variable (payload.h)`. If set, it will be loaded into memory as DLL. Same thing happens for `xe variable` (loaded and executed in memory as PE executable file). `xf variable`, if set, will be executed as shellcode. In order to generate payload, `x-payload.py` script is provided.

External payloads should be stored in files in the same directory as executable. Those should be encoded using `m-payload.py` scipt. `.ml` is threated as DLL, `.me` as EXE and `.mf` as shellcode.

## Hints

 - Remember to be use **consistent architecture** between binary and used payloads.

 - Once first present payload is executed, program waits for an input and is terminated. pause is used to wait for an input. If needed, this can be commented out or just bypassed - i.e. `echo | pause`

## License

[MIT License](LICENSE)

This project uses [MemoryModule](https://github.com/fancycode/MemoryModule) library (MPL 2.0 Licensed)
