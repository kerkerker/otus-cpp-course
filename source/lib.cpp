#include "lib.h"
#include "version.h"

int major_version() {
    return PROJECT_VERSION_MAJOR;
}

int minor_version() {
    return PROJECT_VERSION_MINOR;
}

int patch_version() {
    return PROJECT_VERSION_PATCH;
}