/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	TentakillBoss class


*******************************************************************************
******************************************************************************/

#include "TentakillBoss.h"



//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::Init( int Palette, glImage* const Sprites,
						  int Palette2, glImage* const Sprites2,
						  int _id )
{
	Cannons.clear();
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	ColorTable2 = Palette2;		// init needed data for drawing
	Images2 = Sprites2;
	ID = _id;
	
}

//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::Spawn( TurretArray &Turrets )
{

	x = 400 << 12;
	y = 96 << 12;
	Dx = -1024;
	Dy = 96 << 12;
	
	Frame = 0;
	Counter = 0;
	Active = true;
	Alive = true;
	Hp = 700 * 5;
	MaxHp = Hp;
	DeathScore = MaxHp * 10 * 2;
	

	Width = 96;
	Height = 64;
	
	BaseRotation = 0;
	ArmRotation = 0;
	RotationDelta = 32;
	ArmFrame = 0;
	
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
	Load2WayRotate( Turrets,
			       &(Cannons[0].TurretPosition),
			       &(Cannons[0].TurretTarget),
			       Cannons[0].TurretPtr,
				   0 );
	Load2WayRotate( Turrets,
			       &(Cannons[1].TurretPosition),
			       &(Cannons[1].TurretTarget),
			       Cannons[1].TurretPtr,
				   BRAD_PI/2 );
	Load2WayRotate( Turrets,
			       &(Cannons[2].TurretPosition),
			       &(Cannons[2].TurretTarget),
			       Cannons[2].TurretPtr,
				   BRAD_PI/2*2 );
	Load2WayRotate( Turrets,
			       &(Cannons[3].TurretPosition),
			       &(Cannons[3].TurretTarget),
			       Cannons[3].TurretPtr,
				   BRAD_PI/2*3 );
	
	// 1st, 1
	for( int i = 0; i < 4; i++)
	{
			Add4WaySmart( &(Cannons[i].TurretPosition),
						  &(Cannons[i].TurretTarget),
			              Cannons[i].TurretPtr );
	}
		
	// 1st, 2
	for( int i = 0; i < 4; i++)
	{
			AddStraightLasers( &(Cannons[i].TurretPosition),
						       &(Cannons[i].TurretTarget),
			                   Cannons[i].TurretPtr,
							   i * 10);
	}
	
	//1st, 3
	for( int i = 0; i < 4; i++)
	{
		AddRings( &(Cannons[i].TurretPosition),
				  &(Cannons[i].TurretTarget),
				  Cannons[i].TurretPtr );
	}
	
	
	//  ----- 2nd -----
	// 2nd, 0
	LoadSelfTargetKetsuiSprayTrafalgar( Turrets,
									    &(Cannons[4].TurretPosition),
									    &(Cannons[4].TurretTarget),
									    Cannons[4].TurretPtr );
	LoadSelfTargetKetsuiSprayTrafalgar( Turrets,
									    &(Cannons[5].TurretPosition),
									    &(Cannons[5].TurretTarget),
									    Cannons[5].TurretPtr );
	
	// 2nd, 1
	AddPsikyoSprayWave3(  &(Cannons[4].TurretPosition),
						  &(Cannons[4].TurretTarget),
					      Cannons[4].TurretPtr,
						  1 );
	AddPsikyoSprayWave3(  &(Cannons[5].TurretPosition),
						  &(Cannons[5].TurretTarget),
					      Cannons[5].TurretPtr,
						  -1 );
	
	// 2nd, 2
	AddSprayWaveDelta3Waves( &(Cannons[4].TurretPosition),
						     &(Cannons[4].TurretTarget),
					         Cannons[4].TurretPtr );
	AddRings( &(Cannons[5].TurretPosition),
			  &(Cannons[5].TurretTarget),
			  Cannons[5].TurretPtr );
	
	AddPsikyoSprayW5Decel( &(Cannons[4].TurretPosition),
						     &(Cannons[4].TurretTarget),
					         Cannons[4].TurretPtr );
	AddPsikyoSprayW5Decel( &(Cannons[5].TurretPosition),
						     &(Cannons[5].TurretTarget),
					         Cannons[5].TurretPtr );
	
	
	// Deactivate
	Cannons[0].TurretPtr->Deactivate();
	Cannons[1].TurretPtr->Deactivate();
	Cannons[2].TurretPtr->Deactivate();
	Cannons[3].TurretPtr->Deactivate();
	
	// center
	Cannons[4].TurretPtr->Deactivate();
	Cannons[5].TurretPtr->Deactivate();
	
	
}


