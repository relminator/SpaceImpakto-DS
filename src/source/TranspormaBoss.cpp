/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	TranspormaBoss class


*******************************************************************************
******************************************************************************/

#include "TranspormaBoss.h"

void LoadElasticSmart( TurretArray &Turrets,
			         Vector2df32 *Position,
			         Vector2df32 *Target,
			         std::vector<Turret>::iterator &TurretPtr,
					 s32 angle );
void LoadSlowKiller( TurretArray &Turrets,  
					 Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr );

void Add1WaySmart( Vector2df32 *Position,
			       Vector2df32 *Target,
			       std::vector<Turret>::iterator &TurretPtr );

void AddStraightSpreadOffseted(  Vector2df32 *Position,
						   Vector2df32 *Target,
					       std::vector<Turret>::iterator &TurretPtr,
						   int AngleDirection );

void AddKetsuiSelfTargetBig( Vector2df32 *Position,
						      Vector2df32 *Target,
					          std::vector<Turret>::iterator &TurretPtr );

void AddPsikyoWaveStage5(  Vector2df32 *Position,
			    Vector2df32 *Target,
				std::vector<Turret>::iterator &TurretPtr,
				int angle );

void AddDoubleElectricSprayer( Vector2df32 *Position,
				        Vector2df32 *Target,
					    std::vector<Turret>::iterator &TurretPtr,
					    int Direction );
						
void AddProgearWhip(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr );

void AddStraight(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int Delay );


//--------------------------------------
//	
//--------------------------------------
void TranspormaBoss::Spawn( TurretArray &Turrets )
{

	const Vector2df32 Offsets[3] =
	{
		{ -6, -24 },
		{ -34, -20 },
		{ -24, 16 } 
		
	};
	
	x = 400 << 12;
	y = 96 << 12;
	Dx = -1024;
	Dy = 96 << 12;
	
	Frame = 0;
	Counter = 0;
	Active = true;
	Alive = true;
	Hp = 900 * 10;
	MaxHp = Hp;
	
	DeathScore = Hp * 8;
	

	Width = 32;
	Height = 32;
	
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
	LoadElasticSmart( Turrets,
			       &(Cannons[0].TurretPosition),
			       &(Cannons[0].TurretTarget),
			       Cannons[0].TurretPtr,
				   0 );
	LoadElasticSmart( Turrets,
			       &(Cannons[1].TurretPosition),
			       &(Cannons[1].TurretTarget),
			       Cannons[1].TurretPtr,
				   BRAD_PI/2 );
	LoadElasticSmart( Turrets,
			       &(Cannons[2].TurretPosition),
			       &(Cannons[2].TurretTarget),
			       Cannons[2].TurretPtr,
				   BRAD_PI/2*2 );
	
	// 1st, 1
	for( int i = 0; i < 3; i++)
	{
			Add1WaySmart( &(Cannons[i].TurretPosition),
						  &(Cannons[i].TurretTarget),
			              Cannons[i].TurretPtr );
	}
		
	// 1st, 2
	for( int i = 0; i < 3; i++)
	{
			AddStraight( &(Cannons[i].TurretPosition),
						 &(Cannons[i].TurretTarget),
			             Cannons[i].TurretPtr,
						 i * 10);
	}
	
	
	
	//  ----- 2nd -----
	// 2nd, 0
	LoadSlowKiller( Turrets,
					&(Cannons[3].TurretPosition),
					&(Cannons[3].TurretTarget),
					Cannons[3].TurretPtr );
	LoadSlowKiller( Turrets,
					&(Cannons[4].TurretPosition),
					&(Cannons[4].TurretTarget),
					Cannons[4].TurretPtr );
	
	// 2nd, 1
	AddKetsuiSelfTargetBig(  &(Cannons[3].TurretPosition),
						  &(Cannons[3].TurretTarget),
					      Cannons[3].TurretPtr );
	AddKetsuiSelfTargetBig(  &(Cannons[4].TurretPosition),
						  &(Cannons[4].TurretTarget),
					      Cannons[4].TurretPtr );
	
	// 2nd, 2
	AddPsikyoWaveStage5( &(Cannons[3].TurretPosition),
					     &(Cannons[3].TurretTarget),
					     Cannons[3].TurretPtr, 1 );
	
	AddPsikyoWaveStage5( &(Cannons[4].TurretPosition),
					     &(Cannons[4].TurretTarget),
					     Cannons[4].TurretPtr, -1 );
	
	// 2nd, 3
	AddDoubleElectricSprayer( &(Cannons[3].TurretPosition),
						     &(Cannons[3].TurretTarget),
					         Cannons[3].TurretPtr, 1 );
	AddDoubleElectricSprayer( &(Cannons[4].TurretPosition),
						     &(Cannons[4].TurretTarget),
					         Cannons[4].TurretPtr, -1 );
	
	// 2nd 4
	
	AddStraightSpreadOffseted( &(Cannons[3].TurretPosition),
						     &(Cannons[3].TurretTarget),
					         Cannons[3].TurretPtr, 1 );
	AddStraightSpreadOffseted( &(Cannons[4].TurretPosition),
						     &(Cannons[4].TurretTarget),
					         Cannons[4].TurretPtr, -1 );
	
	// 2nd 5
	AddProgearWhip( &(Cannons[3].TurretPosition),
				 &(Cannons[3].TurretTarget),
				 Cannons[3].TurretPtr );
				 
	AddProgearWhip( &(Cannons[4].TurretPosition),
				 &(Cannons[4].TurretTarget),
				 Cannons[4].TurretPtr );
	
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
		Parts[i].Width = Images[i+1].width;
		Parts[i].Height = Images[i+1].height;
		LoadPartPath( Parts[i], Offsets[i].x, Offsets[i].y, Offsets[i].x, Offsets[i].y );
	}
	
	
}


