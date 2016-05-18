/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	GluteusBoss class


*******************************************************************************
******************************************************************************/

#include "GluteusBoss.h"

namespace GluteusStuff
{
	
void LoadElasticSmartSingle( TurretArray &Turrets,
			         Vector2df32 *Position,
			         Vector2df32 *Target,
			         std::vector<Turret>::iterator &TurretPtr,
					 s32 angle );
void LoadWasagSingleLayer( TurretArray &Turrets,  
					 Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction );

void Add1WaySmart( Vector2df32 *Position,
			       Vector2df32 *Target,
			       std::vector<Turret>::iterator &TurretPtr );

void AddStraightSpreadOffseted(  Vector2df32 *Position,
						   Vector2df32 *Target,
					       std::vector<Turret>::iterator &TurretPtr,
						   int AngleDirection );

void AddSprayWaveDeltaStraight( Vector2df32 *Position,
						      Vector2df32 *Target,
					          std::vector<Turret>::iterator &TurretPtr,
							  int direction );

void AddDoubleIntersecting7Way(  Vector2df32 *Position,
			    Vector2df32 *Target,
				std::vector<Turret>::iterator &TurretPtr,
				int direction );

void AddIntersectingOffsetted( Vector2df32 *Position,
				        Vector2df32 *Target,
					    std::vector<Turret>::iterator &TurretPtr );
						
void AddProgearWhip(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr, int direction );

void AddSprayWaveDecel(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction );
					 
void AddStraight(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int Delay );

void AddSprayWaveJumping(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction );

}   // end namespace
//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::Spawn( TurretArray &Turrets )
{

	const Vector2df32 Offsets[3] =
	{
		{-40, -64 },{-42, 64 },{-8, 0 }	
	};
	
	x = 400 << 12;
	y = 96 << 12;
	Dx = -1024;
	Dy = 96 << 12;
	
	Frame = 0;
	Counter = 0;
	Active = true;
	Alive = true;
	Hp = 800 * 10;
	MaxHp = Hp;
	
	DeathScore = Hp * 10;
	

	Width = 77;
	Height = 89 * 2;
	
	
	LinkFrame = 0;
	
	DestroyedCounter = 60 * 5;
	
	LoadPath();
	
	// set up options
	Cannon cannon;
	
	cannon.x = x;
	cannon.y = y;
	cannon.Width = 32;
	cannon.Height = 32;
	
	cannon.TurretPosition.x = x;
	cannon.TurretPosition.y = y;
	cannon.TurretTarget.x = 0;
	cannon.TurretTarget.y = 96 << 12;
	
	// be safe
	Cannons.clear();
	
	// add arms
	Cannons.push_back(cannon);    // 0
	Cannons.push_back(cannon);    // 1
	Cannons.push_back(cannon);    // 2
	Cannons.push_back(cannon);    // 3
	
	// center
	Cannons.push_back(cannon);    // 4
	Cannons.push_back(cannon);    // 5
	
	
	
	// 1st, 0
	GluteusStuff::LoadElasticSmartSingle( Turrets,
			       &(Cannons[0].TurretPosition),
			       &(Cannons[0].TurretTarget),
			       Cannons[0].TurretPtr,
				   0 );
	GluteusStuff::LoadElasticSmartSingle( Turrets,
			       &(Cannons[1].TurretPosition),
			       &(Cannons[1].TurretTarget),
			       Cannons[1].TurretPtr,
				   BRAD_PI/2 );
	GluteusStuff::LoadElasticSmartSingle( Turrets,
			       &(Cannons[2].TurretPosition),
			       &(Cannons[2].TurretTarget),
			       Cannons[2].TurretPtr,
				   BRAD_PI/2*2 );
	
	// 1st, 1
	GluteusStuff::AddProgearWhip( &(Cannons[0].TurretPosition),
							 &(Cannons[0].TurretTarget),
							 Cannons[0].TurretPtr, 1 );
	GluteusStuff::AddProgearWhip( &(Cannons[1].TurretPosition),
							 &(Cannons[1].TurretTarget),
							 Cannons[1].TurretPtr, -1 );
	GluteusStuff::AddProgearWhip( &(Cannons[2].TurretPosition),
							 &(Cannons[2].TurretTarget),
							 Cannons[2].TurretPtr, 0 );
	

	// 1st, 2
	for( int i = 0; i < 3; i++)
	{
			GluteusStuff::AddStraight( &(Cannons[i].TurretPosition),
						 &(Cannons[i].TurretTarget),
			             Cannons[i].TurretPtr,
						 i * 10);
	}
	
	// 1st, 3
	GluteusStuff::AddSprayWaveJumping( &(Cannons[0].TurretPosition),
				 &(Cannons[0].TurretTarget),
				 Cannons[0].TurretPtr,
				 1);

	GluteusStuff::AddSprayWaveJumping( &(Cannons[1].TurretPosition),
				 &(Cannons[1].TurretTarget),
				 Cannons[1].TurretPtr,
				 -1);
	
	GluteusStuff::AddSprayWaveJumping( &(Cannons[2].TurretPosition),
				 &(Cannons[2].TurretTarget),
				 Cannons[2].TurretPtr,
				 0);
	
	//  ----- 2nd -----
	// 2nd, 0
	GluteusStuff::LoadWasagSingleLayer( Turrets,
					&(Cannons[3].TurretPosition),
					&(Cannons[3].TurretTarget),
					Cannons[3].TurretPtr,
					1 );
	GluteusStuff::LoadWasagSingleLayer( Turrets,
					&(Cannons[4].TurretPosition),
					&(Cannons[4].TurretTarget),
					Cannons[4].TurretPtr,
					-1 );
	
	// 2nd, 1
	GluteusStuff::AddSprayWaveDeltaStraight(  &(Cannons[3].TurretPosition),
						  &(Cannons[3].TurretTarget),
					      Cannons[3].TurretPtr, 1 );
	GluteusStuff::AddSprayWaveDeltaStraight(  &(Cannons[4].TurretPosition),
						  &(Cannons[4].TurretTarget),
					      Cannons[4].TurretPtr, -1 );
	
	// 2nd, 2
	GluteusStuff::AddDoubleIntersecting7Way( &(Cannons[3].TurretPosition),
					     &(Cannons[3].TurretTarget),
					     Cannons[3].TurretPtr, 1 );
	
	GluteusStuff::AddDoubleIntersecting7Way( &(Cannons[4].TurretPosition),
					     &(Cannons[4].TurretTarget),
					     Cannons[4].TurretPtr, -1 );
	
	// 2nd, 3
	GluteusStuff::AddIntersectingOffsetted( &(Cannons[3].TurretPosition),
						     &(Cannons[3].TurretTarget),
					         Cannons[3].TurretPtr );
	GluteusStuff::AddIntersectingOffsetted( &(Cannons[4].TurretPosition),
						     &(Cannons[4].TurretTarget),
					         Cannons[4].TurretPtr );
	
	// 2nd 4	
	GluteusStuff::AddStraightSpreadOffseted( &(Cannons[3].TurretPosition),
						     &(Cannons[3].TurretTarget),
					         Cannons[3].TurretPtr, 1 );
	GluteusStuff::AddStraightSpreadOffseted( &(Cannons[4].TurretPosition),
						     &(Cannons[4].TurretTarget),
					         Cannons[4].TurretPtr, -1 );
	
	// 2nd 5
	GluteusStuff::AddSprayWaveDecel( &(Cannons[3].TurretPosition),
				 &(Cannons[3].TurretTarget),
				 Cannons[3].TurretPtr,1 );
				 
	GluteusStuff::AddSprayWaveDecel( &(Cannons[4].TurretPosition),
				 &(Cannons[4].TurretTarget),
				 Cannons[4].TurretPtr,-1 );
	
	
	// 2nd 6
	GluteusStuff::AddProgearWhip( &(Cannons[3].TurretPosition),
				 &(Cannons[3].TurretTarget),
				 Cannons[3].TurretPtr,1 );
				 
	GluteusStuff::AddProgearWhip( &(Cannons[4].TurretPosition),
				 &(Cannons[4].TurretTarget),
				 Cannons[4].TurretPtr,-1 );
	
	
	// Deactivate
	Cannons[0].TurretPtr->Deactivate();
	Cannons[1].TurretPtr->Deactivate();
	Cannons[2].TurretPtr->Deactivate();
	Cannons[3].TurretPtr->Deactivate();
	
	// center
	Cannons[4].TurretPtr->Deactivate();
	Cannons[5].TurretPtr->Deactivate();
	
	
	for( int i = 0; i < 3; i++)
	{
		Parts[i].Width = Images[4].width;
		Parts[i].Height = Images[4].height;
		LoadPartPath( Parts[i], Offsets[i].x, Offsets[i].y, Offsets[i].x, Offsets[i].y, i );
	}
	
	
}