//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	//iprintf( "\x1b[1;1HSplineIndex = %i         ", Path.GetIndex() );
	//iprintf( "\x1b[2;1HTurretIndex = %i         ", Cannons[0].TurretPtr->GetIndex() );
	
	// Update options
	// arm cannons
	for( int i = 0; i < 4; i++ )
	{
		Cannons[i].x = Arms[TENTAKILL_MAX_SEGMENTS-1][i].x + x;
		Cannons[i].y = Arms[TENTAKILL_MAX_SEGMENTS-1][i].y + y;
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
	
	
	// center
	for( int i = 4; i < 6; i++ )
	{
		Cannons[i].x = x;
		Cannons[i].y = y;
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
		case 2:
			for( int i = 0; i < 4; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(0);
				}
			}
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			for( int i = 0; i < 4; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			
			break;
		case 6:
			break;
		case 7:
			// Ketsui self target
			Cannons[4].TurretTarget.x = Cannons[4].x;
			Cannons[4].TurretTarget.y = Cannons[4].y;
		
			if( !Cannons[4].TurretPtr->IsActivated() )
			{
				Cannons[4].TurretPtr->Activate();
				Cannons[4].TurretPtr->DoPattern(0);
			}
			break;
		case 8:
			// Ketsui self target
			Cannons[4].TurretTarget.x = Cannons[4].x;
			Cannons[4].TurretTarget.y = Cannons[4].y;
		break;
		case 9:
			// Ketsui self target
			Cannons[4].TurretTarget.x = Cannons[4].x;
			Cannons[4].TurretTarget.y = Cannons[4].y;
		break;
		case 10:
			// Ketsui self target
			Cannons[4].TurretTarget.x = Cannons[4].x;
			Cannons[4].TurretTarget.y = Cannons[4].y;
			//deactivate ketsui
			for( int i = 4; i < 6; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 11:
			
			// activate "feinter"
			for( int i = 0; i < 4; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(1);
				}
			}
			break;
		case 12:
			break;
		case 13:
			
			break;
		case 14:
			for( int i = 0; i < 4; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 15:
			// Activate psikyo 3 wave double 
			Cannons[4].y = y - (60 << 12);
			Cannons[5].y = y + (60 << 12);
			
			for( int i = 4; i < 6; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
		
			for( int i = 4; i < 6; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(1);
				}
			}
			
			break;
		case 16:
			// Activate psikyo 3 wave double 
			Cannons[4].y = y - (60 << 12);
			Cannons[5].y = y + (60 << 12);
			
			for( int i = 4; i < 6; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
		break;
		case 17:
			// Activate psikyo 3 wave double 
			Cannons[4].y = y - (30 << 12);
			Cannons[5].y = y + (30 << 12);
			
			for( int i = 4; i < 6; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
		break;
		case 18:
			for( int i = 4; i < 6; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 19:
			break;
		case 20:
			for( int i = 4; i < 6; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(2);
				}
			}
			break;
		case 21:
			break;
		case 22:
			break;
		case 23:
			for( int i = 4; i < 6; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 24:
			break;
		case 25:
			// wave
			if( !Cannons[4].TurretPtr->IsActivated() )
			{
				Cannons[4].TurretPtr->Activate();
				Cannons[4].TurretPtr->DoPattern(3);
			}
			break;
		case 26:
			break;
		case 27:
			for( int i = 0; i < 6; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 28:
			for( int i = 0; i < 4; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
				}
			}
			// activate "Straight rings"
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].TurretPtr->DoPattern(3);
			}
			break;
		case 29:
			break;
		case 30:
			// activate "Feint"
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].TurretPtr->DoPattern(1);
			}
			break;
		case 31:
			break;
		case 32:
			for( int i = 0; i < 4; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 33:
			// activate "Straight lasers"
			for( int i = 0; i < 4; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(2);
				}
			}
			break;
		case 34:
			break;
		case 35:
			for( int i = 0; i < 6; i++ )
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
	
	
	Path.Update();
	
	x = Path.GetX();
	y = Path.GetY();
	
	
	Targets::Instance()->AddCoord( x, y );
	
		
	
	
	// animate
	if( ( Counter & 7 ) == 0 )
	{
		Frame++;
		if( Frame > 2 ) Frame = 0;
	}
	if( ( Counter & 3 ) == 0 )
	{
		ArmFrame++;
		if( ArmFrame > 2 ) ArmFrame = 0;
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height );
			 
	UpdateArms();
	
	
}


//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::Draw()
{
	int DrawFrame = Frame+2;
	
	DrawArms();
	if( (DrawMode == EN_WHITE) )
	{
		DrawMode = EN_NORMAL;
		glColor( RGB15(31,0,0) );
		// top
		// center
		glSprite( (x >> 12) - 16, 
				  (y >> 12) - 32, 
				  GL_FLIP_NONE, &Images[DrawFrame] );
		// left
		glSprite( ((x >> 12) - 16) - 32, 
				  (y >> 12) - 32, 
				  GL_FLIP_NONE, &Images[0] );
		// right
		glSprite( ((x >> 12) - 16) + 32, 
				  (y >> 12) - 32, 
				  GL_FLIP_NONE, &Images[1] );
		
		// bottom
		// center
		glSprite( (x >> 12) - 16, 
				  (y >> 12) + 0, 
				  GL_FLIP_V, &Images[DrawFrame] );
		// left
		glSprite( ((x >> 12) - 16) - 32, 
				  (y >> 12) + 0, 
				  GL_FLIP_V, &Images[0] );
		// right
		glSprite( ((x >> 12) - 16) + 32, 
				  (y >> 12) + 0, 
				  GL_FLIP_V, &Images[1] );
		glColor( RGB15(31,31,31) );
	}
	else
	{
		// top
		// center
		glSprite( (x >> 12) - 16, 
				  (y >> 12) - 32, 
				  GL_FLIP_NONE, &Images[DrawFrame] );
		// left
		glSprite( ((x >> 12) - 16) - 32, 
				  (y >> 12) - 32, 
				  GL_FLIP_NONE, &Images[0] );
		// right
		glSprite( ((x >> 12) - 16) + 32, 
				  (y >> 12) - 32, 
				  GL_FLIP_NONE, &Images[1] );
		
		// bottom
		// center
		glSprite( (x >> 12) - 16, 
				  (y >> 12) + 0, 
				  GL_FLIP_V, &Images[DrawFrame] );
		// left
		glSprite( ((x >> 12) - 16) - 32, 
				  (y >> 12) + 0, 
				  GL_FLIP_V, &Images[0] );
		// right
		glSprite( ((x >> 12) - 16) + 32, 
				  (y >> 12) + 0, 
				  GL_FLIP_V, &Images[1] );
		
		
	}
	

	DrawHp( Counter );
	
}


//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::DrawAABB( int color ) const 
{

	for( int j = 0; j < 4; j++)
	{
		for( int i = 0; i < TENTAKILL_MAX_SEGMENTS; i++ )
		{
			AABB aabb = Arms[i][j].Aabb.Get();
			glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
		}
	}
	
	AABB aabb = Aabb.Get();
	glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
	aabb.Resize(512*3);
	glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
			
}


//--------------------------------------
//	
//--------------------------------------
EntityContainer::CollisionPacket TentakillBoss::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed )
{
	EntityContainer::CollisionPacket Ret;
	Ret.Collision = 0;
	
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
				
				for( int j = 0; j < 4; j++)
				{
					for( int i = 0; i < TENTAKILL_MAX_SEGMENTS; i++ )
					{
						if( Vic.GetAABB().Intersects(Arms[i][j].Aabb) )
						{
							Ret.x = Vic.GetX() >> 12;
							Ret.y = Vic.GetY() >> 12;
							Ret.Collision = 2;
							Vic.Kill();
						}
					} // next i
				} // next j
				
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
			
			// Arms
			for( int j = 0; j < 4; j++)
			{
				for( int i = 0; i < TENTAKILL_MAX_SEGMENTS; i++ )
				{
					Vic.CollideShots(Arms[i][j].Aabb);
				} // next i
			} // next j
				
			
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
			
			// Arms
			for( int j = 0; j < 4; j++)
			{
				for( int i = 0; i < TENTAKILL_MAX_SEGMENTS; i++ )
				{
					Vic.CollideMissiles(Arms[i][j].Aabb);
				} // next i
			} // next j
			
		}  // end if
	}

	return Ret;
	
}


