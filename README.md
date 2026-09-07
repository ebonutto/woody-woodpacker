# woody_woodpacker
This project is about coding a simple packer!

https://www.obfuscators.io/


Project structure:


nasm -f bin -o stub.bin stub.asm
xxd -i stub.bin > stub.h


## References

### Packers

- YES


### Encryption

- RC4




`hello`


nasm -f bin stub/hello.S -o stub.bin &&
python3 -c "
data = open('stub.bin', 'rb').read()
print('unsigned char stub_bin[] = {')
print(', '.join(f'0x{b:02x}' for b in data))
print('};')
print(f'unsigned int stub_bin_len = {len(data)};')
"