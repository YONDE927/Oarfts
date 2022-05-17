#pragma once
#include <sys/stat.h>

typedef enum req_t {
    NONE,
    OPEN,
    CLOSE,
    READ,
    WRITE,
    STAT,
    READDIR,
} req_t;

typedef enum res_t {
    DENY,
    YES,
    NO,
    DISCONNECT,
    BUSY,
    WAIT
} res_t;

typedef struct Request{
    req_t type;
    char path[256];
    char data[64];
} Request;

typedef struct Response{
    res_t type;
} Response;

typedef struct filedata{
    char data[1024];
} filedata;

typedef struct fileattr{
    char path[256];
    struct stat st;
} fileattr;