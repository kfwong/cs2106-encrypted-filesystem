#include "libefs.h"

// FS Descriptor
TFileSystemStruct *_fs;

// Open File Table
TOpenFile *_oft;

// File pointer, because fuck structs
FILE *_fp;

// Open file table counter
int _oftCount=0;

// Mounts a partition given in fsPartitionName. Must be called before all
// other functions
void initFS(const char *fsPartitionName, const char *fsPassword)
{
	mountFS(fsPartitionName, fsPassword);
	_fs = getFSInfo();
}

// Opens a file in the partition. Depending on mode, a new file may be created
// if it doesn't exist, or we may get FS_FILE_NOT_FOUND in _result. See the enum above for valid modes.
// Return -1 if file open fails for some reason. E.g. file not found when mode is MODE_NORMAL, or
// disk is full when mode is MODE_CREATE, etc.

int openFile(const char *filename, unsigned char mode)
{	
	//_oft = (TOpenFile *)malloc(sizeof(TOpenFile));
	_oft = new TOpenFile();
	_oft->openMode = mode;
	_oft->blockSize = _fs->blockSize;
	_oft->inodeBuffer = makeInodeBuffer();
	_oft->buffer = makeDataBuffer();
	_oft->readPtr = *_oft->buffer;
	_oft->writePtr = *_oft->buffer;

	unsigned int fileNdx = findFile(filename); 

	unsigned int len = getFileLength(filename);
	
	unsigned long blockNum;
	
	unsigned long byteIndex;
	
	unsigned long freeBlock;
	
	unsigned int dirNdx;

	switch (mode)
	{
		case MODE_NORMAL:
			if(fileNdx == FS_FILE_NOT_FOUND){
				printf("Cannot find encrypted file %s\n", filename);
				exit(-1);
			}
			_oft->inode = getInodeForFile(filename);
			loadInode(_oft->inodeBuffer, _oft->inode);
			blockNum = _oft->inodeBuffer[0];
			
			// calculate byte offset
			byteIndex = _fs->dataByteIndex + (blockNum-1) * _fs->blockSize;
			fseek(_fp, byteIndex, SEEK_SET);
			
			//_oft->filePtr = *_fp;

			break;
		case MODE_CREATE:
		    if(fileNdx == FS_FILE_NOT_FOUND){
				// create new file
				
				// Write the directory entry
				dirNdx = makeDirectoryEntry(filename, 0x0, 0);
				
				// Find a free block
				freeBlock = findFreeBlock();
				
				// Mark the free block now as busy
				markBlockBusy(freeBlock);
				
			}
			_oft->inode = getInodeForFile(filename);
			loadInode(_oft->inodeBuffer, _oft->inode);
			blockNum = _oft->inodeBuffer[0];
			
			byteIndex = _fs->dataByteIndex + (blockNum-1) * _fs->blockSize;
			fseek(_fp, byteIndex, SEEK_SET);
			
			//_oft->filePtr = _fp;
			break;
		case MODE_READ_ONLY:
			if(fileNdx == FS_FILE_NOT_FOUND){
				printf("Cannot find encrypted file %s\n", filename);
				exit(-1);
			}
			_oft->inode = getInodeForFile(filename);
			loadInode(_oft->inodeBuffer, _oft->inode);
			blockNum = _oft->inodeBuffer[0];
			printf("%lu", blockNum);
			
			byteIndex = _fs->dataByteIndex + (blockNum-1) * _fs->blockSize;
			fseek(_fp, byteIndex, SEEK_SET);
			
			//_oft->filePtr = _fp;
			break;
		case MODE_READ_APPEND:
			if(fileNdx == FS_FILE_NOT_FOUND){
				printf("Cannot find encrypted file %s\n", filename);
				exit(-1);
			}
			_oft->inode = getInodeForFile(filename);
			loadInode(_oft->inodeBuffer, _oft->inode);
			blockNum = _oft->inodeBuffer[0];
			
			byteIndex = _fs->dataByteIndex + (blockNum-1) * _fs->blockSize;
			//filePtr starts from back of file
			fseek(_fp, byteIndex, SEEK_END);
			
			//_oft->filePtr = _fp;
			break;
		default:
			return -1;
	}
}

// Write data to the file. File must be opened in MODE_NORMAL or MODE_CREATE modes. Does nothing
// if file is opened in MODE_READ_ONLY mode.
void writeFile(int fp, void *buffer, unsigned int dataSize, unsigned int dataCount)
{
}

// Flush the file data to the disk. Writes all data buffers, updates directory,
// free list and inode for this file.
void flushFile(int fp)
{
}

// Read data from the file.
void readFile(int fp, void *buffer, unsigned int dataSize, unsigned int dataCount)
{
	/*
	readBlock(_oft->buffer, blockNum);

	unmountFS();

	fwrite(_oft->buffer, sizeof(char), len, fp);

	fclose(fp);

	free(_oft->inodeBuffer);
	free(_oft->buffer);
	*/
}

// Delete the file. Read-only flag (bit 2 of the attr field) in directory listing must not be set. 
// See TDirectory structure.
void delFile(const char *filename);

// Close a file. Flushes all data buffers, updates inode, directory, etc.
void closeFile(int fp);

// Unmount file system.
void closeFS(){
	unmountFS();
};

