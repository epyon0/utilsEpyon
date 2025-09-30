# utilsEpyon
A collection of my utilities that will run on linux and windows
## head
### Help:
```
./bin/head -h

Display portion of the begining of a file

./bin/head <FILE>
OR
cat <FILE> | ./bin/head

[-h || --help]        Print this help message
[-v || --verbose]     Turn on verbose output
[-n || --lines] <INT> Print the first <INT> lines, Default: 10
<FILE>                File to process

Return Codes:
  0 = Success
  1 = File inaccessible
  2 = No file or input stream given
  3 = Invalid line limit given
```
### Examples:
```
cat ./src/head.c | ./bin/head
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "../lib/libEpyon/misc.h" // git clone git@github.com:epyon0/libEpyon.git

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
```
./bin/head ./src/head.c -n 13
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "../lib/libEpyon/misc.h" // git clone git@github.com:epyon0/libEpyon.git

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

char dBuff[1024] = {'\0'};
```
```
## hexdump
Dump file contents
### Help:
```
./hexdump -h

Dump file contents

./hexdump <FILE>
OR
cat <FILE> | ./hexdump -i

[-h || --help]        Print this help message
[-v || --verbose]     Turn on verbose output
[-b || --bin]         Toggle binary output, default: false
[-x || --hex]         Toggle hexadecimal output, default: true
[-o || --oct]         Toggle octal output, default: false
[-d || --dec]         Toggle decimal output, default: false
[-a || --ascii]       Toggle ASCII output, default: false
[-i || --stdin]       Read file from STDIN instead of parameter
[-l || --len] <INT>   Number of bytes displayed before wrapping to next line, default: 8
[-f || --file] <FILE> Path to file
```
### Examples:
```
cat hexdump.c | ./hexdump -i | head
0x0000000000000000: [0x23]  [0x69]  [0x6E]  [0x63]  [0x6C]  [0x75]  [0x64]  [0x65]
0x0000000000000008: [0x20]  [0x3C]  [0x73]  [0x74]  [0x64]  [0x69]  [0x6F]  [0x2E]
0x0000000000000010: [0x68]  [0x3E]  [0x0D]  [0x0A]  [0x23]  [0x69]  [0x6E]  [0x63]
0x0000000000000018: [0x6C]  [0x75]  [0x64]  [0x65]  [0x20]  [0x3C]  [0x73]  [0x74]
0x0000000000000020: [0x72]  [0x69]  [0x6E]  [0x67]  [0x2E]  [0x68]  [0x3E]  [0x0D]
0x0000000000000028: [0x0A]  [0x23]  [0x69]  [0x6E]  [0x63]  [0x6C]  [0x75]  [0x64]
0x0000000000000030: [0x65]  [0x20]  [0x3C]  [0x75]  [0x6E]  [0x69]  [0x73]  [0x74]
0x0000000000000038: [0x64]  [0x2E]  [0x68]  [0x3E]  [0x0D]  [0x0A]  [0x23]  [0x69]
0x0000000000000040: [0x6E]  [0x63]  [0x6C]  [0x75]  [0x64]  [0x65]  [0x20]  [0x3C]
0x0000000000000048: [0x73]  [0x74]  [0x64]  [0x6C]  [0x69]  [0x62]  [0x2E]  [0x68]
```
```
cat hexdump.c | ./hexdump -i -a | head
0x0000000000000000: [0x23][ # ]  [0x69][ i ]  [0x6E][ n ]  [0x63][ c ]  [0x6C][ l ]  [0x75][ u ]  [0x64][ d ]  [0x65][ e ]
0x0000000000000008: [0x20][SPC]  [0x3C][ < ]  [0x73][ s ]  [0x74][ t ]  [0x64][ d ]  [0x69][ i ]  [0x6F][ o ]  [0x2E][ . ]
0x0000000000000010: [0x68][ h ]  [0x3E][ > ]  [0x0D][ CR]  [0x0A][ LF]  [0x23][ # ]  [0x69][ i ]  [0x6E][ n ]  [0x63][ c ]
0x0000000000000018: [0x6C][ l ]  [0x75][ u ]  [0x64][ d ]  [0x65][ e ]  [0x20][SPC]  [0x3C][ < ]  [0x73][ s ]  [0x74][ t ]
0x0000000000000020: [0x72][ r ]  [0x69][ i ]  [0x6E][ n ]  [0x67][ g ]  [0x2E][ . ]  [0x68][ h ]  [0x3E][ > ]  [0x0D][ CR]
0x0000000000000028: [0x0A][ LF]  [0x23][ # ]  [0x69][ i ]  [0x6E][ n ]  [0x63][ c ]  [0x6C][ l ]  [0x75][ u ]  [0x64][ d ]
0x0000000000000030: [0x65][ e ]  [0x20][SPC]  [0x3C][ < ]  [0x75][ u ]  [0x6E][ n ]  [0x69][ i ]  [0x73][ s ]  [0x74][ t ]
0x0000000000000038: [0x64][ d ]  [0x2E][ . ]  [0x68][ h ]  [0x3E][ > ]  [0x0D][ CR]  [0x0A][ LF]  [0x23][ # ]  [0x69][ i ]
0x0000000000000040: [0x6E][ n ]  [0x63][ c ]  [0x6C][ l ]  [0x75][ u ]  [0x64][ d ]  [0x65][ e ]  [0x20][SPC]  [0x3C][ < ]
0x0000000000000048: [0x73][ s ]  [0x74][ t ]  [0x64][ d ]  [0x6C][ l ]  [0x69][ i ]  [0x62][ b ]  [0x2E][ . ]  [0x68][ h ]
```
```
cat hexdump.c | ./hexdump -i -x -o -b -l 4 | head
0x0000000000000000: [0o043][0b00100011]  [0o151][0b01101001]  [0o156][0b01101110]  [0o143][0b01100011]
0x0000000000000004: [0o154][0b01101100]  [0o165][0b01110101]  [0o144][0b01100100]  [0o145][0b01100101]
0x0000000000000008: [0o040][0b00100000]  [0o074][0b00111100]  [0o163][0b01110011]  [0o164][0b01110100]
0x000000000000000C: [0o144][0b01100100]  [0o151][0b01101001]  [0o157][0b01101111]  [0o056][0b00101110]
0x0000000000000010: [0o150][0b01101000]  [0o076][0b00111110]  [0o015][0b00001101]  [0o012][0b00001010]
0x0000000000000014: [0o043][0b00100011]  [0o151][0b01101001]  [0o156][0b01101110]  [0o143][0b01100011]
0x0000000000000018: [0o154][0b01101100]  [0o165][0b01110101]  [0o144][0b01100100]  [0o145][0b01100101]
0x000000000000001C: [0o040][0b00100000]  [0o074][0b00111100]  [0o163][0b01110011]  [0o164][0b01110100]
0x0000000000000020: [0o162][0b01110010]  [0o151][0b01101001]  [0o156][0b01101110]  [0o147][0b01100111]
0x0000000000000024: [0o056][0b00101110]  [0o150][0b01101000]  [0o076][0b00111110]  [0o015][0b00001101]
```
```
./hexdump -f hexdump.c -v -a -x | head -n 20
15:59:25 | ./hexdump.c:86 | main | DEBUG: Setting ASCII Flag
15:59:25 | ./hexdump.c:89 | main | DEBUG: ASCII Flag: true
15:59:25 | ./hexdump.c:79 | main | DEBUG: Setting HEXADECIMAL Flag
15:59:25 | ./hexdump.c:82 | main | DEBUG: HEXADECIMAL Flag: false
15:59:25 | ./hexdump.c:175 | main | DEBUG: Reading from FILE: hexdump.c
0x0000000000000000: [ # ]  [ i ]  [ n ]  [ c ]  [ l ]  [ u ]  [ d ]  [ e ]
0x0000000000000008: [SPC]  [ < ]  [ s ]  [ t ]  [ d ]  [ i ]  [ o ]  [ . ]
0x0000000000000010: [ h ]  [ > ]  [ CR]  [ LF]  [ # ]  [ i ]  [ n ]  [ c ]
0x0000000000000018: [ l ]  [ u ]  [ d ]  [ e ]  [SPC]  [ < ]  [ s ]  [ t ]
0x0000000000000020: [ r ]  [ i ]  [ n ]  [ g ]  [ . ]  [ h ]  [ > ]  [ CR]
0x0000000000000028: [ LF]  [ # ]  [ i ]  [ n ]  [ c ]  [ l ]  [ u ]  [ d ]
0x0000000000000030: [ e ]  [SPC]  [ < ]  [ u ]  [ n ]  [ i ]  [ s ]  [ t ]
0x0000000000000038: [ d ]  [ . ]  [ h ]  [ > ]  [ CR]  [ LF]  [ # ]  [ i ]
0x0000000000000040: [ n ]  [ c ]  [ l ]  [ u ]  [ d ]  [ e ]  [SPC]  [ < ]
0x0000000000000048: [ s ]  [ t ]  [ d ]  [ l ]  [ i ]  [ b ]  [ . ]  [ h ]
0x0000000000000050: [ > ]  [ CR]  [ LF]  [ # ]  [ i ]  [ n ]  [ c ]  [ l ]
0x0000000000000058: [ u ]  [ d ]  [ e ]  [SPC]  [ " ]  [ . ]  [ / ]  [ l ]
0x0000000000000060: [ i ]  [ b ]  [ E ]  [ p ]  [ y ]  [ o ]  [ n ]  [ / ]
0x0000000000000068: [ m ]  [ i ]  [ s ]  [ c ]  [ . ]  [ h ]  [ " ]  [SPC]
0x0000000000000070: [ / ]  [ / ]  [SPC]  [ g ]  [ i ]  [ t ]  [SPC]  [ c ]
0x0000000000000078: [ l ]  [ o ]  [ n ]  [ e ]  [SPC]  [ g ]  [ i ]  [ t ]
0x0000000000000080: [ @ ]  [ g ]  [ i ]  [ t ]  [ h ]  [ u ]  [ b ]  [ . ]
0x0000000000000088: [ c ]  [ o ]  [ m ]  [ : ]  [ e ]  [ p ]  [ y ]  [ o ]
0x0000000000000090: [ n ]  [ 0 ]  [ / ]  [ l ]  [ i ]  [ b ]  [ E ]  [ p ]
0x0000000000000098: [ y ]  [ o ]  [ n ]  [ . ]  [ g ]  [ i ]  [ t ]  [ CR]
```
## ipcalc
Calculate CIDR IP address
### Help:
``` bash
./ipcalc -h

Calculate CIDR IP Address

./ipcalc <A.B.C.D/XY>

[-h || --help]     Print this help message
[-d || --debug]    Dispaly debug output
[-v || --verbose]  List IP address of each host
<A.B.C.D/XY>       CIDR IP address
```
### Examples:
``` bash
./ipcalc 245.58.99.13/10

Address:   245.58.99.13
Netmask:   255.192.0.0
Wildcard:  0.63.255.255
Network:   245.0.0.0/10
Broadcast: 245.63.255.255
Host Min:  245.0.0.1
Host Max:  245.63.255.254
Hosts/Net: 4194302
Class:     E
```
``` bash
./ipcalc 230.55.255.72/29 -v

Address:   230.55.255.72       11100110.00110111.11111111.01001000
Netmask:   255.255.255.248     11111111.11111111.11111111.11111000
Wildcard:  0.0.0.7             00000000.00000000.00000000.00000111
Network:   230.55.255.72/29    11100110.00110111.11111111.01001000
Broadcast: 230.55.255.79       11100110.00110111.11111111.01001111
Host Min:  230.55.255.73       11100110.00110111.11111111.01001001
Host Max:  230.55.255.78       11100110.00110111.11111111.01001110
Hosts/Net: 6
Class:     D
Hosts:
  230.55.255.73
  230.55.255.74
  230.55.255.75
  230.55.255.76
  230.55.255.77
  230.55.255.78
```
``` bash
./ipcalc 192.168.55.99/12 -d

09:39:01 | ./ipcalc.c:73 | parseIP | DEBUG: Parsing IP string: 192.168.55.99/12
09:39:01 | ./ipcalc.c:103 | parseIP | DEBUG: Octet 1 end position: 3
09:39:01 | ./ipcalc.c:106 | parseIP | DEBUG: Octet 2 end position: 7
09:39:01 | ./ipcalc.c:109 | parseIP | DEBUG: Octet 3 end position: 10
09:39:01 | ./ipcalc.c:112 | parseIP | DEBUG: Octet 4 end position: 13
09:39:01 | ./ipcalc.c:118 | parseIP | DEBUG: Octet 1: 192
09:39:01 | ./ipcalc.c:126 | parseIP | DEBUG: Octet 2: 168
09:39:01 | ./ipcalc.c:134 | parseIP | DEBUG: Octet 3: 55
09:39:01 | ./ipcalc.c:142 | parseIP | DEBUG: Octet 4: 99
09:39:01 | ./ipcalc.c:150 | parseIP | DEBUG: Network Bit: 12
09:39:01 | ./ipcalc.c:184 | parseIP | DEBUG: Given IP: 192.168.55.99
09:39:01 | ./ipcalc.c:195 | parseIP | DEBUG: Subnet mask: 255.240.0.0
09:39:01 | ./ipcalc.c:200 | parseIP | DEBUG: Network ID: 192.160.0.0
09:39:01 | ./ipcalc.c:205 | parseIP | DEBUG: Broadcast: 192.175.255.255
Address:   192.168.55.99
Netmask:   255.240.0.0
Wildcard:  0.15.255.255
Network:   192.160.0.0/12
Broadcast: 192.175.255.255
Host Min:  192.160.0.1
Host Max:  192.175.255.254
Hosts/Net: 1048574
Class:     C
```
``` bash
./ipcalc 128.55.66.77/30 127.0.0.1/29 -v

Address:   128.55.66.77        10000000.00110111.01000010.01001101
Netmask:   255.255.255.252     11111111.11111111.11111111.11111100
Wildcard:  0.0.0.3             00000000.00000000.00000000.00000011
Network:   128.55.66.76/30     10000000.00110111.01000010.01001100
Broadcast: 128.55.66.79        10000000.00110111.01000010.01001111
Host Min:  128.55.66.77        10000000.00110111.01000010.01001101
Host Max:  128.55.66.78        10000000.00110111.01000010.01001110
Hosts/Net: 2
Class:     B
Hosts:
  128.55.66.77
  128.55.66.78

Address:   127.0.0.1           01111111.00000000.00000000.00000001
Netmask:   255.255.255.248     11111111.11111111.11111111.11111000
Wildcard:  0.0.0.7             00000000.00000000.00000000.00000111
Network:   127.0.0.0/29        01111111.00000000.00000000.00000000
Broadcast: 127.0.0.7           01111111.00000000.00000000.00000111
Host Min:  127.0.0.1           01111111.00000000.00000000.00000001
Host Max:  127.0.0.6           01111111.00000000.00000000.00000110
Hosts/Net: 6
Class:     A
Hosts:
  127.0.0.1
  127.0.0.2
  127.0.0.3
  127.0.0.4
  127.0.0.5
  127.0.0.6
```
``` bash
./ipcalc 127.2.3.4/5 190.5.4.9/10 222.255.255.255/30

Address:   127.2.3.4
Netmask:   248.0.0.0
Wildcard:  7.255.255.255
Network:   120.0.0.0/5
Broadcast: 127.255.255.255
Host Min:  120.0.0.1
Host Max:  127.255.255.254
Hosts/Net: 134217726
Class:     A

Address:   190.5.4.9
Netmask:   255.192.0.0
Wildcard:  0.63.255.255
Network:   190.0.0.0/10
Broadcast: 190.63.255.255
Host Min:  190.0.0.1
Host Max:  190.63.255.254
Hosts/Net: 4194302
Class:     B

Address:   222.255.255.255
Netmask:   255.255.255.252
Wildcard:  0.0.0.3
Network:   222.255.255.252/30
Broadcast: 222.255.255.255
Host Min:  222.255.255.253
Host Max:  222.255.255.254
Hosts/Net: 2
Class:     C
```