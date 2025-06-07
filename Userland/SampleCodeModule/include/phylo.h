#ifndef _PHYLO_H_
#define _PHYLO_H_

#include <stdint.h>

void phylo(uint64_t argc, char ** argv);
void addPhilosopher(uint8_t idx);
void removePhilosopher(uint8_t idx);
void view();
void philosopher(uint64_t argc, char ** argv);
void take_forks(int i);
void put_forks(int i);
void think(int i);
void eat(int i);

#endif