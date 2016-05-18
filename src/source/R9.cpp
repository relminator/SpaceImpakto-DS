/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	R9 class


*******************************************************************************
******************************************************************************/

#include "R9.h"

#define MIN_X ( 0 )
#define MIN_Y ( 0 )

#define MAX_X ( SCREEN_WIDTH << 12 )
#define MAX_Y ( ( SCREEN_HEIGHT-8 ) << 12 )


#define  MAX_R9_SHOT_DELAY 6
#define  MAX_R9_MISSILE_DELAY 16

static const s32 R9_DX = floattof32(2.048);
static const s32 R9_DY = floattof32(1.536);
static const s32 R9_SLOWDOWN_DAMPER = floattof32(0.5);


//--------------------------------------------------------------------
//    
//--------------------------------------------------------------------
void R9::Init( int Palette, glImage* const Sprites )
{
	Images = Sprites;
	ColorTable = Palette;
	Width = 26;
	Height = 16;
	DamageMultiplier = 1;
	ID = PlayerShip::R9;
	ControlType = 0;
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
	Aabb.Resize( 1024*3 );
		
}

//--------------------------------------
//	
//--------------------------------------
bool R9::Update( int Key, int KeyD, Bomb &Bomber )
{
	Counter++;
	
	if( x > (300<<12) )
	{
		return true;
	}
	
	
	// update shots
	NumShots = 0;
	for( int i = 0; i < MAX_PLAYERSHIP_BULLETS; i++)
	{
		if( Shots[i].IsActive() )
		{
			NumShots++;
			Shots[i].Update();
		}
	}
	
	
	NumMissiles = 0;
	for( int i = 0; i < MAX_PLAYERSHIP_MISSILES; i++)
	{
		if( Missiles[i].IsActive() )
		{
			NumMissiles++;
			Missiles[i].Update();
		}
	}
	
	// Get out early if viper is dead
	if( !Alive ) return false;
	
	
	if( StateCounter > 0 )
	{
		StateCounter--;
		if( StateCounter == 0 )
		{
			State &= ~E_INVINCIBLE;
		}
	}
	
	Dx = 0;
	Dy = 0;
	
	// Manual Control
	Direction = NEUTRAL;
	if( !(State & E_LOCKED) )
	{
		// process input
		if( Key & Button::Control[Button::CONTROL_RIGHT] )
		{
			if( (x >> 12) < SCREEN_WIDTH-12 ) Dx += R9_DX;
			Direction = RIGHT;
			UTIL::BitSet(Button::Buttons, Button::BUTTON_RIGHT);
		}
		
		if( Key & Button::Control[Button::CONTROL_LEFT] )
		{
			if( (x >> 12) > 12 ) Dx -= R9_DX;
			Direction = LEFT;
			UTIL::BitSet(Button::Buttons, Button::BUTTON_LEFT);
		}
		
		if( Key & Button::Control[Button::CONTROL_DOWN] )
		{
			if( (y >> 12) < SCREEN_HEIGHT-8 ) Dy += R9_DY;
			Direction = DOWN;
			UTIL::BitSet(Button::Buttons, Button::BUTTON_DOWN);
		}

		if( (Key & Button::Control[Button::CONTROL_UP]) )
		{
			if( (y >> 12) > 8 ) Dy -= R9_DY;
			Direction = UP;
			UTIL::BitSet(Button::Buttons, Button::BUTTON_UP);
		}
		
		if( (Key & Button::Control[Button::CONTROL_FIRE]) )
		{			
			ShotSpawnDelay--;
			if( ShotSpawnDelay < 0 )
			{
				ShotSpawnDelay = MAX_R9_SHOT_DELAY;
				SpawnSpreadShot();
				Sound::Instance()->PlaySFX( E_SFX_SHOT );
			}
			
			MissileSpawnDelay--;
			if( MissileSpawnDelay < 0 )
			{
				MissileSpawnDelay = MAX_R9_MISSILE_DELAY;
				if( Level > 3 ) SpawnMissile();
			}
			UTIL::BitSet(Button::Buttons, Button::BUTTON_FIRE);
		}
		
		
		// Bomber
		if( (KeyD & Button::Control[Button::CONTROL_BOMB]) || ThrowBomb )
		{
			if( Bombs > 0 )
			{
				if( !Bomber.IsActive() )
				{
					DecBombs();
					Bomber.Reset(ID);
					Bomber.Activate();
					Sound::Instance()->PlaySFX( E_SFX_BOMB );
				}
			}
			UTIL::BitSet(Button::Buttons, Button::BUTTON_BOMB);
		}
		
		if( (Key & Button::Control[Button::CONTROL_SLOWDOWN]) )
		{
			Dx = mulf32( Dx, R9_SLOWDOWN_DAMPER );
			Dy = mulf32( Dy, R9_SLOWDOWN_DAMPER );
			UTIL::BitSet(Button::Buttons, Button::BUTTON_SLOWDOWN);
		}
		
		x += Dx;
		y += Dy;
		
		
		
		//Limit();
			
	}
	else		// Auto mode
	{
		if( Mode == E_ENTER )
		{
			if( x < (60 << 12) ) 
			{
				Direction = RIGHT;
				x += 2 << 12 ;
			}
			else
			{
				State &= ~E_LOCKED;
			}
		}
		else  //exit
		{
			if( x < (350 << 12) ) 
			{
				Direction = RIGHT;
				x += 2 << 12 ;
			}
		}
		
		
		
	}
	
	
	Animate();

	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
	Aabb.Resize( 1024*3 );
	
	if( (Counter & 3) == 0 )
	{
		JetFrame = (JetFrame +  1) & 3;
	}	
		
	Position.x = x;
	Position.y = y;
	
	//DamageMultiplier = 1 + (Level / 2);
	DamageMultiplier = Level;
	
	
	return false;
	
	
}