//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::ForceDestruction()
{
	Destroy();
}
	
//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::LoadPath()
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
	
	p.x = 30;
	p.y = 96;
	Path.AddPoint( p );  // 14
	
	p.x = 128;
	p.y = 20;
	Path.AddPoint( p );  // 15
	
	p.x = 128;
	p.y = 172;
	Path.AddPoint( p );  // 16
	
	p.x = 220;
	p.y = 96;
	Path.AddPoint( p );  // 17
	
	p.x = 220;
	p.y = 96;
	Path.AddPoint( p );  // 18
	
	p.x = 230;
	p.y = 96;
	Path.AddPoint( p );  // 19
	
	p.x = 230;
	p.y = 96;
	Path.AddPoint( p );  // 20
	
	
	p.x = 230;
	p.y = 96;
	Path.AddPoint( p );  // 21
	
	p.x = 230;
	p.y = 96;
	Path.AddPoint( p );  // 22
	
	p.x = 200;
	p.y = 192-40;
	Path.AddPoint( p );  // 23
	
	p.x = 230;
	p.y = 40;
	Path.AddPoint( p );  // 24
	
	p.x = 170;
	p.y = 96;
	Path.AddPoint( p );  // 25
	
	p.x = 170;
	p.y = 96;
	Path.AddPoint( p );  // 26
	
	p.x = 170;
	p.y = 96;
	Path.AddPoint( p );  // 27
	
	p.x = 230;
	p.y = 50;
	Path.AddPoint( p );  // 28
	
	p.x = 230;
	p.y = 100;
	Path.AddPoint( p );  // 29
	
	p.x = 200;
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
void TentakillBoss::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  0,
								  0,
								  Explode::MEDIUM_4,
								  Explode::MEDIUM_2
								);
	
	for( int j = 0; j < 4; j++)
	{
		for( int i = 0; i < TENTAKILL_MAX_SEGMENTS; i++ )
		{
			int ax = (Arms[i][j].x + x) >> 12;
			int ay = (Arms[i][j].y + y) >> 12;
			Explosion::Instance()->Spawn( ax,
								  ay,
								  0,
								  0,
								  Explode::SMALL_2 );
	
	
		}
		
	}
	
	
	Explosion::Instance()->MegaSpawn(x >> 12,y >> 12,90,Explode::TINY,Explode::MEDIUM_5, 60*4 );
	
	Alive = false;
	
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_BIG );
	
}