//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	//iprintf( "\x1b[1;1HTransSplineIndex = %i         ", Path.GetIndex() );
	//iprintf( "\x1b[2;1HPartIndex = %i         ", Parts[0].Path.GetIndex() );
	
	// Parts
	for( int i = 0; i < 3; i++ )
	{
		Parts[i].Angle = atan2Lerp( Vic.GetX() - Parts[i].x, Vic.GetY()- Parts[i].y );
		Cannons[i].x = Parts[i].x - (10<<12);
		Cannons[i].y = Parts[i].y;
		Cannons[i].TurretPosition.x = Cannons[i].x;
		Cannons[i].TurretPosition.y = Cannons[i].y;
		// move targets for kicks
		Cannons[i].TurretTarget.x = Vic.GetX();
		Cannons[i].TurretTarget.y = Vic.GetY();
		
		// Reset AABB
		Cannons[i].Aabb.Init( (Cannons[i].x >> 12) - (Cannons[i].Width >> 1), 
				   (Cannons[i].y >> 12) - (Cannons[i].Height >> 1), 
				   Width, Height );
		
	}
	// Hands/wings 
	for( int i = 3; i < 5; i++ )
	{
		Cannons[i].x = x;
		if( i == 3 )
			Cannons[i].y = y - inttof32(64);
		else
			Cannons[i].y = y + inttof32(64);
			
		Cannons[i].TurretPosition.x = Cannons[i].x;
		Cannons[i].TurretPosition.y = Cannons[i].y;
		// move targets for kicks
		Cannons[i].TurretTarget.x = Vic.GetX();
		Cannons[i].TurretTarget.y = Vic.GetY();
		
		// Reset AABB
		Cannons[i].Aabb.Init( (Cannons[i].x >> 12) - (Cannons[i].Width >> 1), 
				   (Cannons[i].y >> 12) - (Cannons[i].Height >> 1), 
				   Width, Height );
	
	}
	
	switch( Path.GetIndex() )
	{
		case 1:
			
			if( !Cannons[3].TurretPtr->IsActivated() )
			{
				Cannons[3].TurretPtr->Activate();
				Cannons[3].TurretPtr->DoPattern(0);
			}
			
			if( !Cannons[4].TurretPtr->IsActivated() )
			{
				Cannons[4].TurretPtr->Activate();
				Cannons[4].TurretPtr->DoPattern(0);
			}
			
			break;
		case 3:
			for( int i = 0; i < 3; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(0);
				}
			}
			
			break;
		case 4:
			
			break;
		case 5:
			break;
		case 6:
			for( int i = 0; i < 5; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 7:
			// Spray wave delta straight
			for( int i = 3; i < 5; i++ )
			{
				Cannons[i].TurretPtr->Activate();
				Cannons[i].TurretPtr->DoPattern(1);
			}
			for( int i = 0; i < 4; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(0);
				}
			}
			break;
		case 8:
			
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			for( int i = 0; i < 5; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 12:
			// 7 way intersecting
			for( int i = 3; i < 5; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(2);
				}
			}
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			for( int i = 3; i < 5; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			
			// Progear Whip
			for( int i = 0; i < 3; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(1);
				}
			}
			
			break;
		case 16:
			break;
		case 17:
			for( int i = 0; i < 3; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			// Intersecting offsetted
			for( int i = 3; i < 5; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(3);
				}
			}
			break;
		case 18:
			break;
		case 19:
			break;
		case 20:
			for( int i = 0; i < 5; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 21:
			// Use only two  turrets
			for( int i = 0; i < 2; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(3);
				}
			}
			break;
		case 22:
			break;
		case 23:
			for( int i = 0; i < 3; i++ )
			{
				Cannons[i].TurretPtr->Deactivate();
			}
			// Spraywave decel
			for( int i = 3; i < 5; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(5);
				}
			}
			break;
		case 24:
			break;
		case 25:
			break;
		case 26:
			for( int i = 3; i < 5; i++ )
			{
				Cannons[i].TurretPtr->Deactivate();
			}
			break;
		case 27:
			// activate Offseted
			for( int i = 3; i < 5; i++ )
			{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(4);
			}			
			for( int i = 0; i < 3; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 28:
			break;
		case 29:
			break;
		case 30:
			for( int i = 3; i < 5; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 31:
			// Progear whip
			Cannons[3].TurretPtr->Activate();
			Cannons[3].TurretPtr->DoPattern(6);
			Cannons[4].TurretPtr->Activate();
			Cannons[4].TurretPtr->DoPattern(6);
			break;
		case 32:
			
			for( int i = 0; i < 3; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(2);
				}
			}
			break;
		case 33:
			break;
		case 34:
			break;
		case 35:
			for( int i = 0; i < 5; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
		
	}

	if( Path.GetIndex() == 7 )
	{
		Path.SetPoint(0, 200, 96);
		Path.SetPoint(1, 200, 96);
		Path.SetPoint(3, 235, 40);
		Path.SetPoint(4, 235, 192-40);
		Path.SetPoint(5, 235, 30);
		Path.SetPoint(6, 235, 192-30);
	}
	
	//temp
	//for( int i = 0; i < 5; i++ )
	//{
	//if( Cannons[i].TurretPtr->IsActivated() )
	//	{
	//		Cannons[i].TurretPtr->Deactivate();
	//	}
	//}
	// end temp
	
	Path.Update();
	
	x = Path.GetX();
	y = Path.GetY();
	
	if( (Path.GetIndex() < 9) || (Path.GetIndex() > 25))
		AnimateLinks();
	else
		CloseLinks();
		
	
	
	Targets::Instance()->AddCoord( x, y );
	
		
	
	
	Frame = 0;
	if( ( Counter & 3 ) == 0 )
	{
		LinkFrame = (LinkFrame + 1) & 3;
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height );
			 
	//Parts
	for( int i = 0; i < 3; i++)
	{
		Parts[i].Aabb.Init( (Parts[i].x >> 12) - (Parts[i].Width /2 ), 
							(Parts[i].y >> 12) - (Parts[i].Height / 2 ), 
							Parts[i].Width, Parts[i].Height );
	
	}
		
	
	
}


