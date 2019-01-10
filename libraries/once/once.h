#ifndef ONCE_H
#define ONCE_H
#define ONCE(operation) { static bool run_yet{false}; if (run_yet) { { operation; } run_yet = true; }}
#endif