//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::UpdateArms()
{
	s32 BaseDelta = 256;
	Matrix2D M1, M2, M3;

	switch( Path.GetIndex() )
	{
		case 5:
			OpenArms();
			BaseDelta = 64;
			break;
		case 6:
			OpenArms();
			BaseDelta = 64;
			break;
		case 7:
			OpenArms();
			BaseDelta = 64;
			break;
		case 8:
			OpenArms();
			BaseDelta = 128;
			break;
		case 9:
			OpenArms();
			BaseDelta = 128;
			break;
		case 10:
			CloseArms();
			BaseDelta = 64;
			break;
		case 11:
			CloseArms();
			BaseDelta = 128;
			break;
		case 12:
			CloseArms();
			BaseDelta = 256;
			break;
		case 13:
			CloseArms();
			BaseDelta = 400;
			break;
		case 14:
			CloseArms();
			BaseDelta = 400;
			break;
		case 19:
			OpenArms();
			BaseDelta = -64;
			break;
		case 20:
			OpenArms();
			BaseDelta = -64;
			break;
		case 21:
			OpenArms();
			BaseDelta = -64;
			break;
		case 22:
			OpenArms();
			BaseDelta = -64;
			break;
		case 23:
			OpenArms();
			BaseDelta = -64;
			break;
		case 24:
			OpenArms();
			BaseDelta = -64;
			break;
		case 25:
			OpenArms();
			BaseDelta = -64;
			break;
		case 26:
			OpenArms();
			BaseDelta = -64;
			break;
		case 27:
			OpenArms();
			BaseDelta = -64;
			break;
		case 28:
			OpenArms();
			BaseDelta = -64;
			break;
		case 29:
			OpenArms();
			BaseDelta = -64;
			break;
		case 30:
			OpenArms();
			BaseDelta = -64;
			break;
		case 31:
			OpenArms();
			BaseDelta = +64;
			break;
		case 32:
			OpenArms();
			BaseDelta = +64;
			break;
		case 33:
			OpenArms();
			BaseDelta = +64;
			break;
		case 34:
			OpenArms();
			BaseDelta = +64;
			break;
		case 35:
			OpenArms();
			BaseDelta = +64;
			break;
		default:
			AnimateArms();
			BaseDelta = 256;
			
	
	}
	
	BaseRotation += BaseDelta;
	
	M1.LoadIndentity();
	M2.LoadIndentity();
	
	M3.LoadRotation( BaseRotation );
		
	Vector2df32 in;
	in.x = 0;
	in.y = 0;
	for( int i = 0; i < TENTAKILL_MAX_SEGMENTS; i++ )
	{
		M1.Rotate( ArmRotation );
		M1.Translate( 12 << 12, 0 );
		M2.Rotate( ArmRotation );
		M2.Translate( 0, 12 << 12 );
		
		Vector2df32 P1 = M1.TransformPoint( in );
		Vector2df32 P2 = M2.TransformPoint( in );
		
		Vector2df32 O1 = M3.TransformPoint( P1 );
		Vector2df32 O2 = M3.TransformPoint( P2 );
		
		Arms[i][0].x = O1.x;
		Arms[i][0].y = O1.y;
		Arms[i][0].Angle = ArmRotation*i;
		Arms[i][0].Aabb.Init( ((Arms[i][0].x + x) >> 12) - (5), 
							  ((Arms[i][0].y + y) >> 12) - (5), 
							  10, 10 );
	
		Arms[i][1].x = O2.x;
		Arms[i][1].y = O2.y;
		Arms[i][1].Angle = ArmRotation*i;
		Arms[i][1].Aabb.Init( ((Arms[i][1].x + x) >> 12) - (5), 
							  ((Arms[i][1].y + y) >> 12) - (5), 
							  10, 10 );
	
		Arms[i][2].x = -O1.x;
		Arms[i][2].y = -O1.y;
		Arms[i][2].Angle = ArmRotation*i;
		Arms[i][2].Aabb.Init( ((Arms[i][2].x + x) >> 12) - (5), 
							  ((Arms[i][2].y + y) >> 12) - (5), 
							  10, 10 );
	
		Arms[i][3].x = -O2.x;
		Arms[i][3].y = -O2.y;
		Arms[i][3].Angle = ArmRotation*i;
		Arms[i][3].Aabb.Init( ((Arms[i][3].x + x) >> 12) - (5), 
							  ((Arms[i][3].y + y) >> 12) - (5), 
							  10, 10 );
	
		
	}
			
}

