# kpqm4
Collection of Korean post-quantum cryptographic alrogithms for the ARM Cortex-M4

## Introduction
This project is inpired by pqm4 project.
- 포팅한 pqm4의 버전을 명시해야할듯
- pk, sk, ct, sig의 size를 각 알고리즘별로 명시
- 각각의 성능을 측정해서 배정

## Benchmark - PKE/KEM
| Algorithm | Security Level | Keygen | Encap | Decap |
|:---------:|:--------------:|:------:|:-----:|:-----:|
|  SMAUG-T  |        1       |        |       |       |
|  SMAUG-T  |        3       |        |       |       |
|  SMAUG-T  |        5       |        |       |       |

## Benchmark - DSA
| Algorithm | Security Level | Keygen | Sign  | Verify |
|:---------:|:--------------:|:------:|:-----:|:------:|
|           |        1       |        |       |        |
|           |        3       |        |       |        |
|           |        5       |        |       |        |