//--------------------------------------
//	
//--------------------------------------
bool R9::Replay( int Frame, Bomb &Bomber, int Stage )
{
	Counter++;
	
	if( x > (300<<12) )
	{
		return true;
	}
	
	
	// update shots
	NumShots = 0;
	for( int i = 0; i < MAX_PLAYERSHIP_BULLETS; i++)
	{
		if( Shots[i].IsActive() )
		{
			NumShots++;
			Shots[i].Update();
		}
	}
	
	
	NumMissiles = 0;
	for( int i = 0; i < MAX_PLAYERSHIP_MISSILES; i++)
	{
		if( Missiles[i].IsActive() )
		{
			NumMissiles++;
			Missiles[i].Update();
		}
	}
	
	// Get out early if viper is dead
	if( !Alive ) return false;
	
	
	if( StateCounter > 0 )
	{
		StateCounter--;
		if( StateCounter == 0 )
		{
			State &= ~E_INVINCIBLE;
		}
	}
	
	Dx = 0;
	Dy = 0;
	
	// Manual Control
	Direction = NEUTRAL;
	if( !(State & E_LOCKED) )
	{
		// process input
		if( UTIL::BitIsSet( Button::KeyPressed[Stage][Frame], Button::BUTTON_RIGHT ) )
		{
			if( (x >> 12) < SCREEN_WIDTH-12 ) Dx += R9_DX;
			Direction = RIGHT;
		}
		
		if( UTIL::BitIsSet( Button::KeyPressed[Stage][Frame], Button::BUTTON_LEFT ) )
		{
			if( (x >> 12) > 12 ) Dx -= R9_DX;
			Direction = LEFT;
		}
		
		if( UTIL::BitIsSet( Button::KeyPressed[Stage][Frame], Button::BUTTON_DOWN ) )
		{
			if( (y >> 12) < SCREEN_HEIGHT-8 ) Dy += R9_DY;
			Direction = DOWN;
		}

		if( UTIL::BitIsSet( Button::KeyPressed[Stage][Frame], Button::BUTTON_UP ) )
		{
			if( (y >> 12) > 8 ) Dy -= R9_DY;
			Direction = UP;
		}
		
		if( UTIL::BitIsSet( Button::KeyPressed[Stage][Frame], Button::BUTTON_FIRE ) )
		{			
			ShotSpawnDelay--;
			if( ShotSpawnDelay < 0 )
			{
				ShotSpawnDelay = MAX_R9_SHOT_DELAY;
				SpawnSpreadShot();
				Sound::Instance()->PlaySFX( E_SFX_SHOT );
			}
			
			MissileSpawnDelay--;
			if( MissileSpawnDelay < 0 )
			{
				MissileSpawnDelay = MAX_R9_MISSILE_DELAY;
				if( Level > 3 ) SpawnMissile();
			}
		}
		
		
		// Bomber
		if( UTIL::BitIsSet( Button::KeyPressed[Stage][Frame], Button::BUTTON_BOMB ) || ThrowBomb )
		{
			if( Bombs > 0 )
			{
				if( !Bomber.IsActive() )
				{
					DecBombs();
					Bomber.Reset(ID);
					Bomber.Activate();
					Sound::Instance()->PlaySFX( E_SFX_BOMB );
				}
			}
		}
		
		if( UTIL::BitIsSet( Button::KeyPressed[Stage][Frame], Button::BUTTON_SLOWDOWN ) )
		{
			Dx = mulf32( Dx, R9_SLOWDOWN_DAMPER );
			Dy = mulf32( Dy, R9_SLOWDOWN_DAMPER );
		}
		
		x += Dx;
		y += Dy;
		
		
		
		//Limit();
			
	}
	else		// Auto mode
	{
		if( Mode == E_ENTER )
		{
			if( x < (60 << 12) ) 
			{
				Direction = RIGHT;
				x += 2 << 12 ;
			}
			else
			{
				State &= ~E_LOCKED;
			}
		}
		else  //exit
		{
			if( x < (350 << 12) ) 
			{
				Direction = RIGHT;
				x += 2 << 12 ;
			}
		}
		
		
		
	}
	
	
	Animate();

	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
	Aabb.Resize( 1024*3 );
	
	if( (Counter & 3) == 0 )
	{
		JetFrame = (JetFrame +  1) & 3;
	}	
		
	Position.x = x;
	Position.y = y;
	
	//DamageMultiplier = 1 + (Level / 2);
	DamageMultiplier = Level;
	
	return false;
	
	
}

