/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.phatcode.net

	Button utility


*******************************************************************************
******************************************************************************/

#include "Button.h"


namespace Button
{


// Control Remap
int Control[8];

// Replay Buttons
unsigned char Buttons;

ReplayType Replay;

u8 KeyPressed[10][BUTTON_MAX_FRAME];
bool CanReplay = false;


ConfigFile Config;

//--------------------------------------
//	
//--------------------------------------
void Init()
{
	Control[CONTROL_UP] = KEY_UP;
	Control[CONTROL_DOWN] = KEY_DOWN;
	Control[CONTROL_LEFT] = KEY_LEFT;
	Control[CONTROL_RIGHT] = KEY_RIGHT;
	Control[CONTROL_FIRE] = KEY_B;
	Control[CONTROL_BOMB] = KEY_A;
	Control[CONTROL_SLOWDOWN] = KEY_R;
	Control[CONTROL_RESERVE] = KEY_L;
	
	Replay.Mode = 0;    // mode 0 = normal, 1 boss rush
	Replay.Stage = 0;
	Replay.ShipType = 0;
	Replay.ShipLevel = 1;
	Replay.ShipLives = 2;
	Replay.ShipBombs = 2;
	Replay.Score = 0;
	Replay.MaxScore = 0;
	Replay.AutoBomb = false;
	
	for( int j = 0; j < 10; j++ )
	{
		for( int i = 0; i < BUTTON_MAX_FRAME; i++)
		{
			KeyPressed[j][i] = 0;
		}
	}
	
	for( int i = 0; i < 8; i++ )
		Config.Control[i] = Control[i];
	
	
	CanReplay = false;
}
	

//--------------------------------------
//	
//--------------------------------------
void Clear()
{
	
	for( int j = 0; j < 10; j++ )
	{
		for( int i = 0; i < BUTTON_MAX_FRAME; i++)
		{
			KeyPressed[j][i] = 0;
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void SaveReplay( int slot )
{

	
			
	// save this too
//	Replay.Mode = 0;
//	Replay.Stage = 0;
//	Replay.ShipType = 0;
//	Replay.ShipLevel = 1;
//	Replay.ShipLives = 2;
//	Replay.ShipBombs = 2;
//	Replay.Score = 0;
	
	
	FILE *ReplayFile;
	switch( slot )
	{
		case 0:
			ReplayFile = fopen( "space_impakto_ds_0.dem", "wb" );
			break;
		case 1:
			ReplayFile = fopen( "space_impakto_ds_1.dem", "wb" );
			break;
		case 2:
			ReplayFile = fopen( "space_impakto_ds_2.dem", "wb" );
			break;
		case 3:
			ReplayFile = fopen( "space_impakto_ds_3.dem", "wb" );
			break;
		default:
			ReplayFile = fopen( "space_impakto_ds_3.dem", "wb" );
		
	}
	
	if( ReplayFile )
	{
		fwrite( &Replay, sizeof(ReplayType), 1, ReplayFile );
		fwrite( KeyPressed, sizeof(u8) * BUTTON_MAX_FRAME, 10, ReplayFile );
		fclose( ReplayFile );
	} 
	
	
	
}

//--------------------------------------
//	
//--------------------------------------
bool LoadReplay( int slot )
{

	FILE *ReplayFile;
	switch( slot )
	{
		case 0:
			ReplayFile = fopen( "space_impakto_ds_0.dem", "rb" );
			break;
		case 1:
			ReplayFile = fopen( "space_impakto_ds_1.dem", "rb" );
			break;
		case 2:
			ReplayFile = fopen( "space_impakto_ds_2.dem", "rb" );
			break;
		case 3:
			ReplayFile = fopen( "space_impakto_ds_3.dem", "rb" );
			break;
		default:
			ReplayFile = fopen( "space_impakto_ds_3.dem", "rb" );
			break;
		
	}
	
	if( ReplayFile )
	{
		fread( &Replay, sizeof(ReplayType), 1, ReplayFile );
		fread( KeyPressed, sizeof(u8) * BUTTON_MAX_FRAME, 10, ReplayFile );
		fclose( ReplayFile );
		CanReplay = true;
		return true;
	} 
	
	CanReplay = false;
	return false;
	
}

//--------------------------------------
//	
//--------------------------------------
bool LoadDemo( const u8 *demo_bin )
{

	
	memcpy( &Replay, demo_bin, sizeof(ReplayType)  );
	// offset by 4 bytes(1 bool) since old replays are not compatible and we are using  a 1.3.0 replay
	memcpy( KeyPressed, demo_bin + sizeof(ReplayType) - 4, sizeof(u8) * BUTTON_MAX_FRAME * 10  );
	CanReplay = true;
	return true;

}

//--------------------------------------
//	
//--------------------------------------
bool ReadReplayHeader( int slot, ReplayType &Rep )
{

	FILE *ReplayFile;
	switch( slot )
	{
		case 0:
			ReplayFile = fopen( "space_impakto_ds_0.dem", "rb" );
			break;
		case 1:
			ReplayFile = fopen( "space_impakto_ds_1.dem", "rb" );
			break;
		case 2:
			ReplayFile = fopen( "space_impakto_ds_2.dem", "rb" );
			break;
		case 3:
			ReplayFile = fopen( "space_impakto_ds_3.dem", "rb" );
			break;
		default:
			ReplayFile = fopen( "space_impakto_ds_3.dem", "rb" );
			break;
		
	}
	
	if( ReplayFile )
	{
		fread( &Rep, sizeof(ReplayType), 1, ReplayFile );
		fclose( ReplayFile );
		//CanReplay = false;
		return true;
	} 
	
	//CanReplay = false;
	return false;
	
}


//--------------------------------------
//	
//--------------------------------------
bool SaveConfigFile()
{
	
	FILE *Configuration;
	Configuration = fopen( "space_impakto_ds.cfg", "wb" );
	
	if( Configuration )
	{
		fwrite( &Config, sizeof(ConfigFile), 1, Configuration );
		fclose( Configuration );
		
		return true;
	} 
	return false;
}


//--------------------------------------
//	
//--------------------------------------
bool LoadConfigFile()
{
	
	FILE *Configuration;
	Configuration = fopen( "space_impakto_ds.cfg", "rb" );
	
	if( Configuration )
	{
		fread( &Config, sizeof(ConfigFile), 1, Configuration );
		fclose( Configuration );
		
		return true;
	} 
	return false;
}

}  // namespace