//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::Draw()
{
	

	// front center Cannon
	glSpriteRotate( ( Parts[2].x >> 12 ) - qran_range(-1,1), 
					( Parts[2].y >> 12 ), 
					  Parts[2].Angle,
					  GL_FLIP_NONE, 
					  &Images[5] );
	
	if( (DrawMode == EN_WHITE) )
	{
		DrawMode = EN_NORMAL;
		glColor( RGB15(31,0,0) );
		
		
		
		// Main Body
		glSprite( (x >> 12) - Width/2, 
				  (y >> 12) - Height/2, 
				  GL_FLIP_NONE, 
				  &Images[0] );

		glSprite( (x >> 12) - Width/2, 
				  (y >> 12), 
				  GL_FLIP_V, 
				  &Images[0] );
		
		
		
		glColor( RGB15(31,31,31) );
	}
	else
	{
		
		
		// Main Body
		glSprite( (x >> 12) - Width/2, 
				  (y >> 12) - Height/2, 
				  GL_FLIP_NONE, 
				  &Images[0] );

		glSprite( (x >> 12) - Width/2, 
				  (y >> 12), 
				  GL_FLIP_V, 
				  &Images[0] );
		
		
		
	}
	
	// Top Left
	glSpriteRotate( ( Parts[0].x >> 12 ), 
					( Parts[0].y >> 12 ) + qran_range(-1,1), 
					  Parts[0].Angle,
					  GL_FLIP_NONE, 
					  &Images[4] );
	
	// Bottom Left
	glSpriteRotate( ( Parts[1].x >> 12 ), 
					( Parts[1].y >> 12 ) + qran_range(-1,1), 
					  Parts[1].Angle,
					  GL_FLIP_NONE, 
					  &Images[4] );
		
	// exhaust
	if( (Counter & 3 ) == 0 )
	{
		// Top
		Explosion::Instance()->Spawn( (x >> 12) + 32,
								      (y >> 12) - 68,
								  1 << 12,
								  floattof32(-0.3),
								  Explode::SMALL_4
								);
		// Bottom
		Explosion::Instance()->Spawn( (x >> 12) + 32,
								      (y >> 12) + 68,
								  1 << 12,
								  floattof32(0.3),
								  Explode::SMALL_4
								);
	    // Center
		Explosion::Instance()->Spawn( (x >> 12) + 42,
								      (y >> 12),
								  1 << 12,
								  qran_range(-4096,4096),
								  Explode::SMALL_3
								);
	}
	
	DrawHp( Counter );
	
	
}


