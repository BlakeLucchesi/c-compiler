//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef debug_h
#define debug_h

static int LOGS_ENABLED;

#define debug(string, ...) \
if (LOGS_ENABLED) { \
    printf(string "\n", ##__VA_ARGS__); \
}

#endif /* debug_h */
