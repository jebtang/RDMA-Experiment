TARGET ="server"
TARGET_2="client"
all:
	cc -g -o ${TARGET} ${TARGET}.c -lrdmacm -libverbs
	$ cc -o ${TARGET_2} ${TARGET_2}.c -lrdmacm -libverbs

.PHONY: clean
clean:
	rm -f *.o ${TARGET} ${TARGET_2}