//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::DrawAABB( int color ) const 
{

	
	AABB aabb = Aabb.Get();
	glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
	aabb.Resize(512*3);
	glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
	
	//Parts
	for( int i = 0; i < 3; i++)
	{
		aabb = Parts[i].Aabb.Get();
		glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
		aabb.Resize(512*3);
		glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
		
	}
	
}


//--------------------------------------
//	
//--------------------------------------
EntityContainer::CollisionPacket GluteusBoss::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed )
{
	EntityContainer::CollisionPacket Ret;
	Ret.Collision = 0;
	AABB Paabb;
	
	if( Active )
	{
			
		AABB aabb = Aabb.Get();
		aabb.Resize(512*3);
		
		if( !Vic.IsInvincible() )
		{
			if( !BombIsActive )
			{
				if( Vic.GetAABB().Intersects(aabb) )
				{
					Ret.x = Vic.GetX() >> 12;
					Ret.y = Vic.GetY() >> 12;
					Ret.Collision = 2;
					Vic.Kill();
				}
				
				//Parts
				for( int i = 0; i < 3; i++)
				{
					Paabb = Parts[i].Aabb.Get();
					Paabb.Resize(512*3);
					if( Vic.GetAABB().Intersects(Paabb) )
					{
						Ret.x = Vic.GetX() >> 12;
						Ret.y = Vic.GetY() >> 12;
						Ret.Collision = 2;
						Vic.Kill();
					}
				
				}
	
			}
		}
		
		// Viper shots
		int Collisions = 0;
		if( x < ( 256 << 12 ) )
		{
			Collisions = Vic.CollideShots(aabb);
			if( Collisions )
			{
				DrawMode = EN_WHITE;
				// Dec HP
				Hp -= Collisions;
				if( Hp < 0 )
				{
					Destroy();
					Vic.AddToScore( DeathScore );
					IsDestroyed = true;
				}
				
			}
			
			//Parts
			for( int i = 0; i < 3; i++)
			{
				Paabb = Parts[i].Aabb.Get();
				Paabb.Resize(512*3);
				Collisions = Vic.CollideShots(Paabb);
				//if( Collisions )
				//{
					//DrawMode = EN_WHITE;
					// Dec HP
					//Hp -= Collisions;
					//if( Hp < 0 )
					//{
					//	Destroy();
					//	Vic.AddToScore( DeathScore );
					//}
					
				//}
			}
				
			
			// Viper Missiles
			Collisions = Vic.CollideMissiles(aabb);
			if( Collisions )
			{
				DrawMode = EN_WHITE;
				// Dec HP
				Hp -= Collisions;
				if( Hp < 0 )
				{
					Destroy();
					Vic.AddToScore( DeathScore );
					IsDestroyed = true;
				}
				
			}
			
			//Parts
			for( int i = 0; i < 3; i++)
			{
				Paabb = Parts[i].Aabb.Get();
				Paabb.Resize(512*3);
				Collisions = Vic.CollideMissiles(Paabb);
				//if( Collisions )
				//{
				//	DrawMode = EN_WHITE;
				//	// Dec HP
				//	Hp -= Collisions;
				//	if( Hp < 0 )
				//	{
				//		Destroy();
				//		Vic.AddToScore( DeathScore );
				//		IsDestroyed = true;
				//	}
				//	
				//}
			}
			
			
			
		}  // end if
	}

	return Ret;
	
}


