//https://atcoder.jp/contests/rco-contest-2019-qual/submissions/4238153

#include "sys/time.h"

struct timeval timer_begin, timer_end;
int timer_called;
inline void timer_start() 
{
  timer_called++;
  gettimeofday(&timer_begin, NULL);    
}     
inline double timer_now() 
{         
  timer_called++;
  gettimeofday(&timer_end, NULL);         
  return timer_end.tv_sec - timer_begin.tv_sec +
    (timer_end.tv_usec - timer_begin.tv_usec)/1000000.0;     
}