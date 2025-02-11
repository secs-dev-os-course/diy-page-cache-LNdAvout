#ifndef APP_H
#define APP_H
#include <iostream>
#include <random>
#include <map>
#include <windows.h>

extern int get_cache_miss();
extern int get_cache_hit();
extern void reset_cache_stats();
extern void free_all_cache_blocks();
extern int get_rand_from_to(int min, int max);
extern int lab2_close(HANDLE fd);
extern HANDLE lab2_open(const char* path);
extern ptrdiff_t lab2_read(HANDLE fd, void *buf, size_t count);
extern ptrdiff_t lab2_write(HANDLE fd, const void *buf, size_t count);
extern int lab2_lseek(HANDLE fd, int offset, int whence);
extern int lab2_fsync(HANDLE fd);

#endif //APP_H
