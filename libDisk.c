#include "libDisk.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(){
	int binfile = openDisk("numbers", 0);
	printf("numbers fd: %d\n", binfile);

	char *block = malloc(257);
	int status = readBlock(binfile, 0, block);
	block[256] = '\0';

	int index;
	//for (index = 0; index < 256; index++){
	//	printf("%x ", *(block + index));
	//}
	printf("%s\n", block);

	writeBlock(binfile, 1, block);
	closeDisk(binfile);
	

	return 0;
}

int openDisk(char *filename, int nBytes){
	int diskNum = -1;

	if (nBytes > 0){
		diskNum = open(filename, O_RDWR | O_CREAT);
	}

	else if (nBytes == 0){
		diskNum = open(filename, O_RDWR);
	}

	return diskNum;
}


int readBlock(int disk, int bNum, void *block){
	int seek_error = -1;
	int read_error = -1;

	seek_error = lseek(disk, bNum * BLOCKSIZE, SEEK_SET);

	if (seek_error >= 0){
		
		read_error = read(disk, block, BLOCKSIZE);
		if (read_error > 1){
			return 0;
		}
		else {
			return read_error;
		}

	}
	return seek_error;
}

int writeBlock(int disk, int bNum, void *block){
	//more error checking needed here?
	//worry about nBytes size?...

	int seekRetVal = lseek(disk, bNum * BLOCKSIZE, SEEK_SET);
	int writeRetVal = -1;
	if (seekRetVal == EBADF) {
		return -1;
		//-1 means the file is not open
	}
	else if (seekRetVal == EOVERFLOW) {
		return -2;
		//-2 means the index is out of bounds?
	}
	else {
		//lseek was successful. Write buffer
		writeRetVal = write(disk, block, BLOCKSIZE);
		return writeRetVal;
		//return 0 on success. Still need to define all errors
	}
}


void closeDisk(int disk){
	close(disk);
}