//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::ForceDestruction()
{
	Destroy();
}
	
//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::LoadPath()
{
	Path.Init( 40 );
	
	Spline::Coord p;
	
	// start
	p.x = 400;
	p.y = 96;
	
	// we should use 2 points
	Path.AddPoint( p );  // 1
	Path.AddPoint( p );  // 2
	
	p.x = 230;
	p.y = 96;
	Path.AddPoint( p );  // 3
 	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 4
	
	p.x = 230;
	p.y = 96;
	Path.AddPoint( p );  // 5
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 6
	
	p.x = 200;
	p.y = 40;
	Path.AddPoint( p );  // 7
	
	p.x = 230;
	p.y = 192-40;
	Path.AddPoint( p );  // 8
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 9
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 10
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 11
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 12
	
	p.x = 230;
	p.y = 40;
	Path.AddPoint( p );  // 13
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 14
	
	p.x = 200;
	p.y = 20;
	Path.AddPoint( p );  // 15
	
	p.x = 210;
	p.y = 172;
	Path.AddPoint( p );  // 16
	
	p.x = 220;
	p.y = 96;
	Path.AddPoint( p );  // 17
	
	p.x = 220;
	p.y = 96;
	Path.AddPoint( p );  // 18
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 19
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 20
	
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 21
	
	p.x = 220;
	p.y = 96;
	Path.AddPoint( p );  // 22
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 23
	
	p.x = 230;
	p.y = 96;
	Path.AddPoint( p );  // 24
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 25
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 26
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 27
	
	p.x = 220;
	p.y = 50;
	Path.AddPoint( p );  // 28
	
	p.x = 200;
	p.y = 100;
	Path.AddPoint( p );  // 29
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 30
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 31
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 32
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 33
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 34
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 35
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 36
	
	Path.SetSpeed(24);
		
	Path.SetT(0);
	
}

