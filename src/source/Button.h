/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.phatcode.net

	Button utility


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BUTTON_H
#define BUTTON_H

namespace Button
{

#define BUTTON_MAX_FRAME 32768	
	
enum E_BUTTON_FLAGS
{
	BUTTON_UP = (1 << 0),
	BUTTON_DOWN = (1 << 1),
	BUTTON_LEFT = (1 << 2),
	BUTTON_RIGHT = (1 << 3),
	BUTTON_FIRE = (1 << 4),
	BUTTON_BOMB = (1 << 5),
	BUTTON_SLOWDOWN = (1 << 6),
	BUTTON_RESERVE = (1 << 7)
};

enum E_BUTTON_CONTROLS
{
	CONTROL_UP = 0,
	CONTROL_DOWN,
	CONTROL_LEFT,
	CONTROL_RIGHT,
	CONTROL_FIRE,
	CONTROL_BOMB,
	CONTROL_SLOWDOWN,
	CONTROL_RESERVE
};

struct ReplayType
{
	int Mode;  // 0 = normal, 1 boss rush
	int Stage;
	int ShipType;
	int ShipLevel;
	int ShipLives;
	int ShipBombs;
	int Score;
	int MaxScore;
	bool AutoBomb;
};

struct ConfigFile
{
	char Name[10];
	bool AutoBomb;
	int Control[8];
};
	

// Control Remap
extern int Control[8];

// Replay Buttons
extern unsigned char Buttons;
extern ReplayType Replay;
extern u8 KeyPressed[10][BUTTON_MAX_FRAME];
extern bool CanReplay;
 
// config file
extern ConfigFile Config;


//--------------------------------------
//	
//--------------------------------------
void Init();

void Clear();

void SaveReplay( int slot );

bool LoadReplay( int slot );

bool LoadDemo( const u8 *demo_bin );

bool ReadReplayHeader( int slot, ReplayType &Rep  );

bool SaveConfigFile();
bool LoadConfigFile();

} // namespace

#endif // BUTTON_H