//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::DrawArms()
{
	
	glColor( RGB15(31,31,31) );
	for( int j = 0; j < 4; j++)
	{
		int f = ArmFrame;
		for( int i = 0; i < TENTAKILL_MAX_SEGMENTS; i++ )
		{
			int ax = (Arms[i][j].x + x) >> 12;
			int ay = (Arms[i][j].y + y) >> 12;
			f++;
			if( f > 2) f = 0;
			s32 angle = j * BRAD_PI/2;
			glSpriteRotate( ax, ay, BaseRotation + angle + Arms[i][j].Angle, GL_FLIP_NONE, &Images2[34+f]);
		}
		
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::CloseArms()
{
	if( ArmRotation > -2700 )
	{
		ArmRotation -= 32;
	}
	else if ( ArmRotation < 2700 )
	{
		ArmRotation += 32;
	}
			
}

//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::OpenArms()
{
	if( ArmRotation < -1024 )
	{
		ArmRotation += 32;
	}
	else if ( ArmRotation > 1024 )
	{
		ArmRotation -= 32;
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::AnimateArms()
{
	ArmRotation += RotationDelta;
	if( ArmRotation < -3000 ) RotationDelta = -RotationDelta;
	if( ArmRotation > 3000 ) RotationDelta = -RotationDelta;
	
}

//--------------------------------------
//	
//--------------------------------------
void TentakillBoss::BombMe( PlayerShip &Vic, int BombDamageValue )
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

	
//------------------------------------------------------------------------------
//	Private funks
//------------------------------------------------------------------------------

// ------------- 1st cannon -------------------------
void TentakillBoss::Load2WayRotate( TurretArray &Turrets,
			         Vector2df32 *Position,
			         Vector2df32 *Target,
			         std::vector<Turret>::iterator &TurretPtr,
					 s32 angle )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 0;				// 
	Tp.Yradius			= 0;				// 
	Tp.Angle			= angle;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 512;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/12;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 4;			// 
	Tp.BulletsPerShot	= 1;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 5;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SINGLE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(1.25);
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 16;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
		
	
	Turrets.Spawn( Tp, 
				   Position, 
				   Target,
				   TurretPtr );


}

// To intersect with another one copy of itself
void TentakillBoss::Add4WaySmart( Vector2df32 *Position,
			       Vector2df32 *Target,
			       std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= true;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/8;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 20;			// 
	Tp.BulletsPerShot	= 4;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
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
	Tp.Bp.BaseFrame = 28;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	TurretPtr->AddPattern( Tp );
	
}

// set target to self when in use
void TentakillBoss::LoadSelfTargetKetsuiSprayTrafalgar( TurretArray &Turrets,  
										 Vector2df32 *Position,
									     Vector2df32 *Target,
									     std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 1024;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/6;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 120;			// 
	Tp.BulletsPerShot	= 8;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 12;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 5;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)*2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 11;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 12;
	Tp.Bp.Height = 12;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	Turrets.Spawn( Tp, 
				   Position, 
				   Target,
				   TurretPtr );
	
}


void TentakillBoss::AddPsikyoSprayWave3(  Vector2df32 *Position,
						   Vector2df32 *Target,
					       std::vector<Turret>::iterator &TurretPtr,
						   int AngleDirection )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 512;				// + rotation speed
	Tp.SpreadAngle		= BRAD_2PI/12;			// distance of each bullet on a spread
	Tp.AngleDirection	= AngleDirection;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 30;			// 
	Tp.BulletsPerShot	= 12;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 3;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(1.25);
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 5;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 12;
	Tp.Bp.Height = 12;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	TurretPtr->AddPattern( Tp );
	
}

void TentakillBoss::AddSprayWaveDelta3Waves( Vector2df32 *Position,
						      Vector2df32 *Target,
					          std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;

	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= BRAD_PI/2;				// 
	Tp.MaxAngle			= BRAD_2PI-BRAD_PI/2;
	Tp.AngleDelta		= 512;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/4;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 90;			// 
	Tp.BulletsPerShot	= 3;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 7;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 500;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)/8;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 31;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;

	TurretPtr->AddPattern( Tp );
	
}

