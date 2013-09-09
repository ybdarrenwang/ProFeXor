#ifndef __DEBUG_H__
#define __DEBUG_H__

inline void DUMP(const char *str) {
#ifdef DEBUG
  std::cout << str << std::endl;
#endif
}

#endif
