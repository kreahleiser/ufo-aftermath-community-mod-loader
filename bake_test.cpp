#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "modloader.h"

extern "C" void ModLog(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: bake_test \"C:\\path\\to\\UFO Aftermath\"\n");
        return 1;
    }
    ModLoaderSetGameDir(argv[1]);
    EnsureModsApplied();
    printf("bake finished\n");
    return 0;
}