//--------------------------------------
//	
//--------------------------------------
void TranspormaBoss::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	//iprintf( "\x1b[1;1HTransSplineIndex = %i         ", Path.GetIndex() );
	//iprintf( "\x1b[2;1HPartIndex = %i         ", Parts[0].Path.GetIndex() );
	
	// Parts
	for( int i = 0; i < 3; i++ )
	{
		Cannons[i].x = Parts[i].x;
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
	// center
	for( int i = 3; i < 5; i++ )
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
		case 1:
			for( int i = 0; i < 3; i++ )
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
			for( int i = 0; i < 3; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			
			if( !Cannons[3].TurretPtr->IsActivated() )
			{
				Cannons[3].TurretPtr->Activate();
				Cannons[3].TurretPtr->DoPattern(0);
			}
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			for( int i = 0; i < 5; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 9:
			break;
		case 10:
			// 1 way smart + ketsui self target
			Cannons[3].TurretTarget.x = Cannons[3].x;
			Cannons[3].TurretTarget.y = Cannons[3].y;
			for( int i = 0; i < 4; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(1);
				}
			}
			break;
		case 11:
			Cannons[3].TurretTarget.x = Cannons[3].x;
			Cannons[3].TurretTarget.y = Cannons[3].y;
			break;
		case 12:
			Cannons[3].TurretTarget.x = Cannons[3].x;
			Cannons[3].TurretTarget.y = Cannons[3].y;
			break;
		case 13:
			Cannons[3].TurretTarget.x = Cannons[3].x;
			Cannons[3].TurretTarget.y = Cannons[3].y;
			break;
		case 14:
			for( int i = 0; i < 5; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 15:
			// Activate psikyo 3 wave double 
			Cannons[3].y = y - (30 << 12);
			Cannons[4].y = y + (30 << 12);
			
			for( int i = 3; i < 5; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
		
			for( int i = 3; i < 5; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(2);
				}
			}
			
			break;
		case 16:
			Cannons[3].y = y - (30 << 12);
			Cannons[4].y = y + (30 << 12);
			break;
		case 17:
			Cannons[3].y = y - (30 << 12);
			Cannons[4].y = y + (30 << 12);
			break;
		case 18:
			Cannons[3].y = y - (30 << 12);
			Cannons[4].y = y + (30 << 12);
			for( int i = 3; i < 5; i++ )
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
			Cannons[3].y = y - (30 << 12);
			Cannons[4].y = y + (30 << 12);
			for( int i = 3; i < 5; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}for( int i = 3; i < 5; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(3);
				}
			}
			break;
		case 21:
			Cannons[3].y = y - (30 << 12);
			Cannons[4].y = y + (30 << 12);
			for( int i = 3; i < 5; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}break;
		case 22:
			Cannons[3].y = y - (30 << 12);
			Cannons[4].y = y + (30 << 12);
			for( int i = 3; i < 5; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}break;
		case 23:
			break;
		case 24:
			for( int i = 3; i < 5; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 25:
			break;
		case 26:
			// activate Offseted
			Cannons[3].y = y - (20 << 12);
			Cannons[4].y = y + (20 << 12);
			for( int i = 3; i < 5; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
			for( int i = 3; i < 5; i++ )
			{
				if( !Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Activate();
					Cannons[i].TurretPtr->DoPattern(4);
				}
			}
			break;
		case 27:
			for( int i = 0; i < 3; i++ )
			{
				if( Cannons[i].TurretPtr->IsActivated() )
				{
					Cannons[i].TurretPtr->Deactivate();
				}
			}
			break;
		case 28:
			Cannons[3].y = y - (20 << 12);
			Cannons[4].y = y + (20 << 12);
			for( int i = 3; i < 5; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
			break;
		case 29:
			Cannons[3].y = y - (30 << 12);
			Cannons[4].y = y + (30 << 12);
			for( int i = 3; i < 5; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
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
			break;
		case 32:
			// Progear whip
			if( !Cannons[3].TurretPtr->IsActivated() )
			{
				Cannons[3].TurretPtr->Activate();
				Cannons[3].TurretPtr->DoPattern(5);
			}
			break;
		case 33:
			for( int i = 0; i < 3; i++ )
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
//	for( int i = 0; i < 5; i++ )
//			{
//				if( Cannons[i].TurretPtr->IsActivated() )
//				{
//					Cannons[i].TurretPtr->Deactivate();
//				}
//			}
//	// end temp
	
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
void TranspormaBoss::Draw()
{
	
	if( (DrawMode == EN_WHITE) )
	{
		DrawMode = EN_NORMAL;
		glColor( RGB15(31,31,31) );
		for( int i = 0; i < 3; i++)
		{
			DrawLinks( x, y, Parts[i].x, Parts[i].y, Counter + i * 3000, &Images[4 + LinkFrame] );  
		}
		
		glColor( RGB15(31,0,0) );
		
		glSprite( ( x >> 12 ) - Images[0].width/2, 
				  ( y >> 12 ) - Images[0].height/2, 
				  GL_FLIP_NONE, 
				  &Images[0] );
		
		for( int i = 0; i < 3; i++)
		{
			int randy = qran_range(-1,1);
			glSprite( ( Parts[i].x >> 12 ) - Images[i+1].width/2, 
				      ( Parts[i].y >> 12 ) - Images[i+1].height/2 + randy, 
				      GL_FLIP_NONE, 
				      &Images[i+1] );
			
		}
	
		glColor( RGB15(31,31,31) );
	}
	else
	{
		for( int i = 0; i < 3; i++)
		{
			DrawLinks( x, y, Parts[i].x, Parts[i].y, Counter + i * 3000, &Images[4 + LinkFrame] );  
		}
		
		glSprite( ( x >> 12 ) - Images[0].width/2, 
				  ( y >> 12 ) - Images[0].height/2, 
				  GL_FLIP_NONE, 
				  &Images[0] );
		
		for( int i = 0; i < 3; i++)
		{
			int randy = qran_range(-1,1);
			glSprite( ( Parts[i].x >> 12 ) - Images[i+1].width/2, 
				      ( Parts[i].y >> 12 ) - Images[i+1].height/2 + randy, 
				      GL_FLIP_NONE, 
				      &Images[i+1] );
			
		}
	
	}
	
	// exhaust
	if( (Counter & 3 ) == 0 )
	for( int i = 0; i < 3; i++)
	{
		Explosion::Instance()->Spawn( (Parts[i].x >> 12) + Parts[i].Width/2 - 4,
								  (Parts[i].y >> 12) + Parts[i].Height/2 - 4,
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
void TranspormaBoss::DrawAABB( int color ) const 
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
EntityContainer::CollisionPacket TranspormaBoss::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed )
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
			}
			
			
			
		}  // end if
	}

	return Ret;
	
}


//--------------------------------------
//	
//--------------------------------------
void TranspormaBoss::ForceDestruction()
{
	Destroy();
}
	
//--------------------------------------
//	
//--------------------------------------
void TranspormaBoss::LoadPath()
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
	
	p.x = 180;
	p.y = 96;
	Path.AddPoint( p );  // 14
	
	p.x = 180;
	p.y = 20;
	Path.AddPoint( p );  // 15
	
	p.x = 180;
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
void TranspormaBoss::Destroy()
{
	for( int i = 0; i < 3; i++)
	{
		ExplodeLinks( x, y, Parts[i].x, Parts[i].y, Counter + i * 3000 );  
		
		Explosion::Instance()->Spawn( Parts[i].x >> 12,
									  Parts[i].y >> 12,
									  0,
									  0,
									  Explode::MEDIUM_3,
									  Explode::SMALL_2 );
			
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
void TranspormaBoss::BombMe( PlayerShip &Vic, int BombDamageValue )
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
void TranspormaBoss::DrawLinks( s32 sx, s32 sy, s32 ex, s32 ey, int Frame, glImage *Sprite )
{
	
	Vector2df32 Start, End;
	Start.x = sx; 
	Start.y = sy; 
	End.x = ex; 
	End.y = ey; 
	
	for( int i = 0; i < 8; i++ )
	{
		int x0 = (Start.x + cosLerp(Counter*740) * 5) >> 12;
		int y0 = (Start.y + sinLerp(Counter*440) * 5) >> 12;
		int x1 = Start.x >> 12;
		int y1 = Start.y >> 12;
		
		int x2 = End.x >> 12;
		int y2 = End.y >> 12;
		int x3 = (End.x + sinLerp(Counter*640) * 10) >> 12;;
		int y3 = (End.y + cosLerp(Counter*540) * 10) >> 12;;
		
		s32 nx = Bezierf32( x0, x1, x2, x3, i * (4096/8) );
		s32 ny = Bezierf32( y0, y1, y2, y3, i * (4096/8) );
		 
		glSprite( (nx >> 12) - 8, (ny >> 12) - 8, GL_FLIP_NONE, Sprite );
	}
	
			
}

//--------------------------------------
//	
//--------------------------------------
void TranspormaBoss::ExplodeLinks( s32 sx, s32 sy, s32 ex, s32 ey, int Frame )
{
	
	Vector2df32 Start, End;
	Start.x = sx; 
	Start.y = sy; 
	End.x = ex; 
	End.y = ey; 
	
	for( int i = 0; i < 8; i++ )
	{
		int x0 = (Start.x + cosLerp(Counter*740) * 5) >> 12;
		int y0 = (Start.y + sinLerp(Counter*440) * 5) >> 12;
		int x1 = Start.x >> 12;
		int y1 = Start.y >> 12;
		
		int x2 = End.x >> 12;
		int y2 = End.y >> 12;
		int x3 = (End.x + sinLerp(Counter*640) * 10) >> 12;;
		int y3 = (End.y + cosLerp(Counter*540) * 10) >> 12;;
		
		s32 nx = Bezierf32( x0, x1, x2, x3, i * (4096/8) );
		s32 ny = Bezierf32( y0, y1, y2, y3, i * (4096/8) );
		
		Explosion::Instance()->Spawn( nx >> 12,
									  ny >> 12,
									  0,
									  0,
									  Explode::SMALL_2
									);
	}
	
			
}

//--------------------------------------
//	
//--------------------------------------
void TranspormaBoss::LoadPartPath( Part &Prt, int _sx, int _sy, int _ex, int _ey )
{
	Prt.Path.Init( 16 );
	
	Spline::Coord p;
	
	// start
	p.x = _sx;
	p.y = _sy;
	
	// we should use 2 points
	Prt.Path.AddPoint( p );
	Prt.Path.AddPoint( p );
	
	// randomize for now
	for (int i=0; i < 10; i++)
	{
		p.x = UTIL::RND_RANGE(-100, 100);
		p.y = UTIL::RND_RANGE(-90, 90);
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
void TranspormaBoss::AnimateLinks()
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
void TranspormaBoss::CloseLinks()
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

// ------------- 1st cannon -------------------------
void LoadElasticSmart( TurretArray &Turrets,
			         Vector2df32 *Position,
			         Vector2df32 *Target,
			         std::vector<Turret>::iterator &TurretPtr,
					 s32 angle )
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
	Tp.MaxSpawnDelay	= 120;			// 
	Tp.BulletsPerShot	= 2;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 6;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 360;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_CENTERED;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = 184;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 31;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	Turrets.Spawn( Tp, 
				   Position, 
				   Target,
				   TurretPtr );


}


void LoadSlowKiller( TurretArray &Turrets,  
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
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 3776;				// + rotation speed
	Tp.SpreadAngle		= 6656;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 2;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 24;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 3;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= -2360;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(1.7);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPREAD_OFFSET;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(2.25);
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 5;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 12;
	Tp.Bp.Height = 12;
	Tp.Bp.Type = Bullet::DECEL;
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
	Tp.Bp.BaseFrame = 4;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 22;
	Tp.Bp.Height = 22;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::ROTATED;
	
	TurretPtr->AddPattern( Tp );
	
}

// Set target to self
void AddKetsuiSelfTargetBig( Vector2df32 *Position,
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
	Tp.AngleDelta		= 4096;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/12;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 160;			// 
	Tp.BulletsPerShot	= 4;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 8;				// maximum bullets per wave
	Tp.MaxWaveDelay		= -1;			// maximum delay for next shot in a wave
	
	
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
	Tp.Bp.BaseFrame = 8;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 20;
	Tp.Bp.Height = 20;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	TurretPtr->AddPattern( Tp );
	
}

// two turrets
void AddPsikyoWaveStage5(  Vector2df32 *Position,
			    Vector2df32 *Target,
				std::vector<Turret>::iterator &TurretPtr,
				int angle )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= BRAD_PI/2+BRAD_PI/4;				// 
	Tp.MaxAngle			= BRAD_2PI-BRAD_PI/2-BRAD_PI/4;
	Tp.AngleDelta		= 1024;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/6;			// distance of each bullet on a spread
	Tp.AngleDirection	= angle;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 4;			// 
	Tp.BulletsPerShot	= 5;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 1;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 5;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_CENTERED;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(1.5);
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 0;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 7;
	Tp.Bp.Height = 7;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	TurretPtr->AddPattern( Tp );
	

}


// 1 up 1 down about 64 units away from each other
void AddDoubleElectricSprayer( Vector2df32 *Position,
				        Vector2df32 *Target,
					    std::vector<Turret>::iterator &TurretPtr,
					    int Direction )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= (BRAD_PI/8)*8;				// 
	Tp.MaxAngle			= BRAD_2PI-(BRAD_PI/8)*8;
	Tp.AngleDelta		= 128;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/8;			// distance of each bullet on a spread
	Tp.AngleDirection	= Direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 2;			// 
	Tp.BulletsPerShot	= 5;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(0);		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(1);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_CENTERED;
	
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


void AddProgearWhip(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr )
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
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 30;			// 
	Tp.BulletsPerShot	= 2;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 24;				// maximum bullets per wave
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
	Tp.Bp.BaseFrame = 34;
	Tp.Bp.Smart = true;
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
	Tp.Bp.Speed = inttof32(1) * 4;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 0;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::DECEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	TurretPtr->AddPattern( Tp );
	

}
