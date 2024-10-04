#/bin/bash


echo "Enter a filename without .bin:"
#filepath=$(python3 integrity_test/get_filename.py)


filepath=$(python3 integrity_test/gen_hash.py)

echo "Filepath: ${filepath}"


xxd -r -p integrity_test/hmac_${filepath}.txt integrity_test/hmac_${filepath}.bin

# rm ${filepath}.txt

STM32_Programmer_CLI -c port=SWD -w integrity_test/hmac_${filepath}.bin 0x081FF000  -v 