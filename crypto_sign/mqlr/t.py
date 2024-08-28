# with open("/home/unlimit/Documents/ARM/kpqm4/crypto_sign/mqlr/m4/sk.txt", "r") as f:
#     line = f.readline()
#     l = []
#     for i in range(0, len(line), 2):
#         l.append(line[i:i+2])

# # with open("/home/unlimit/Documents/ARM/kpqm4/crypto_sign/mqlr/m4/sk_formatted.txt", "w") as f:
# #     for i in range(0, len(l), 4):
# #         f.write(f"\t.byte 0x{l[i+3]}, 0x{l[i+2]}, 0x{l[i+1]}, 0x{l[i+0]}\n")
        
        
# with open("/home/unlimit/Documents/ARM/kpqm4/crypto_sign/mqlr/m4/sk_cdat.txt", "w") as f:
#     f.write("{ ")
#     for i in range(0, len(l) - 1):
#         f.write(f"0x{l[i]},")
#         if i % 128 == 0 and i != 0:
#             f.write("\n")
#     f.write("}")
        
        
# ###### PK
# with open("/home/unlimit/Documents/ARM/kpqm4/crypto_sign/mqlr/m4/pk.txt", "r") as f:
#     line = f.readline()
#     l = []
#     for i in range(0, len(line), 2):
#         l.append(line[i:i+2])

# # with open("/home/unlimit/Documents/ARM/kpqm4/crypto_sign/mqlr/m4/pk_formatted.txt", "w") as f:
# #     for i in range(0, len(l), 4):
# #         f.write(f"\t.byte 0x{l[i]}, 0x{l[i+1]}, 0x{l[i+2]}, 0x{l[i+3]}\n")

# with open("/home/unlimit/Documents/ARM/kpqm4/crypto_sign/mqlr/m4/pk_cdat.txt", "w") as f:
#     f.write("{ ")
#     for i in range(0, len(l) - 1):
#         f.write(f"0x{l[i]},")
#         if i % 128 == 0 and i != 0:
#             f.write("\n")
#     f.write("}")

print(hex(0x1ff-82))