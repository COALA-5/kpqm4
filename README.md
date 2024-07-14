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
- [X] Add hashing evaulations
- [ ] Add max/min/median clock cycles (only shows average clock cycles for now)
- [ ] ... and more



## Contributors
* Seog Chung Seo, Professor (scseo@kookmin.ac.kr)
* YoungBeom Kim, Ph.D. Student (darania@kookmin.ac.kr)
* YongRyeol Choi, Master's course Student (mike0726@kookmin.ac.kr)

## Other KpqC Project
- [KpqBench](https://github.com/kpqclib/kpqclib)