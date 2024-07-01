# kpqm4
Collection of Korean post-quantum cryptographic alrogithms for the ARM Cortex-M4

## Introduction
This project is inpired by pqm4 project.
KPQM4 provides executable KpqC Competition Round 2 candidates code on Cortex-M4.
We used NUCLEO-l4r5zi which has 640KB SRAM and 2MB flash memory.

## Dependency
- [pqm4 commit version 006a109](https://github.com/mupq/pqm4/tree/006a1098d2a4a117f10a026c711658c39b102fdd)
It is totally copied into our project, so you do not need to download it.

## How to build
All of build steps work well on Ubuntu 22.04LTS with NUCLEO-l4r5zi board.

Install dependencies.
```bash
sudo apt install gcc-arm-none-eabi build-essential libusb-1.0-0-dev cmake make python3-pip
```

Install stlink.
```bash
git clone https://github.com/stlink-org/stlink .
cmake .
make
# If you fail here, install additional dependencies according to error message.
sudo cp bin/* /usr/local/bin
sudo cp lib/*.so* /lib32
sudo cp config/udev/rules.d/49-stlinkv* /etc/udev/rules.d/
```

Now connect your board with your PC, and check it is connected well.
```bash
lsusb
# Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
# Bus 001 Device 003: ID 0483:374b STMicroelectronics ST-LINK/V2.1
# Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
st-info --probe
# Found 1 stlink programmers
#   version:    V2J35S26
#   serial:     0670FF363947423043132622
#   flash:      2097152 (pagesize: 4096)
#   sram:       655360
#   chipid:     0x470
#   dev-type:   STM32L4Rx
```

Clone the repository into your workspace.
```bash
git clone https://github.com/COALA-5/kpqm4
```

Move to kpqm4 workspace, and build it.
```bash
make -j16 PLATFORM=nucleo-l4r5zi
# If you do not use -j option, it will take lots of times.
```



## Future Works
- [ ] Make a table contains each size of pk, sk, ct/sig
- [X] Write builing steps
- [ ] ... and more

## Benchmark - PKE/KEM
| Algorithm | Security Level |  Keygen |  Encap  |  Decap  |
|:---------:|:--------------:|:-------:|:-------:|:-------:|
|  SMAUG-T  |        1       |  950119 | 1215695 | 1622068 |
|  SMAUG-T  |        3       | 1950635 | 2005858 | 2473625 |
|  SMAUG-T  |        5       | 3119509 | 3389406 | 3866477 |
|   NTRU+   |       576      |  594518 |  388953 |  534554 |
|   NTRU+   |       768      |  846023 |  540297 |  744154 |
|   NTRU+   |       864      |  903049 |  591688 |  832962 |
|   NTRU+   |      1152      | 2265553 |  800569 | 1169875 |

## Benchmark - DSA
| Algorithm | Security Level | Keygen |  Sign   |  Verify  |
|:---------:|:--------------:|:------:|:-------:|:--------:|
|  AIMer-p1 |        1       | 396445 |33421937 | 32114337 |
|  AIMer-p1 |        3       | 986245 |92869260 | 89976575 |
|  AIMer-p1 |        5       |3335884 |234960280|171489263 |
|  HAETAE   |        2       |5523490 |31924174 | 1110693  |
|  HAETAE   |        3       |9178639 |17506204 | 2006013  |
|  HAETAE   |        5       |7753629 |59688464 | 2486222  |
|  NCC-Sign |        2       |1448306 | 3795759 | 2359179  |
|  NCC-Sign |        3       |1893599 | 4716491 | 2965231  |
|  NCC-Sign |        5       |2897092 | 7675877 | 4731909  |

## Contributors
* Seog Chung Seo, Professor (scseo@kookmin.ac.kr)
* YoungBeom Kim, Ph.D. Student (darania@kookmin.ac.kr)
* YongRyeol Choi, Master's course Student (mike0726@kookmin.ac.kr)

## Other KpqC Project
- [KpqBench](https://github.com/kpqclib/kpqclib)