//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::Destroy()
{
	
	for( int i = 0; i < 3; i++)
	{
		Explosion::Instance()->Spawn( Parts[i].x >> 12,
									  Parts[i].y >> 12,
									  0,
									  0,
									  Explode::MEDIUM_5,
									  Explode::SMALL_1 );
			
	}
	
	
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  0,
								  0,
								  Explode::MEDIUM_4,
								  Explode::MEDIUM_2
								);
	
	
	Explosion::Instance()->MegaSpawn(x >> 12,y >> 12,80,Explode::TINY,Explode::MEDIUM_5, 60*4 );
	
		
	Alive = false;
	
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_BIG );
	
}



//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::BombMe( PlayerShip &Vic, int BombDamageValue )
{
	
	if( Active ) 
	{
		Hp -= BombDamageValue;
		if( Hp < 0 )
		{
			Destroy();
			Vic.AddToScore( DeathScore );
		}
		
	}
	
}




//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::LoadPartPath( Part &Prt, int _sx, int _sy, int _ex, int _ey, int CannonID )
{
	Prt.Path.Init( 16 );
	
	Spline::Coord p;
	
	
	int XMultiplier = 1;
	int YMultiplier = 1;
	switch( CannonID )
	{
		case 0:
			XMultiplier = 1;
			YMultiplier = 1;
			break;
		case 1:
			XMultiplier = 1;
			YMultiplier = -1;
			break;
		case 2:
			XMultiplier = 0;
			YMultiplier = 0;
			break;
	}
	// start
	p.x = _sx;
	p.y = _sy;
	
	// we should use 2 points
	Prt.Path.AddPoint( p );
	Prt.Path.AddPoint( p );
	
	// randomize for now
	for (int i=0; i < 10; i++)
	{
		int xx = cosLerp( i * 8500 )* 20;
		int yy = sinLerp( i * 8500 )* 90;
		p.x = -30 + ((xx * XMultiplier) >> 12);
		p.y = (yy * YMultiplier) >> 12;
		Prt.Path.AddPoint( p );
	}
	
	// close last 2 coords
	p.x = _ex;
	p.y = _ey;
	
	// we should use 2 points
	Prt.Path.AddPoint( p );
	Prt.Path.AddPoint( p );
	
	Prt.Path.SetSpeed(64);
	Prt.Path.SetT(0);
	
}