void TentakillBoss::AddRings(  Vector2df32 *Position,
			    Vector2df32 *Target,
				std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 15;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= 4096;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 9;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPREAD_RINGS;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = 224;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 20;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 12;
	Tp.Bp.Height = 12;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	TurretPtr->AddPattern( Tp );
	

}


void TentakillBoss::AddStraightLasers( Vector2df32 *Position,
				        Vector2df32 *Target,
					    std::vector<Turret>::iterator &TurretPtr,
					    int SpawnDelay )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_PI * 2;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= 0;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= SpawnDelay;				// 
	Tp.MaxSpawnDelay	= 30;			// 
	Tp.BulletsPerShot	= 1;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 4;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 5;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_NORMAL;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(0.75)/3;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -30;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 1;
	Tp.Bp.BaseFrame = 24;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	TurretPtr->AddPattern( Tp );
	

}


void TentakillBoss::AddPsikyoSprayW5Decel(  Vector2df32 *Position,
							 Vector2df32 *Target,
							 std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= -20;				// 
	Tp.Yradius			= -20;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 512;				// + rotation speed
	Tp.SpreadAngle		= BRAD_2PI/12;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 30;			// 
	Tp.BulletsPerShot	= 12;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 5;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(0.75)*4;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 16;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::DECEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	TurretPtr->AddPattern( Tp );
	

}

