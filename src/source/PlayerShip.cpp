/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	PlayerShip class


*******************************************************************************
******************************************************************************/

#include "PlayerShip.h"


//--------------------------------------------------------------------
//    
//--------------------------------------------------------------------
PlayerShip::PlayerShip()
{
	
	Active = false;
	Alive = false;
	Level = 1;
	DamageMultiplier = Level;
	Score = 0;
	Lives = 2;
	Bombs = 2;
	
	ShotSpawnDelay = 0;
	MissileSpawnDelay = 0;
	ShotFrameOffset = 0;
	WaveShotFrameOffset = 0;
	ShotAngleOffset = 0;
	MissileFrameOffset = 0;
	MissileRotation = 1;
	
	NewLifeCounter = 64;
	Frame = 0;
	BaseFrame = 2;
	Counter = 0;
	Direction = NEUTRAL;
	AnimationSpeed = 0;
	x = -30 << 12;
	y = ( SCREEN_HEIGHT/2 ) << 12;
	Dx = 0;
	Dy = 0;
	
	ThrowBomb = false;
	AutoBomb = false;
	
	Width = 32;
	Height = 16;
	
	Position.x = x;
	Position.y = y;
	
	State = 0;
	State = E_LOCKED | E_INVINCIBLE;
	Mode = E_ENTER;
	
	StateCounter = 60 * 4;
	
	
	
}

//--------------------------------------------------------------------
//    
//--------------------------------------------------------------------
PlayerShip::~PlayerShip()
{

}


//--------------------------------------
//	
//--------------------------------------
void PlayerShip::NewStage()
{
	
	Counter = 0;
	State = 0;
	State = E_LOCKED | E_INVINCIBLE;
	Mode = E_ENTER;
	
	StateCounter = 60 * 4;
	
	
	x = -30 << 12;
	y = 96 << 12;
	Dx = 0;
	Dy = 0;
	
	ShotSpawnDelay = 0;
	MissileSpawnDelay = 0;
	ShotFrameOffset = 0;
	WaveShotFrameOffset = 0;
	ShotAngleOffset = 0;
	MissileFrameOffset = 0;
	MissileRotation = 1;
	
	ThrowBomb = false;
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
	Active = true;
	Alive = true;
	
	Direction = NEUTRAL;
	AnimationSpeed = 0;
	
	Sound::Instance()->PlaySFX( E_SFX_VIPER_ENTER );	
	
		
}

//--------------------------------------
//	
//--------------------------------------
void PlayerShip::NewLife()
{

	Counter = 0;
	State = 0;
	State = E_LOCKED | E_INVINCIBLE;
	Mode = E_ENTER;
	
	StateCounter = 60 * 4;
	
	
	x = -30 << 12;
	y = 96 << 12;
	Dx = 0;
	Dy = 0;
	
	Level = 1;
	Bombs = 2;
	
	ShotSpawnDelay = 0;
	MissileSpawnDelay = 0;
	ShotFrameOffset = 0;
	WaveShotFrameOffset = 0;
	ShotAngleOffset = 0;
	MissileFrameOffset = 0;
	MissileRotation = 1;
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
	ThrowBomb = false;
	
	Active = true;
	Alive = true;
	
	Direction = NEUTRAL;
	AnimationSpeed = 0;
	
	Sound::Instance()->PlaySFX( E_SFX_VIPER_ENTER );	
	
}




//--------------------------------------
//	
//--------------------------------------
void PlayerShip::NewGame()
{
	Score = 0;
	Bombs = 2;
	Lives = 2;
	Level = 1;
	ShotSpawnDelay = 0;
	MissileSpawnDelay = 0;
	ThrowBomb = false;
	
	NewStage();	
}


//--------------------------------------
//	
//--------------------------------------
int PlayerShip::LoadReplay( const Button::ReplayType &Replay )
{
	
	NewLife();
	
	ID = (SHIP_ID)Replay.ShipType;
	Level = Button::Replay.ShipLevel;
	Lives = Button::Replay.ShipLives;
	Bombs = Button::Replay.ShipBombs;
	Score = Button::Replay.Score;
	AutoBomb = Button::Replay.AutoBomb;
	
	return Replay.Stage;
		
}

//--------------------------------------
//	
//--------------------------------------
void PlayerShip::Kill()
{
	Lives--;
	NewLifeCounter = 60 * 2;
	Alive = false;
	
	Sound::Instance()->PlaySFX( E_SFX_VIPER_EXPLODE );
	
	// Add some nice explosions
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  Dx,
								  Dy,
								  Explode::MEDIUM_3,
								  Explode::SMALL_2
								 );
		
	
	
	x = -30 << 12;
	y = 96 << 12;
	
	Position.x = x;
	Position.y = y;
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
	
	State = E_LOCKED | E_INVINCIBLE;
	
}



//--------------------------------------
//	
//--------------------------------------
void PlayerShip::SetTargets()
{

	int NumTargets = Targets::Instance()->GetNumTargets();
		
	if( NumTargets == 0)
	{
		for( int j = 0; j < MAX_PLAYERSHIP_MISSILES; j++ )
		{
			Missiles[j].SetTarget( 128 << 24, y );
		}
	}
	else
	{
		int StartTarget = 0;
		for( int j = 0; j < MAX_PLAYERSHIP_MISSILES; j++ )
		{
			TargetEntity T = Targets::Instance()->GetTarget(StartTarget);
			Missiles[j].SetTarget( T.GetX(), T.GetY() );
			StartTarget++;
			if( StartTarget >= ( NumTargets - 1 ) ) StartTarget = 0;
			
		}  //next j
	}  // end if

}
	

//--------------------------------------
//	
//--------------------------------------
void PlayerShip::KillArmament()
{
	
	for( int i = 0; i < MAX_PLAYERSHIP_BULLETS; i++)
	{
		Shots[i].Kill();
	}

	for( int i = 0; i < MAX_PLAYERSHIP_MISSILES; i++)
	{
		Missiles[i].Kill();
	}  	
}
	

