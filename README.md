# kpqm4
Collection of Korean post-quantum cryptographic alrogithms for the ARM Cortex-M4

## Introduction
This project is inpired by pqm4 project.
- 포팅한 pqm4의 버전을 명시해야할듯
- pk, sk, ct, sig의 size를 각 알고리즘별로 명시
- 각각의 성능을 측정해서 배정
- 개발자 명시 (최용렬, 김영범, 서석충), kpqclean도

## Introduction
This project is inpired by pqm4 project.

## Benchmark - PKE/KEM
| Algorithm | Security Level |  Keygen |  Encap  |  Decap  |
|:---------:|:--------------:|:-------:|:-------:|:-------:|
|  SMAUG-T  |        1       |  950119 | 1215695 | 1622068 |
|  SMAUG-T  |        3       | 1950635 |2005858|2473625|
|  SMAUG-T  |        5       |3119509|3389406|3866477|
|   NTRU+   |       576      |  594518 |  388953 |  534554 |
|   NTRU+   |       768      |  846023 |  540297 |  744154 |
|   NTRU+   |       864      |  903049 |  591688 |  832962 |
|   NTRU+   |      1152      | 2265553 |  800569 | 1169875 |

## Benchmark - DSA
| Algorithm | Security Level | Keygen | Sign  | Verify |
|:---------:|:--------------:|:------:|:-----:|:------:|
|  AIMer-p1 |        1       |396445 |33421937|32114337|
|  AIMer-p1 |        3       |986245|92869260|89976575|
|  AIMer-p1 |        5       |        |       |        |
|  AIMer-p2 |        1       |396443 |81909894|80848999|
|  AIMer-p2 |        3       |        |       |        |
|  AIMer-p2 |        5       |        |       |        |
|  AIMer-p3 |        1       |396440 |33421937|32114337|
|  AIMer-p3 |        3       |        |       |        |
|  AIMer-p3 |        5       |        |       |        |
|  HAETAE   |        2       |5523490 |31924174|1110693|
|  HAETAE   |        3       |9178639|17506204|2006013|
|  HAETAE   |        5       |7753629|59688464|2486222|
|  NCC-Sign |        2       |1448306|3795759|2359179|
|  NCC-Sign |        3       |1893599|4716491|2965231|
|  NCC-Sign |        5       |2897092|7675877|4731909|

## Contributors
* Seog Chung Seo, Professor (scseo@kookmin.ac.kr)
* YoungBeom Kim, Ph.D. Student (darania@kookmin.ac.kr)
* YongRyeol Choi, Master's course Student (mike0726@kookmin.ac.kr)