//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::AnimateLinks()
{
	
	for( int i = 0; i < 3; i++)
	{
		Parts[i].Path.Update();
		Parts[i].x = Parts[i].Path.GetX() + x;
		Parts[i].y = Parts[i].Path.GetY() + y;
	
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void GluteusBoss::CloseLinks()
{
	for( int i = 0; i < 3; i++)
	{
		if( Parts[i].Path.GetIndex() != 0 )
		{
			Parts[i].Path.Update();
		}
		Parts[i].x = Parts[i].Path.GetX() + x;
		Parts[i].y = Parts[i].Path.GetY() + y;
	
	}
}




//------------------------------------------------------------------------------
//	Private funks
//------------------------------------------------------------------------------

namespace GluteusStuff
{

// ------------- 1st cannon -------------------------
void LoadElasticSmartSingle( TurretArray &Turrets,
			         Vector2df32 *Position,
			         Vector2df32 *Target,
			         std::vector<Turret>::iterator &TurretPtr,
					 s32 angle )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1<<24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= true;					// if targeting or not
	Tp.Xradius			= 0;				// 
	Tp.Yradius			= 0;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= 0;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 2;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 180;			// 
	Tp.BulletsPerShot	= 1;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 2;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 320;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SINGLE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = 384;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 28;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	Turrets.Spawn( Tp, 
				   Position, 
				   Target,
				   TurretPtr );


}


void LoadWasagSingleLayer( TurretArray &Turrets,  
					 Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 12;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 5;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 3776;				// + rotation speed
	Tp.SpreadAngle		= 6656;			// distance of each bullet on a spread
	Tp.AngleDirection	= direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 2;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 24;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 320;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_NORMAL;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = 384;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 11;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 12;
	Tp.Bp.Height = 12;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	Turrets.Spawn( Tp, 
				   Position, 
				   Target,
				   TurretPtr );
	
}

void Add1WaySmart( Vector2df32 *Position,
			       Vector2df32 *Target,
			       std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= true;					// if targeting or not
	Tp.Xradius			= 0;				// 
	Tp.Yradius			= 0;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= 512;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 2;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 140;			// 
	Tp.BulletsPerShot	= 1;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 6;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 360;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_NORMAL;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(1.5)*2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 31;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	TurretPtr->AddPattern( Tp );
	
}

void AddStraightSpreadOffseted(  Vector2df32 *Position,
						   Vector2df32 *Target,
					       std::vector<Turret>::iterator &TurretPtr,
						   int AngleDirection )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 20;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/12;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 5;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 5;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(0.5);		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(2.5);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPREAD_OFFSET;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)*2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 1;
	Tp.Bp.BaseFrame = 15;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	TurretPtr->AddPattern( Tp );
	
}

void AddSprayWaveDeltaStraight( Vector2df32 *Position,
						      Vector2df32 *Target,
					          std::vector<Turret>::iterator &TurretPtr,
							  int direction )
{
	Turret::Params Tp;

	Tp.ActiveTime		= 1 << 12;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= BRAD_PI-1000;				// 
	Tp.MaxAngle			= BRAD_PI+1000;
	Tp.AngleDelta		= 512;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/4;			// distance of each bullet on a spread
	Tp.AngleDirection	= direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 3;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 7;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= -300;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)/8;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 0;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 7;
	Tp.Bp.Height = 7;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	TurretPtr->AddPattern( Tp );
	
}

// two turrets
void AddDoubleIntersecting7Way(  Vector2df32 *Position,
			    Vector2df32 *Target,
				std::vector<Turret>::iterator &TurretPtr,
				int direction )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 12;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= true;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/8;			// distance of each bullet on a spread
	Tp.AngleDirection	= direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 7;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 5;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 5;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_CENTERED;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(1.5) * 2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 34;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	TurretPtr->AddPattern( Tp );
	

}


void AddIntersectingOffsetted( Vector2df32 *Position,
				        Vector2df32 *Target,
					    std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= true;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 20;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/8;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 5;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 8;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(-0.15);;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(-0.45);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPREAD_OFFSET;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)*4;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 20;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 12;
	Tp.Bp.Height = 12;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
		
	
	TurretPtr->AddPattern( Tp );
	

}


void AddProgearWhip(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr, int direction )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 8;				// 
	Tp.Yradius			= 24;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= BRAD_2PI/12;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/12;			// distance of each bullet on a spread
	Tp.AngleDirection	= direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 40;			// 
	Tp.BulletsPerShot	= 2;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 12;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1) * 4;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 28;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::DECEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	TurretPtr->AddPattern( Tp );
	

}


void AddSprayWaveDecel(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 12;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= -20;				// 
	Tp.Yradius			= -20;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 512;				// + rotation speed
	Tp.SpreadAngle		= BRAD_2PI/12;			// distance of each bullet on a spread
	Tp.AngleDirection	= direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 12;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 7;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 5;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(0.75)*3;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 31;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::DECEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	TurretPtr->AddPattern( Tp );
	

}

void AddStraight(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int Delay )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 40;				// 
	Tp.Yradius			= 60;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 800;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/12;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= Delay;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 1;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 5;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SINGLE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1) / 2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 5;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	TurretPtr->AddPattern( Tp );
	

}


void AddSprayWaveJumping(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= (BRAD_PI/8)*9;				// 
	Tp.MaxAngle			= BRAD_2PI-(BRAD_PI/8)*9;
	Tp.AngleDelta		= 128;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/4;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 4;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 30;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(0);		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(1);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)*4;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 34;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	TurretPtr->AddPattern( Tp );
	

}

}  // end namespace