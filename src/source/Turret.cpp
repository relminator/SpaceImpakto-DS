/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.phatcode.net

	Turret class


*******************************************************************************
******************************************************************************/
#include "Turret.h"

void Rotate1(Turret *t);
void Rotate2(Turret *t);
void Rotate3(Turret *t);

void ModeSingle( Turret *t, BulletArray &Bullets );
void ModeSprayNormal( Turret *t, BulletArray &Bullets );
void ModeSprayCentered( Turret *t, BulletArray &Bullets );
void ModeSpreadOffset( Turret *t, BulletArray &Bullets );
void ModeSpreadRings( Turret *t, BulletArray &Bullets );

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
Turret::Turret()
{
	
	
	Active = false;
	Activated = false;
	Patterns.clear();
	Index = 0;
	ShotCounter = 0;
	
	Rotate = Rotate1;
	DoMode = ModeSingle;
	
	DefaultPosition.x = 0;
	DefaultPosition.y = 0;
	
	DefaultTarget.x = 0;
	DefaultTarget.y = 0;
	
	Position = &DefaultPosition;
	Target = &DefaultTarget;
	
}

Turret::~Turret()
{
	Kill();
	Position = 0;
	Target = 0;
	Index = 0;
	
}


//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void Turret::Update( BulletArray &Bullets )
{
	
	// Dec activetime until < 0
	// then increment index circularly
	// and load new parameters
	ActiveTime--;
	if( ActiveTime < 0 )
	{
		Index++;
		if (Index >= Patterns.size())  // rotate index 
		{
			Index = 0;
		}
		
		// Reload new turret params
		ResetPattern( Patterns[Index] );
		
		
	}
	
	// Do ai
	Shoot( Bullets );

		
}

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void Turret::FirstPattern()
{
	
	Index = 0;
	// Reload new turret params
	ResetPattern( Patterns[Index] );
		
}

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void Turret::NextPattern()
{
	
	Index++;
	if (Index >= Patterns.size())  // rotate index 
	{
		Index = 0;
	}
	
	// Reload new turret params
	ResetPattern( Patterns[Index] );
		
}

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void Turret::DoPattern( unsigned int i )
{
	
	if ( i == Index ) return;
	
	Index = i ;
	if (Index >= Patterns.size())  // rotate index 
	{
		Index = Patterns.size() - 1 ;
	}
	
	// Reload new turret params
	ResetPattern( Patterns[Index] );
		
}

//--------------------------------------
//	
//--------------------------------------
void Turret::Init( Params const &P, Vector2df32 *position, Vector2df32 *target  )
{
	
	Position = position;
	Target = target;
	Patterns.push_back( P );
	Index = 0;
	Activated = true;
	
	ResetPattern( P );
	
}


//--------------------------------------
//	
//--------------------------------------
void Turret::Init( Params const &P, s32 px, s32 py, s32 tx, s32 ty )
{
	
	DefaultPosition.x = px;
	DefaultPosition.y = py;
	
	DefaultTarget.x = tx;
	DefaultTarget.y = ty;
	
	Position = &DefaultPosition;
	Target = &DefaultTarget;
	
	Patterns.push_back( P );
	Index = 0;
	Activated = true;
	
	ResetPattern( P );
	
}



//--------------------------------------
//	
//--------------------------------------
void Turret::AddPattern( Params const &P )
{
	Patterns.push_back( P );
}



