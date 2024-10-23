#pragma once
typedef enum ResultCode {
	SEEK_ERROR,
	EMPTY_STRING,
	FILE_OPEN_ERROR,
	MEMORY_ALLOCATING_ERROR,
	EMPTY_FILE,
	SUCCESS
} ResultCode;

typedef enum KMPErrors { MEMORY_ERROR, SUCCESSS } KMPErrors;