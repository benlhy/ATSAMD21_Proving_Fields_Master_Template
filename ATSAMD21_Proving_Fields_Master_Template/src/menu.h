

#ifndef MENU_H__
#define MENU_H__

#include <asf.h>
#include <stdio.h>
#include "lcd.h"

// typedef struct {
// 	char* nodeName;
// 	uint8_t numSubMenu;
// 	struct NODE* prevNode;
// 	uint8_t current_pointer;
// 	struct NODE* submenu[];
// } NODE;


void printMenu(uint8_t pos,uint8_t menuIndex, uint8_t selection);
/*void printNodeMenu(NODE curr_node);*/


#endif
