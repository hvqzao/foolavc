# foolavc

This project is [foolav](https://github.com/hvqzao/foolav) continuation.

Original foolav was offered only as x86 executable, used single encoding for payload kept in separate file which was later executed as shellcode in separate thread.

foolavc supports both x86 and x86_64 architectures, storing payloads both internally and externally as well as three types of payloads - shellcode, dll and executable.

## Functionality

Once started binary checks for content of xl variable (payload.h). If set, it will be loaded into memory as DLL. Same thing happens for xe variable (loaded and executed in memory as PE executable file). xf variable, if set, will be executed as shellcode.

External payloads should be stored in files in the same directory as executable. Those should be encoded using m-payload.py scipt. .ml is threated as DLL, .me as EXE and .mf as shellcode.

Remember to be use same architecture payloads as binary.

Once first present payload is executed, program waits for an input and is terminated.

## License

[MIT License](LICENSE)

This project uses [MemoryModule](https://github.com/fancycode/MemoryModule) library (MPL 2.0 Licensed)