//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef debug_h
#define debug_h

#if DEBUG
#define debug(string, ...) printf(string "\n", ##__VA_ARGS__)
#else
#define debug(string, ...)
#endif

#endif /* debug_h */
