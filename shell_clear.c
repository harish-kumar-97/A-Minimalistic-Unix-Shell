#include "common.h"

int shell_clear(char **args) {
        write(1, "\33[H\33[2J", 7);
        return 1;
}
