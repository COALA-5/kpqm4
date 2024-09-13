# Benchmarks
## Key Encapsulation Schemes

|  scheme   | implementation | Keygen [cycles] | Encapsulation [cycles] | Decapsulation [cycles] | 
|:---------:|:--------------:|:---------------:|:----------------------:|:----------------------:|
| SMAUG-T1  |       ref      |      949230     |        1213910         |        1622068         |
| SMAUG-T3  |       ref      |     1950635     |        2005858         |        2473625         |
| SMAUG-T5  |       ref      |     3119509     |        3389406         |        3866477         |
| NTRU+576  |       ref      |      745010     |         388953         |         534554         |
| NTRU+768  |       ref      |      846023     |         540297         |         744154         |
| NTRU+864  |       ref      |      903049     |         591688         |         832962         |
| NTRU+1152 |       ref      |     2161362     |         800569         |        1169875         |
| PALOMA128 |   ref(flash)   |        -        |        3087378         |        3811089         |

## Digital Signature Schemes

|  scheme   | implementation | Keygen [cycles] |      Sign [cycles]     |     Verify [cycles]    | 
|:---------:|:--------------:|:---------------:|:----------------------:|:----------------------:|
|   AIMer-l1     |       ref      |     396445      |        33421937        |        32114337        |
|   AIMer-l1     |     memopt     |     564286      |        47434210        |        27496766        |
|   AIMer-l3     |       ref      |     986245      |        92869260        |        89976575        |
|   AIMer-l3     |     memopt     |     1393101     |        119848593       |        78058051        |
|   AIMer-l5     |     memopt     |     3335884     |        234960280       |        171489263       |
|    HAETAE2     |       ref      |     9893255     |        41819644        |        1110854         |
|    HAETAE2     |       m4f      |     1549086     |        4829742         |        976956          |
|    HAETAE3     |       ref      |     26932221    |        33046515        |        2005813         |
|    HAETAE3     |       m4f      |     4559001     |        7607057         |        1867887         |
|    HAETAE5     |       ref      |     20788191    |        69222865        |        2486376         |
|    HAETAE5     |       m4f      |     7437044     |        9492716         |        2486326         |
|   NCC-Sign2    |       ref      |     1448306     |        5257637         |        2359179         |
|   NCC-Sign2    |       m4f      |     1002893     |        2644864         |        1259925         |
|   NCC-Sign3    |       ref      |     1893599     |        9245980         |        2965231         |
|   NCC-Sign3    |       m4f      |     1305061     |        4356339         |        1585031         |
|   NCC-Sign5    |       ref      |     2897092     |        12046625        |        4731909         |
|   NCC-Sign5    |       m4f      |     1965693     |        5821410         |        2447042         |
| MQ-Sign-LR-128 |   ref(flash)   |        -        |       54269062         |        81148106        |


# Memory Usage
## Key Encapsulation Schemes

|  scheme   | implementation | Keygen [bytes]  | Encapsulation [bytes]  | Decapsulation [bytes]  | 
|:---------:|:--------------:|:---------------:|:----------------------:|:----------------------:|
| SMAUG-T1  |       ref      |      5176       |          6848          |          7616          |
| SMAUG-T3  |       ref      |      10452      |          12652         |          12652         |
| SMAUG-T5  |       ref      |      25508      |          28740         |          30308         |
| NTRU+576  |       ref      |      7340       |          7360          |          13560         |
| NTRU+768  |       ref      |      9644       |          9544          |          17808         |
| NTRU+864  |       ref      |      10796      |          10640         |          19936         |
| NTRU+1152 |       ref      |      14252      |          13912         |          26304         |

## Digital Signature Schemes

|  scheme   | implementation | Keygen [bytes]  |      Sign [bytes]      |     Verify [bytes]     | 
|:---------:|:--------------:|:---------------:|:----------------------:|:----------------------:|
| AIMer-l1  |       ref      |      10952      |         187756         |         196712         |
| AIMer-l1  |     memopt     |      875 2      |         27324          |         34648          |
| AIMer-l3  |       ref      |      23864      |         413972         |         435632         |
| AIMer-l3  |     memopt     |      19128      |         29968          |         31592          |
| AIMer-l5  |     memopt     |      49976      |         77172          |         78576          |
|  HAETAE2  |       ref      |      26092      |         54428          |         29696          |
|  HAETAE2  |       m4f      |      19772      |         55576          |         23296          |
|  HAETAE3  |       ref      |      43604      |         80420          |         48468          |
|  HAETAE3  |       m4f      |      29484      |         83436          |         31776          |
|  HAETAE5  |       ref      |      54756      |         100260         |         62024          |
|  HAETAE5  |       m4f      |      34196      |         103988         |         37184          |
| NCC-Sign2 |       ref      |      29024      |         49768          |         34584          |
| NCC-Sign2 |       m4f      |      31712      |         50464          |         37272          |
| NCC-Sign3 |       ref      |      38240      |         65944          |         45644          |
| NCC-Sign3 |       m4f      |      42104      |         67000          |         49548          |
| NCC-Sign5 |       ref      |      56672      |         98948          |         67264          |
| NCC-Sign5 |       m4f      |      62888      |         100072         |         73480          |

# Hash Evaluation
## Key Encapsulation Schemes

|  scheme   | implementation | Keygen [%] | Encapsulation [%] | Decapsulation [%] | 
|:---------:|:--------------:|:----------:|:-----------------:|:-----------------:|
| SMAUG-T1  |       ref      |    21.97   |       17.16       |       16.93       |
| SMAUG-T3  |       ref      |    20.79   |       19.53       |       19.80       |
| SMAUG-T5  |       ref      |    25.21   |       24.09       |       24.53       |
| NTRU+576  |       ref      |    13.43   |       35.41       |       18.66       |
| NTRU+768  |       ref      |    12.44   |       33.74       |       17.59       |
| NTRU+864  |       ref      |    13.13   |       33.97       |       17.32       |
| NTRU+1152 |       ref      |    10.19   |       31.79       |       15.30       |

## Digital Signature Schemes

|  scheme   | implementation | Keygen [%] |  Sign [%] |  Verify [%]  | 
|:---------:|:--------------:|:----------:|:---------:|:------------:|
| AIMer-l1  |       ref      |    71.37   |   45.72   |     45.21    |
| AIMer-l3  |       ref      |    76.97   |   40.88   |     40.79    |
| AIMer-l5  |     memopt     |    58.99   |   40.38   |     26.88    |
|  HAETAE2  |       ref      |    27.62   |   56.05   |     50.91    |
|  HAETAE3  |       ref      |    12.94   |   56.25   |     53.81    |
|  HAETAE5  |       ref      |    34.22   |   56.00   |     53.85    |
| NCC-Sign2 |       ref      |    45.68   |   20.20   |     28.47    |
| NCC-Sign3 |       ref      |    47.28   |   18.24   |     30.47    |
| NCC-Sign5 |       ref      |    44.98   |   19.48   |     27.20    |