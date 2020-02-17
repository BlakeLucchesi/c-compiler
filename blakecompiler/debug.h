//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef debug_h
#define debug_h

//#define PRINT 0

#ifdef PRINT
#define debug(string, ...) printf(string, ##__VA_ARGS__)
#else
#define debug(string, ...)
#endif

#endif /* debug_h */