//--------------------------------------
//	
//--------------------------------------
void Turret::ResetPattern( Params const &P )
{

	ActiveTime		= P.ActiveTime;				// Frames current AI is active
	Active			= P.Active;					// 
	Smart			= P.Smart;					// if targeting or not
	Xradius			= P.Xradius;				// 
	Yradius			= P.Yradius;				// 
	Angle			= P.Angle;					// StartAngle
	MinAngle		= P.MinAngle;				// 
	MaxAngle		= P.MaxAngle;
	AngleDelta		= P.AngleDelta;				// + rotation speed
	SpreadAngle		= P.SpreadAngle;			// distance of each bullet on a spread
	AngleDirection	= P.AngleDirection;			// -1 or +1  determines rotation direction
	RotationType	= P.RotationType;			// whether back anf forth or not
	SpawnDelay		= P.SpawnDelay;				// 
	MaxSpawnDelay	= P.MaxSpawnDelay;			// 
	BulletsPerShot	= P.BulletsPerShot;
	WaveCounter	 	= P.WaveCounter;			// 
	MaxWave			= P.MaxWave;				// maximum bullets per wave
	MaxWaveDelay	= P.MaxWaveDelay;			// maximum delay for next shot in a wave
	
	BulletSpeedDelta	= P.BulletSpeedDelta;		// for variable bullet speeds in a single wave
	BulletSpreadOffset	= P.BulletSpreadOffset;		// for offsetted bullet placement in a single wave
		
	Mode			= P.Mode;
	Bp 				= P.Bp;
	
	SetMode( P.Mode );
	SetRotation( P.RotationType );
	
	ShotCounter = 0;
	
	
}
	
//--------------------------------------
//	
//--------------------------------------
void Turret::Shoot( BulletArray &Bullets )
{
	
	
	SpawnDelay--;
	if( Smart )
	{
		if( SpawnDelay < 0)
		{
			if( WaveCounter == 0 )
			{
				Angle = atan2Lookup( Target->x - Position->x, 
									 Target->y - Position->y
								   );
			}
			
			DoMode( this, Bullets );
			
			
			SpawnDelay = MaxWaveDelay;
			
			WaveCounter++;
			
			if( WaveCounter > MaxWave )
			{
				ShotCounter++;
				WaveCounter = 0;
				SpawnDelay = MaxSpawnDelay;
			}
			
		}
	
	}
	else
	{
		
		if( SpawnDelay < 0 )
		{
			DoMode( this, Bullets );
			
			SpawnDelay = MaxWaveDelay;
			
			WaveCounter++;
			
			if( Mode != SPRAY_WAVE )
			{
				if( WaveCounter > MaxWave )
				{
					ShotCounter++;
					WaveCounter = 0;
					SpawnDelay = MaxSpawnDelay;
					Rotate( this );
				}
			}
			else
			{
				if( WaveCounter > MaxWave )
				{
					ShotCounter++;
					WaveCounter = 0;
					SpawnDelay = MaxSpawnDelay;
				}
				Rotate( this );
			}
			
			
		}
		
	}


}


//--------------------------------------
//	
//--------------------------------------
void Turret::Kill()
{
	Position = &DefaultPosition;
	Target = &DefaultTarget;
	
	Active = false;
	Patterns.clear();
	Index = 0;
	Activated = false;
	ShotCounter = 0;
}


