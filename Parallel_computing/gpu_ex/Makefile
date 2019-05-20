CC = nvcc


all: transpose 64_shared_transpose 512_shared_transpose 1024_shared_transpose



transpose : transpose.cu
	$(CC) $^ -o $@
	

64_shared_transpose : 64_shared_transpose.cu 
	$(CC) $^ -o $@
	
512_shared_transpose : 512_shared_transpose.cu
	$(CC) $^ -o $@
	

1024_shared_transpose : shared_transpose.cu
	$(CC) $^ -o $@
	
	
clean:
	@rm -r *.x

.PHONY: clean all
