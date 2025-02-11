#include <iostream>
#include <chrono>
#include <windows.h>
#include <map>
#include <random>
#include <csignal>

extern int get_cache_miss();
extern int get_cache_hit();
extern void reset_cache_stats();
extern void free_all_cache_blocks();

extern int get_rand_from_to(int min, int max);

extern HANDLE lab2_open(const char *path);
extern int lab2_close(HANDLE fd);
extern ptrdiff_t lab2_read(HANDLE fd, void *buf, size_t count);
extern ptrdiff_t lab2_write(HANDLE fd, const void *buf, size_t count);
extern int lab2_lseek(HANDLE fd, int offset, int whence);
extern int lab2_fsync(HANDLE fd);

using namespace std;

int main() {
    bool test1 = false;
    bool test2 = false;
    bool test3 = false;
    bool test4 = true;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    chrono::duration<double> duration;
    HANDLE fd;
    int times;

    if (test1) {
        const char* filename = "test.txt";
        times = 100000;
        char *buf = new char[35000];

        cout << "Test #1 - Simple reading of the same large block " << times << " times\n\n";

        start = chrono::high_resolution_clock::now();
        fd = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
        for (int i = 0; i < times; ++i) {
            DWORD bytesRead;
            SetFilePointer(fd, 0, NULL, FILE_BEGIN);
            ReadFile(fd, buf, 35000, &bytesRead, NULL);
        }
        CloseHandle(fd);
        duration = chrono::high_resolution_clock::now() - start;
        cout << "Execution time without cache: " << duration.count() << " seconds.\n\n";

        start = chrono::high_resolution_clock::now();
        fd = lab2_open(filename);
        for (int i = 0; i < times; ++i) {
            lab2_read(fd, buf, 35000);
            lab2_lseek(fd, 0 , 0);
        }
        lab2_close(fd);
        duration = chrono::high_resolution_clock::now() - start;
        cout << "Execution time with cache: " << duration.count() << " seconds.\n";
        cout << "Cache hits: " << get_cache_hit() << ", Cache miss: " << get_cache_miss() << endl;

        free_all_cache_blocks();
        reset_cache_stats();
        cout << "\n----------------------------------------\n\n\n";
    }

    if (test2) {
        const char* filename = "trash.txt";
        times = 100001;
        char *buf = new char[9000];

        cout << "Test #2 - Reading a random block of data " << times << " times\n\n";

        start = chrono::high_resolution_clock::now();
        fd = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
        for (int i = 0; i < times; ++i) {
            SetFilePointer(fd, get_rand_from_to(0, 300000), NULL, FILE_BEGIN);
            DWORD bytesRead;
            ReadFile(fd, buf, 9000, &bytesRead, NULL);
        }
        CloseHandle(fd);
        duration = chrono::high_resolution_clock::now() - start;
        cout << "Execution time without cache: " << duration.count() << " seconds.\n\n";

        start = chrono::high_resolution_clock::now();
        fd = lab2_open(filename);
        for (int i = 0; i < times; ++i) {
            lab2_lseek(fd, get_rand_from_to(0, 300000), 0);
            lab2_read(fd, buf, 9000);
        }
        lab2_close(fd);
        duration = chrono::high_resolution_clock::now() - start;
        cout << "Execution time with cache: " << duration.count() << " seconds.\n";
        cout << "Cache hits: " << get_cache_hit() << ", Cache miss: " << get_cache_miss() << endl;

        free_all_cache_blocks();
        reset_cache_stats();
        cout << "\n----------------------------------------\n\n\n";
    }

    if (test3) {
        const char* filename = "write_test22.txt";
        times = 100001;
        char *buf = new char[500];

        cout << "Test #3 - Reading from an arbitrary location and writing the result to an arbitrary location in the file " << times << " times\n\n";

        start = chrono::high_resolution_clock::now();
        fd = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
        for (int i = 0; i < times; ++i) {
            SetFilePointer(fd, get_rand_from_to(0, 300000), NULL, FILE_BEGIN);
            DWORD bytesRead;
            ReadFile(fd, buf, 500, &bytesRead, NULL);
            SetFilePointer(fd, get_rand_from_to(0, 300000), NULL, FILE_BEGIN);
            DWORD bytesWritten;
            WriteFile(fd, buf, 500, &bytesWritten, NULL);
        }
        CloseHandle(fd);
        duration = chrono::high_resolution_clock::now() - start;
        cout << "Execution time without cache: " << duration.count() << " seconds.\n\n";

        start = chrono::high_resolution_clock::now();
        fd = lab2_open(filename);
        for (int i = 0; i < times; ++i) {
            lab2_lseek(fd, get_rand_from_to(0, 300000), 0);
            lab2_read(fd, buf, 500);
            lab2_lseek(fd, get_rand_from_to(0, 300000), 0);
            lab2_write(fd, buf, 500);
        }
        lab2_fsync(fd);
        lab2_close(fd);
        duration = chrono::high_resolution_clock::now() - start;
        cout << "Execution time with cache: " << duration.count() << " seconds.\n";
        cout << "Cache hits: " << get_cache_hit() << ", Cache miss: " << get_cache_miss() << endl;

        free_all_cache_blocks();
        reset_cache_stats();
        cout << "\n----------------------------------------\n\n\n";
    }

    if (test4) {
        const char* filename1 = "cifri.txt";  // 6780
        const char* filename2 = "bukvi.txt";  // 5761
        const char* filename3 = "itoito.txt"; // 0
        char *buf = new char[100];

        cout << "Test #4 - Working with multiple files at the same time\n\n";

        HANDLE fd1 = lab2_open(filename1);
        HANDLE fd2 = lab2_open(filename2);
        HANDLE fd3 = lab2_open(filename3);

        lab2_lseek(fd1, get_rand_from_to(0, 6780), 0);
        lab2_read(fd1, buf, 100);

        lab2_write(fd3, buf, 50);

        lab2_lseek(fd2, get_rand_from_to(0, 5761), 0);
        lab2_read(fd2, buf, 100);

        lab2_write(fd3, buf, 100);

        lab2_fsync(fd3);
        lab2_close(fd1);
        lab2_close(fd2);
        lab2_close(fd3);
        cout << "Cache hits: " << get_cache_hit() << ", Cache miss: " << get_cache_miss() << endl;

        free_all_cache_blocks();
        reset_cache_stats();
        cout << "\n----------------------------------------\n\n\n";
    }

    return 0;
}