//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void Turret::SetMode( MODE mode )
{
	
	switch( mode )
	{
		case SINGLE:
			DoMode = ModeSingle;
			break;
		case SPRAY_NORMAL:
			DoMode = ModeSprayNormal;
			break;
		case SPRAY_CENTERED:
			DoMode = ModeSprayCentered;
			break;
		case SPRAY_WAVE:
			DoMode = ModeSprayCentered;
			break;
		case SPREAD_OFFSET:
			DoMode = ModeSpreadOffset;
			break;
		case SPREAD_RINGS:
			DoMode = ModeSpreadRings;
			break;
		default:
			DoMode = ModeSingle;
			
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Turret::SetRotation( int type )
{
	switch( type )
	{
		case 0:
			Rotate = Rotate1;
			break;
		case 1:
			Rotate = Rotate2;
			break;
		case 2:
			Rotate = Rotate3;
			break;
		default:
			Rotate = Rotate1;
			
	}
	
}
	





//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void Rotate1(Turret *t)
{

	t->Angle = (t->Angle + t->AngleDelta * t->AngleDirection);
    
}

//--------------------------------------
//	
//--------------------------------------
void Rotate2(Turret *t)
{

	t->Angle = (t->Angle + t->AngleDelta * t->AngleDirection);
	
	if (t->Angle > t->MaxAngle)         
        t->AngleDirection = -t->AngleDirection;         
    
	if (t->Angle < t->MinAngle)         
        t->AngleDirection = -t->AngleDirection;         
	
}

//--------------------------------------
//	
//--------------------------------------
void Rotate3(Turret *t)
{
	
	t->Angle = (t->Angle + t->AngleDelta * t->AngleDirection);       
	
	if (t->Angle > t->MaxAngle)         
        t->Angle = t->MinAngle;         
    
	if (t->Angle < t->MinAngle)         
        t->Angle = t->MaxAngle;         
    
	
}





//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void ModeSingle( Turret *t, BulletArray &Bullets )
{

	sassert( t->Target, "Turret Target Undefined!!!" );
	sassert( t->Position, "Turret Position Undefined!!!" );
			
	s32 CosA = cosLerp(t->Angle);
	s32 SinA = sinLerp(t->Angle);
	
	
	Bullet::Params Bp;
				
	Bp.x = t->Position->x + t->Xradius * CosA;;
	Bp.y = t->Position->y + t->Yradius * SinA;;
	Bp.Dx = (CosA * t->Bp.Speed) >> 12;
	Bp.Dy = (SinA * t->Bp.Speed) >> 12;
	Bp.Gx = t->Bp.Gx;
	Bp.Gy = t->Bp.Gy;
	Bp.AccelX = Bp.Dx / 2;
	Bp.AccelY = Bp.Dy / 2;
	Bp.Speed = t->Bp.Speed;
	Bp.Angle = t->Angle;
	Bp.Rotation = t->Angle;
	Bp.RotationDelta = t->Bp.RotationDelta;
	Bp.StopDelay = t->Bp.StopDelay;
	Bp.Frame = t->Bp.Frame;
	Bp.NumFrames = t->Bp.NumFrames;
	Bp.BaseFrame = t->Bp.BaseFrame;
	Bp.Smart = t->Bp.Smart;
	Bp.Width = t->Bp.Width;
	Bp.Height = t->Bp.Height;
	Bp.Type = t->Bp.Type;
	Bp.AniMode = t->Bp.AniMode;
	
	Bullets.Spawn( Bp, t->Target );
	
	
}


//--------------------------------------
//	
//--------------------------------------
void ModeSprayNormal( Turret *t, BulletArray &Bullets )
{

	sassert( t->Target, "Turret Target Undefined!!!" );
	sassert( t->Position, "Turret Position Undefined!!!" );
	
	
	Bullet::Params Bp;
	s32 angle = t->Angle;
	
	
	Bp.RotationDelta = t->Bp.RotationDelta;
	Bp.StopDelay = t->Bp.StopDelay;
	Bp.Frame = t->Bp.Frame;
	Bp.NumFrames = t->Bp.NumFrames;
	Bp.BaseFrame = t->Bp.BaseFrame;
	Bp.Smart = t->Bp.Smart;
	Bp.Width = t->Bp.Width;
	Bp.Height = t->Bp.Height;
	Bp.Type = t->Bp.Type;
	Bp.AniMode = t->Bp.AniMode;
	
	Bp.Gx = t->Bp.Gx;
	Bp.Gy = t->Bp.Gy;
	Bp.Speed = t->Bp.Speed;
		
	for( int i = 0; i < t->BulletsPerShot; i++ )
	{
		
		s32 factor = t->Bp.Speed + ( (t->BulletSpeedDelta * (i+1)) / t->BulletsPerShot);  // 12
			
		angle += (t->SpreadAngle * t->AngleDirection);		// 12
		s32 CosA = cosLerp(angle);
		s32 SinA = sinLerp(angle);
		Bp.x = t->Position->x + t->Xradius * CosA;;
		Bp.y = t->Position->y + t->Yradius * SinA;;
		Bp.Dx = (CosA * factor) >> 12;
		Bp.Dy = (SinA * factor) >> 12;
		Bp.AccelX = Bp.Dx / 2;
		Bp.AccelY = Bp.Dy / 2;
		Bp.Angle = angle;
		Bp.Rotation = angle;
		
		Bullets.Spawn( Bp, t->Target );
	
	}
	
				
	
}



//--------------------------------------
//	
//--------------------------------------
void ModeSprayCentered( Turret *t, BulletArray &Bullets )
{

	sassert( t->Target, "Turret Target Undefined!!!" );
	sassert( t->Position, "Turret Position Undefined!!!" );
	
	
	Bullet::Params Bp;
	
	s32 BpsDiv2 = ((t->BulletsPerShot-1) << 12) / 2;
	
	s32 BulletOffset = 1 << 12;
	if (t->WaveCounter & 1) BulletOffset = t->BulletSpreadOffset; 
	
	int SpreadRot = (t->SpreadAngle * BulletOffset) >> 12;
	s32 angle = t->Angle - mulf32((((t->BulletsPerShot + 1)<<12)/2), (SpreadRot * t->AngleDirection));
	
	
	
	Bp.RotationDelta = t->Bp.RotationDelta;
	Bp.StopDelay = t->Bp.StopDelay;
	Bp.Frame = t->Bp.Frame;
	Bp.NumFrames = t->Bp.NumFrames;
	Bp.BaseFrame = t->Bp.BaseFrame;
	Bp.Smart = t->Bp.Smart;
	Bp.Width = t->Bp.Width;
	Bp.Height = t->Bp.Height;
	Bp.Type = t->Bp.Type;
	Bp.AniMode = t->Bp.AniMode;
	
	Bp.Gx = t->Bp.Gx;
	Bp.Gy = t->Bp.Gy;
	Bp.Speed = t->Bp.Speed;
		
	for( int i = 0; i < t->BulletsPerShot; i++ )
	{
		
		s32 CenterFactor = ( (abs(inttof32(i) - BpsDiv2) << 12) / BpsDiv2 ); //12
		s32 factor = t->Bp.Speed + ( (t->BulletSpeedDelta * CenterFactor) >> 12 );  // 12
		
		angle += (t->SpreadAngle * t->AngleDirection);		// 12
		s32 CosA = cosLerp(angle);
		s32 SinA = sinLerp(angle);
		
		
		Bp.x = t->Position->x + t->Xradius * CosA;;
		Bp.y = t->Position->y + t->Yradius * SinA;;
		Bp.Dx = (CosA * factor) >> 12;
		Bp.Dy = (SinA * factor) >> 12;
		Bp.AccelX = Bp.Dx / 2;
		Bp.AccelY = Bp.Dy / 2;
		Bp.Angle = angle;
		Bp.Rotation = angle;
		
		Bullets.Spawn( Bp, t->Target );
	
	}
	
				
	
}



//--------------------------------------
//	
//--------------------------------------
void ModeSpreadOffset( Turret *t, BulletArray &Bullets )
{

	sassert( t->Target, "Turret Target Undefined!!!" );
	sassert( t->Position, "Turret Position Undefined!!!" );
	
	
	Bullet::Params Bp;
	
	s32 BulletOffset = 1 << 12;
	if (t->WaveCounter & 1) BulletOffset = t->BulletSpreadOffset; 
	
	int SpreadRot = (t->SpreadAngle * BulletOffset) >> 12;
	s32 angle = t->Angle - mulf32((((t->BulletsPerShot + 1)<<12)/2), (SpreadRot * t->AngleDirection));
	
	s32 BulletDelta;
	
	BulletDelta = (t->Bp.Speed + (t->BulletSpeedDelta * (t->WaveCounter)));
    
	Bp.RotationDelta = t->Bp.RotationDelta;
	Bp.StopDelay = t->Bp.StopDelay;
	Bp.Frame = t->Bp.Frame;
	Bp.NumFrames = t->Bp.NumFrames;
	Bp.BaseFrame = t->Bp.BaseFrame;
	Bp.Smart = t->Bp.Smart;
	Bp.Width = t->Bp.Width;
	Bp.Height = t->Bp.Height;
	Bp.Type = t->Bp.Type;
	Bp.AniMode = t->Bp.AniMode;
	
	Bp.Gx = t->Bp.Gx;
	Bp.Gy = t->Bp.Gy;
	Bp.Speed = t->Bp.Speed;
		
	for( int i = 0; i < t->BulletsPerShot; i++ )
	{
		
		angle += (SpreadRot * t->AngleDirection);		// 12
		s32 CosA = cosLerp(angle);
		s32 SinA = sinLerp(angle);
		
		
		Bp.x = t->Position->x + t->Xradius * CosA;;
		Bp.y = t->Position->y + t->Yradius * SinA;;
		Bp.Dx = (CosA * BulletDelta) >> 12;
		Bp.Dy = (SinA * BulletDelta) >> 12;
		Bp.AccelX = Bp.Dx / 2;
		Bp.AccelY = Bp.Dy / 2;
		Bp.Angle = angle;
		Bp.Rotation = angle;
		
		Bullets.Spawn( Bp, t->Target );
	
	}
	
				
	
}


//--------------------------------------
//	
//--------------------------------------
void ModeSpreadRings( Turret *t, BulletArray &Bullets )
{

	sassert( t->Target, "Turret Target Undefined!!!" );
	sassert( t->Position, "Turret Position Undefined!!!" );
	
	
	Bullet::Params Bp;
	
	s32 BpsDiv2 = (t->BulletsPerShot << 12) / 2;
	
	s32 BulletOffset = 1 << 12;
	if (t->WaveCounter & 1) BulletOffset = t->BulletSpreadOffset; 
	
	
	
	int SpreadRot = (t->SpreadAngle * BulletOffset) >> 12;
	s32 angle = t->Angle - mulf32((((t->BulletsPerShot + 1)<<12)/2), (SpreadRot * t->AngleDirection));
	
	//s32 BulletDelta;
	
	//if (t->WaveCounter == 0)
    //    BulletDelta = (t->Bp.Speed + (t->BulletSpeedDelta * (t->MaxWave)));
    //else
    //    BulletDelta = (t->Bp.Speed + (t->BulletSpeedDelta * (t->WaveCounter-1)));
    
	
	Bp.RotationDelta = t->Bp.RotationDelta;
	Bp.StopDelay = t->Bp.StopDelay;
	Bp.Frame = t->Bp.Frame;
	Bp.NumFrames = t->Bp.NumFrames;
	Bp.BaseFrame = t->Bp.BaseFrame;
	Bp.Smart = t->Bp.Smart;
	Bp.Width = t->Bp.Width;
	Bp.Height = t->Bp.Height;
	Bp.Type = t->Bp.Type;
	Bp.AniMode = t->Bp.AniMode;
	
	Bp.Gx = t->Bp.Gx;
	Bp.Gy = t->Bp.Gy;
	Bp.Speed = t->Bp.Speed;
	
	
	s32 CosR = cosLerp(t->Angle);
	s32 SinR = sinLerp(t->Angle);
	
	for( int i = 0; i < t->BulletsPerShot; i++ )
	{
		
		s32 CenterFactor = ( (abs(inttof32(i) - BpsDiv2) << 12) / BpsDiv2 ); //12
		s32 factor = t->Bp.Speed + ( (t->BulletSpeedDelta * CenterFactor) >> 12 );  // 12
		
		angle += (t->SpreadAngle * t->AngleDirection);		// 12
		s32 CosA = cosLerp(angle);
		s32 SinA = sinLerp(angle);
		
		
		Bp.x = t->Position->x + t->Xradius * CosA;;
		Bp.y = t->Position->y + t->Yradius * SinA;;
		Bp.Dx = (CosR * factor) >> 12;
		Bp.Dy = (SinR * factor) >> 12;
		Bp.AccelX = Bp.Dx / 2;
		Bp.AccelY = Bp.Dy / 2;
		Bp.Angle = angle;
		Bp.Rotation = angle;
		
		Bullets.Spawn( Bp, t->Target );
	
	}
	
				
	
}





//------------------------------------------------------------------------------
//	Turret Array
//------------------------------------------------------------------------------
TurretArray::TurretArray()
{
	
}

TurretArray::~TurretArray()
{
	Turrets.clear();
}

//--------------------------------------
//	
//--------------------------------------
void TurretArray::Init( int Palette, glImage* const Sprites, int NumTurrets )
{
	
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Turrets.resize( NumTurrets );	 // Init array to maxTurrets   
	
}

//--------------------------------------
//	
//--------------------------------------
void TurretArray::UpdateEntities( BulletArray &Bullets )
{
	
	numEntities = 0;
	std::vector<Turret>::iterator iter;
	for( iter = Turrets.begin(); iter != Turrets.end(); ++iter )
	{
		if( iter->Activated )
		{
			iter->Update( Bullets );
			numEntities++;
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void TurretArray::KillEntities( )
{
	
	std::vector<Turret>::iterator iter;
	for( iter = Turrets.begin(); iter != Turrets.end(); ++iter )
	{
			iter->Kill();
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void TurretArray::Spawn( Turret::Params const &P, 
						 Vector2df32 *position, 
						 Vector2df32 *target
					   )
{

	std::vector<Turret>::iterator iter;
	for( iter = Turrets.begin(); iter != Turrets.end(); ++iter )
	{
		if( !iter->Active )		// insert a turret on a free slot
		{

			iter->Active = true;
			iter->Activated = true;
			iter->Init( P, position, target );
			break;
		
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void TurretArray::Spawn( Turret::Params const &P, 
						 s32 px, s32 py,
						 s32 tx, s32 ty 
					   )
{

	std::vector<Turret>::iterator iter;
	for( iter = Turrets.begin(); iter != Turrets.end(); ++iter )
	{
		if( !iter->Active )		// insert a turret on a free slot
		{

			iter->Active = true;
			iter->Activated = true;
			iter->Init( P, px, py, tx, ty );
			break;
		
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void TurretArray::Spawn( Turret::Params const &P, 
						 Vector2df32 *position, 
						 Vector2df32 *target,
						 std::vector<Turret>::iterator &TurretPtr
					   )
{
  
	std::vector<Turret>::iterator iter;
	for( iter = Turrets.begin(); iter != Turrets.end(); ++iter )
	{
		if( !iter->Active )		// insert a turret on a free slot
		{

			iter->Active = true;
			iter->Activated = true;
			iter->Init( P, position, target );
			TurretPtr = iter;
			break;
		
		}
	}
	
}

	
//--------------------------------------
//	
//--------------------------------------
void TurretArray::Spawn( Turret::Params const &P, 
						 s32 px, s32 py,
						 s32 tx, s32 ty, 
						 std::vector<Turret>::iterator &TurretPtr
					   )
{

	std::vector<Turret>::iterator iter;
	for( iter = Turrets.begin(); iter != Turrets.end(); ++iter )
	{
		if( !iter->Active )		// insert a turret on a free slot
		{

			iter->Active = true;
			iter->Activated = true;
			iter->Init( P, px, py, tx, ty );
			TurretPtr = iter;
			break;
		
		}
	}
	
}
	