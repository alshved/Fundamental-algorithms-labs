#pragma once

typedef enum Opts { OPT_L, OPT_R, OPT_U, OPT_N, OPT_C } Opts;

typedef enum StringResult { 
    MEMORY_ALLOCATING_ERROR,
     SUCCESS 
} StringResult;

typedef enum ParseResult { 
    FEW_ARGUMENTS, 
    SUCCESSS, 
    A_LOT_OF_ARGUMENTS, 
    BAD_ARGUMENTS,
    BAD_FLAG 
} ParseResult;