# Important Notification
- `(flash)` means the public/secret key are flashed at flash memory.
    - In this case, keypair function is not executed.

# Benchmarks
## Key Encapsulation Schemes

|  scheme   | implementation | Keygen [cycles] | Encapsulation [cycles] | Decapsulation [cycles] | 
|:---------:|:--------------:|:---------------:|:----------------------:|:----------------------:|
| SMAUG-T1  |       ref      |     1162495     |        1419774         |        1915454         |
| SMAUG-T3  |       ref      |     2502313     |        2880288         |        3601700         |
| SMAUG-T5  |       ref      |     4263151     |        4761380         |        5722930         |
| NTRU+576  |       ref      |      584967     |         394654         |         512035         |
| NTRU+768  |       ref      |      665229     |         567533         |         733604         |
| NTRU+864  |       ref      |      771287     |         632646         |         850395         |
| NTRU+1152 |       ref      |     1146222     |         843084         |        1111993         |
| PALOMA128 |   ref(flash)   |        -        |         671221         |        21260147        |

## Digital Signature Schemes

|  scheme   | implementation | Keygen [cycles] |      Sign [cycles]     |     Verify [cycles]    | 
|:---------:|:--------------:|:---------------:|:----------------------:|:----------------------:|
|   AIMer-l1     |       ref      |     396445      |        33421937        |        32114337        |
|   AIMer-l1     |     memopt     |     564286      |        47434210        |        27496766        |
|   AIMer-l3     |       ref      |     986245      |        92869260        |        89976575        |
|   AIMer-l3     |     memopt     |     1393101     |        119848593       |        78058051        |
|   AIMer-l5     |     memopt     |     3335884     |        234960280       |        171489263       |
|    HAETAE2     |       ref      |     10883934    |        44973328        |        1100887         |
|    HAETAE2     |       m4f      |     1549086     |        4829742         |        976956          |
|    HAETAE3     |       ref      |     18015211    |        35719119        |        1994983         |
|    HAETAE3     |       m4f      |     4559001     |        7607057         |        1867887         |
|    HAETAE5     |       ref      |     22322692    |        45664190        |        2465736         | 
|    HAETAE5     |       m4f      |     7437044     |        9492716         |        2486326         |
|   NCC-Sign2    |       ref      |     1448306     |        5257637         |        2359179         |
|   NCC-Sign2    |       m4f      |     1002893     |        2644864         |        1259925         |
|   NCC-Sign3    |       ref      |     1893599     |        9245980         |        2965231         |
|   NCC-Sign3    |       m4f      |     1305061     |        4356339         |        1585031         |
|   NCC-Sign5    |       ref      |     2897092     |        12046625        |        4731909         |
|   NCC-Sign5    |       m4f      |     1965693     |        5821410         |        2447042         |
| MQ-Sign-LR-128 |   ref(flash)   |        -        |        9815101         |        14340523        |



# Memory Usage
## Key Encapsulation Schemes

|  scheme   | implementation | Keygen [bytes]  | Encapsulation [bytes]  | Decapsulation [bytes]  | 
|:---------:|:--------------:|:---------------:|:----------------------:|:----------------------:|
| SMAUG-T1  |       ref      |      10940      |          12588         |          13356         |
| SMAUG-T3  |       ref      |      15028      |          17212         |          18412         |
| SMAUG-T5  |       ref      |      20148      |          22844         |          24316         |
| NTRU+576  |       ref      |      7188       |          7036          |          12016         |
| NTRU+768  |       ref      |      9444       |          9220          |          15880         |
| NTRU+864  |       ref      |      10572      |          10316         |          17816         |
| NTRU+1152 |       ref      |      13956      |          13588         |          23608         |
| PALOMA128 |   ref(flash)   |        -        |         15700          |          17308         |

## Digital Signature Schemes

|  scheme   | implementation | Keygen [bytes]  |      Sign [bytes]      |     Verify [bytes]     | 
|:---------:|:--------------:|:---------------:|:----------------------:|:----------------------:|
| AIMer-l1  |       ref      |      10952      |         187756         |         196712         |
| AIMer-l1  |     memopt     |      875 2      |         27324          |         34648          |
| AIMer-l3  |       ref      |      23864      |         413972         |         435632         |
| AIMer-l3  |     memopt     |      19128      |         29968          |         31592          |
| AIMer-l5  |     memopt     |      49976      |         77172          |         78576          |
|  HAETAE2  |       ref      |      24036      |         83068          |         29696          |
|  HAETAE2  |       m4f      |      19772      |         55576          |         23296          |
|  HAETAE3  |       ref      |      41444      |         126524         |         48424          |
|  HAETAE3  |       m4f      |      29484      |         83436          |         31776          |
|  HAETAE5  |       ref      |      52812      |         159652         |         62024          |
|  HAETAE5  |       m4f      |      34196      |         103988         |         37184          |
| NCC-Sign2 |       ref      |      29024      |         49768          |         34584          |
| NCC-Sign2 |       m4f      |      31712      |         50464          |         37272          |
| NCC-Sign3 |       ref      |      38240      |         65944          |         45644          |
| NCC-Sign3 |       m4f      |      42104      |         67000          |         49548          |
| NCC-Sign5 |       ref      |      56672      |         98948          |         67264          |
| NCC-Sign5 |       m4f      |      62888      |         100072         |         73480          |
| MQ-Sign-LR-128 | ref(flash)|        -        |         27364          |          808           |


# Hash Evaluation
## Key Encapsulation Schemes

|  scheme   | implementation | Keygen [%] | Encapsulation [%] | Decapsulation [%] | 
|:---------:|:--------------:|:----------:|:-----------------:|:-----------------:|
| SMAUG-T1  |       ref      |    23.97   |       12.60       |       12.80       |
| SMAUG-T3  |       ref      |    22.81   |       15.12       |       14.82       |
| SMAUG-T5  |       ref      |    20.82   |       14.67       |       14.49       |
| NTRU+576  |       ref      |    17.00   |       37.23       |       21.26       |
| NTRU+768  |       ref      |    14.27   |       32.22       |       17.90       |
| NTRU+864  |       ref      |    13.04   |       33.10       |       17.94       |
| NTRU+1152 |       ref      |    13.45   |       33.43       |       18.54       |

## Digital Signature Schemes

|  scheme   | implementation | Keygen [%] |  Sign [%] |  Verify [%]  | 
|:---------:|:--------------:|:----------:|:---------:|:------------:|
| AIMer-l1  |       ref      |    71.37   |   45.72   |     45.21    |
| AIMer-l3  |       ref      |    76.97   |   40.88   |     40.79    |
| AIMer-l5  |     memopt     |    58.99   |   40.38   |     26.88    |
|  HAETAE2  |       ref      |     9.31   |   42.46   |     44.31    |
|  HAETAE3  |       ref      |    14.23   |   42.36   |     49.87    |
|  HAETAE5  |       ref      |    14.43   |   42.43   |     53.37    |
| NCC-Sign2 |       ref      |    45.68   |   20.20   |     28.47    |
| NCC-Sign3 |       ref      |    47.28   |   18.24   |     30.47    |
| NCC-Sign5 |       ref      |    44.98   |   19.48   |     27.20    |
| MQ-Sign-LR-128 | ref(flash)|      -     |    1.47   |      0.08    |


# TODO - will be updated soon (benchmark result at STM32429I-EVAL1)
| MQ-Sign-LR-128*|   ref(flash)   |        -        |       54269062         |        81148106        |
| PALOMA128*|   ref(flash)   |        -        |        3087378         |        3811089         |
