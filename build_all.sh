make clean case=swint; make all case=swint
make clean case=plic; make all case=plic
make clean case=uart; make all case=uart
sshpass -p iRJk1SVCSg scp -r -p build/*/riscv-*.bin.text  pingping.wu@10.11.23.152:~/work/temp/
sshpass -p iRJk1SVCSg scp -r -p build/*/riscv-*.s  pingping.wu@10.11.23.152:~/work/temp/
