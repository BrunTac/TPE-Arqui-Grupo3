#ifndef _PHYLO_H_
#define _PHYLO_H_

#include <stdint.h>

void phylo();
void addPhilosopher(uint8_t idx);
void removePhilosopher(uint8_t idx);
void view();
void philosopher(uint64_t argc, char ** argv);
uint8_t left(uint8_t i);
uint8_t right(uint8_t i);
void take_forks(int i);
void put_forks(int i);
void think(int i);
void eat(int i);

#endif