//--------------------------------------------------------------------
//    
//--------------------------------------------------------------------
void R9::Draw() const
{
	
	
	
	int f = Frame+BaseFrame;
	int offx = Width/2;
	int offy = Height/2;
	
	glSprite( (x >> 12)-offx-8, (y >> 12)-7, GL_FLIP_NONE, &Images[JetFrame + 5] );
	glSprite( (x >> 12)-offx-4, (y >> 12)-0, GL_FLIP_NONE, &Images[((JetFrame + 2) & 3)+ 5] );
	
	if( IsInvincible() && (((Counter) & 3) == 0) )
	{
		glColor( RGB15(31,0,0) );
		glSprite( (x >> 12)-offx, (y >> 12)-offy, GL_FLIP_NONE, &Images[f] );
		glColor( RGB15(31,31,31) );
	}
	else
	{
		glSprite( (x >> 12)-offx, (y >> 12)-offy, GL_FLIP_NONE, &Images[f] );
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void R9::DrawShots() const
{
	
	// Bullets
	for( int i = 0; i < MAX_PLAYERSHIP_BULLETS; i++)
	{
		if( Shots[i].IsActive() )
		{
			
			int df = Shots[i].GetDrawFrame();
			glSprite( (Shots[i].GetX() >> 12) - Shots[i].GetWidth()/2,
					  (Shots[i].GetY() >> 12) - Shots[i].GetHeight()/2, 
					  GL_FLIP_NONE, 
					  &Images[df] 
					);
			
		}
	}
	
	// Missiles
	for( int i = 0; i < MAX_PLAYERSHIP_MISSILES; i++)
	{
		if( Missiles[i].IsActive() )
		{
			
			int df = Missiles[i].GetDrawFrame();
			glSpriteRotate( (Missiles[i].GetX() >> 12),
							(Missiles[i].GetY() >> 12),
							Missiles[i].GetAngle(),
							GL_FLIP_NONE, 
							&Images[df] );
			
		}
	}
	
	
	
}


//--------------------------------------
//	
//--------------------------------------
void R9::DrawAABB( int color ) const
{
	AABB aabb = Aabb.Get();
	glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
	
	// Bullets
	for( int i = 0; i < MAX_PLAYERSHIP_BULLETS; i++)
	{
		if( Shots[i].IsActive() )
		{
	
			AABB aabb = Shots[i].GetAABB();
			glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
			
		}
	}
	
	// Missiles
	for( int i = 0; i < MAX_PLAYERSHIP_MISSILES; i++)
	{
		if( Missiles[i].IsActive() )
		{
			
			AABB aabb = Missiles[i].GetAABB();
			glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void R9::SpawnSpreadShot()
{

	
	const s32 SPREAD_ANGLE = BRAD_PI / 32;
	
	
	int baseframe = 12;
	int Lvl = Level;
	
	if( Lvl > 3 ) Lvl = 3;
	
	int BULLETS_PER_SHOT = Lvl + (Lvl-1);
	
				
	s32 StartAngle =  -mulf32( ( (BULLETS_PER_SHOT+1) << 12 )/2, (SPREAD_ANGLE));

	ShotFrameOffset++;
	if( ShotFrameOffset > 2 ) ShotFrameOffset = 0;

	for( int j = 0; j < BULLETS_PER_SHOT; j++ )
	{
	
		StartAngle += SPREAD_ANGLE;
		
		if(j <= (BULLETS_PER_SHOT/2))
		{
			baseframe++;
		}
		else
		{
			baseframe--;
		}
		// normal
		for( int i = 0; i < MAX_PLAYERSHIP_BULLETS; i++)
		{
			if( !Shots[i].IsActive() )
			{
				
				Shots[i].InitNormal( x + (16 << 12), y, 
									 ShotFrameOffset,
									 baseframe,
									 StartAngle,
									 4096*6,
									 Lvl );
				
				break;
			}
		}
		
		
	}
	
}



//--------------------------------------
//	
//--------------------------------------
void R9::SpawnMissile()
{

	//bool Spawned = false;
	
	if( NumMissiles > MAX_PLAYERSHIP_MISSILES - 2 ) return;
	
	for( int j = 0; j < 2; j++)
	{
		for( int i = 0; i < MAX_PLAYERSHIP_MISSILES; i++)
		{
			if( !Missiles[i].IsActive() )
			{
				Missiles[i].Init( x - (16 << 12), y, MissileFrameOffset, MissileRotation );
				Missiles[i].SetEnergy(8);
				MissileFrameOffset++;
				if( MissileFrameOffset > 2 ) MissileFrameOffset = 0;
				
				MissileRotation = -MissileRotation;
				//Spawned = true;
				break;
			}
		}
	}
	
	//if( Spawned ) Sound::Instance()->PlaySFX( E_SFX_MISSILE );

}


//--------------------------------------
//	
//--------------------------------------
void R9::Animate()
{
	
	if( (Direction == UP) && AnimationSpeed > -(2 << 12) )
		AnimationSpeed -= 512;
	
	if( (Direction == DOWN) && AnimationSpeed < (2 << 12) )
		AnimationSpeed += 512;
		
	if( (Direction == NEUTRAL) || (Direction == LEFT) || (Direction == RIGHT))
	{
		if( AnimationSpeed > 0 )AnimationSpeed -= 512;
		if( AnimationSpeed < 0 )AnimationSpeed += 512;
		
	}
	
	Frame = -(AnimationSpeed >> 12);
	
}

