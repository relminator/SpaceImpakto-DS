/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Monkat Boss class


*******************************************************************************
******************************************************************************/

#include "MonkatBoss.h"

void LoadStarWave( TurretArray &Turrets,
			        Vector2df32 *Position,
			        Vector2df32 *Target,
			        std::vector<Turret>::iterator &TurretPtr );

void LoadSimpleTarget( TurretArray &Turrets,
					   Vector2df32 *Position,
					   Vector2df32 *Target,
					   std::vector<Turret>::iterator &TurretPtr,
					   int delay );

void Add4wayRotateWave( Vector2df32 *Position,
						Vector2df32 *Target,
						std::vector<Turret>::iterator &TurretPtr,
						int Direction,
						int Delay );
						
void AddSingleStraight( Vector2df32 *Position,
						Vector2df32 *Target,
						std::vector<Turret>::iterator &TurretPtr,
						int delay );


void Add2WayRotate(  Vector2df32 *Position,
			         Vector2df32 *Target,
			         std::vector<Turret>::iterator &TurretPtr,
					 s32 angle );

void AddKetsiuTrafalgar(  Vector2df32 *Position,
						   Vector2df32 *Target,
					       std::vector<Turret>::iterator &TurretPtr,
						   int Direction,
						   int Delay,
						   s32 Angle );

void Add4WayRotate(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction );

void AddSmartRings(  Vector2df32 *Position,
					Vector2df32 *Target,
					std::vector<Turret>::iterator &TurretPtr );

void AddStraight4WaySpread(  Vector2df32 *Position,
						 Vector2df32 *Target,
						 std::vector<Turret>::iterator &TurretPtr );

void AddWasAgSingleLayer(  Vector2df32 *Position,
						   Vector2df32 *Target,
						   std::vector<Turret>::iterator &TurretPtr );

void AddProgearWhip2(  Vector2df32 *Position,
					  Vector2df32 *Target,
					  std::vector<Turret>::iterator &TurretPtr );

void AddFullWaveOffseted(  Vector2df32 *Position,
						   Vector2df32 *Target,
						   std::vector<Turret>::iterator &TurretPtr,
						   int delay );

//--------------------------------------
//	
//--------------------------------------
void MonkatBoss::Init( int Palette, glImage* const Sprites,
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
void MonkatBoss::Spawn( TurretArray &Turrets )
{

	x = 400 << 12;
	y = 96 << 12;
	Dx = -1024;
	Dy = 96 << 12;
	
	Frame = 0;
	Counter = 0;
	Active = true;
	Alive = true;
	Hp = 900 * 8;
	MaxHp = Hp;
	DeathScore = Hp * 12;
	

	Width = 64;
	Height = 64;
	
	DestroyedCounter = 60 * 5;
	
	
	Angle = 0;
	
	LoadPath();
	
	// set up options
	Cannon cannon;
	
	cannon.x = x;
	cannon.y = y;
	cannon.Width = 16;
	cannon.Height = 16;
	
	cannon.TurretPosition.x = x;
	cannon.TurretPosition.y = y;
	cannon.TurretTarget.x = 0;
	cannon.TurretTarget.y = 96 << 12;
	
	// be safe
	Cannons.clear();
	
	// add
	Cannons.push_back(cannon);    // 0
	Cannons.push_back(cannon);    // 1
	Cannons.push_back(cannon);    // 2
	Cannons.push_back(cannon);    // 3
	Cannons.push_back(cannon);    // 4
	Cannons.push_back(cannon);    // 5
	
	
	// Center
	// 0
	LoadStarWave( Turrets,
			       &(Cannons[4].TurretPosition),
			       &(Cannons[4].TurretTarget),
			       Cannons[4].TurretPtr );
	LoadStarWave( Turrets,
			       &(Cannons[5].TurretPosition),
			       &(Cannons[5].TurretTarget),
			       Cannons[5].TurretPtr );
	
	// 1
	Add4wayRotateWave( &(Cannons[4].TurretPosition),
			       &(Cannons[4].TurretTarget),
			       Cannons[4].TurretPtr, 1, 0 );
	
	Add4wayRotateWave( &(Cannons[5].TurretPosition),
			       &(Cannons[5].TurretTarget),
			       Cannons[5].TurretPtr, -1, 30 );
	
	// 2
	Add4WayRotate( &(Cannons[4].TurretPosition),
			       &(Cannons[4].TurretTarget),
			       Cannons[4].TurretPtr, 1 );
	
	Add4WayRotate( &(Cannons[5].TurretPosition),
			       &(Cannons[5].TurretTarget),
			       Cannons[5].TurretPtr, -1 );
	
	// 3
	AddSmartRings( &(Cannons[4].TurretPosition),
			       &(Cannons[4].TurretTarget),
			       Cannons[4].TurretPtr );
	
	AddStraight4WaySpread( &(Cannons[5].TurretPosition),
			       &(Cannons[5].TurretTarget),
			       Cannons[5].TurretPtr );
	
	// 4
	AddWasAgSingleLayer( &(Cannons[4].TurretPosition),
						 &(Cannons[4].TurretTarget),
						 Cannons[4].TurretPtr );
	
	AddProgearWhip2( &(Cannons[5].TurretPosition),
				 &(Cannons[5].TurretTarget),
				 Cannons[5].TurretPtr );

	
	
	// Emitters
	// 0
	for( int i = 0; i < 4; i++ )
	{
		LoadSimpleTarget( Turrets,
					      &(Cannons[i].TurretPosition),
					      &(Cannons[i].TurretTarget),
					      Cannons[i].TurretPtr, i * 20 );
	}
	
	// 1
	for( int i = 0; i < 4; i++ )
	{
		AddSingleStraight( &(Cannons[i].TurretPosition),
					       &(Cannons[i].TurretTarget),
					       Cannons[i].TurretPtr, i * 30 );
	}
	
	// 2
	for( int i = 0; i < 4; i++ )
	{
		Add2WayRotate( &(Cannons[i].TurretPosition),
					       &(Cannons[i].TurretTarget),
					       Cannons[i].TurretPtr,
						   i * BRAD_PI/2);
	}
	
	// 3
	AddKetsiuTrafalgar( &(Cannons[0].TurretPosition),
					   &(Cannons[0].TurretTarget),
					   Cannons[0].TurretPtr,
					   1,
					   0,
					   BRAD_PI/2);

	AddKetsiuTrafalgar( &(Cannons[1].TurretPosition),
					   &(Cannons[1].TurretTarget),
					   Cannons[1].TurretPtr,
					   1,
					   8,
					   BRAD_PI/2);
	
	AddKetsiuTrafalgar( &(Cannons[2].TurretPosition),
					   &(Cannons[2].TurretTarget),
					   Cannons[2].TurretPtr,
					   -1,
					   30,
					   BRAD_2PI-BRAD_PI/2);
	
	AddKetsiuTrafalgar( &(Cannons[3].TurretPosition),
					   &(Cannons[3].TurretTarget),
					   Cannons[3].TurretPtr,
					   -1,
					   38,
					   BRAD_2PI-BRAD_PI/2);
	
	
	// 4
	for( int i = 0; i < 4; i++ )
	{
		AddFullWaveOffseted( &(Cannons[i].TurretPosition),
					       &(Cannons[i].TurretTarget),
					       Cannons[i].TurretPtr,
						   i * 30);
	}
	
	
	// Deactivate
	Cannons[0].TurretPtr->Deactivate();
	Cannons[1].TurretPtr->Deactivate();
	Cannons[2].TurretPtr->Deactivate();
	Cannons[3].TurretPtr->Deactivate();
	Cannons[4].TurretPtr->Deactivate();
	Cannons[5].TurretPtr->Deactivate();
	
	
}


//--------------------------------------
//	
//--------------------------------------
void MonkatBoss::Update( PlayerShip &Vic )
{

//1-3 center stopped far
//4 - 6 up then down far
//7 - 9 center stopped far
//10 - 14 sinwave far to near then far
//15 - 17 stopped far
//18 - 25   goin near 
//then down far top near
//26 - 32 center stopped far
//33 -  34 to near then back center



	
	Counter++;
	
	//iprintf( "\x1b[1;1HMonkatIndex = %i         ", Path.GetIndex() );
	//iprintf( "\x1b[2;1HTurretIndex = %i         ", Cannons[0].TurretPtr->GetIndex() );
	//iprintf( "\x1b[3;1HTurretActive = %i         ", Cannons[0].TurretPtr->IsActivated() );
	
	Angle += 256;
	
	for( int i = 0; i < 4; i++ )
	{
		s32 sx = cosLerp(Angle + i * BRAD_PI/2) * 40;
		s32 sy = sinLerp(Angle + i * BRAD_PI/2) * 40;
		Cannons[i].x = sx + x;
		Cannons[i].y = sy + y;
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
		case 2:  // star
			Cannons[4].TurretPtr->Activate();
			Cannons[4].TurretPtr->DoPattern(0);
			break;
		case 3:
			break;
		case 4:
			Cannons[4].TurretPtr->Deactivate();
			// Progear whip
			Cannons[0].TurretPtr->Activate();
			Cannons[0].TurretPtr->DoPattern(0);
			Cannons[2].TurretPtr->Activate();
			Cannons[2].TurretPtr->DoPattern(0);
			break;
		case 5:
			Cannons[0].TurretPtr->Deactivate();
			Cannons[2].TurretPtr->Deactivate();
			break;
		case 6:
			// rotate 4way
			Cannons[4].TurretPtr->Activate();
			Cannons[4].TurretPtr->DoPattern(1);
			Cannons[5].TurretPtr->Activate();
			Cannons[5].TurretPtr->DoPattern(1);
			// straight single
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].TurretPtr->Activate();
				Cannons[i].TurretPtr->DoPattern(1);
			}
			break;
		case 7:
			break;
		case 8:
			Cannons[4].TurretPtr->Deactivate();
			Cannons[5].TurretPtr->Deactivate();
			break;
		case 9:
			// touhou
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].TurretPtr->Activate();
				Cannons[i].TurretPtr->DoPattern(2);
			}
			for( int i = 0; i < 4; i++ )
			{
				int rad = (sinLerp(Counter*200) * 60 ) >> 12;
				s32 sx = cosLerp(Angle + i * BRAD_PI/2) * rad;
				s32 sy = sinLerp(Angle + i * BRAD_PI/2) * rad;
				Cannons[i].x = sx + x;
				Cannons[i].y = sy + y;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
			break;
		case 10:
			for( int i = 0; i < 4; i++ )
			{
				int rad = (sinLerp(Counter*200) * 60 ) >> 12;
				s32 sx = cosLerp(Angle + i * BRAD_PI/2) * rad;
				s32 sy = sinLerp(Angle + i * BRAD_PI/2) * rad;
				Cannons[i].x = sx + x;
				Cannons[i].y = sy + y;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
			break;
		case 11:
			for( int i = 0; i < 4; i++ )
			{
				int rad = (sinLerp(Counter*200) * 60 ) >> 12;
				s32 sx = cosLerp(Angle + i * BRAD_PI/2) * rad;
				s32 sy = sinLerp(Angle + i * BRAD_PI/2) * rad;
				Cannons[i].x = sx + x;
				Cannons[i].y = sy + y;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
			break;
		case 12:
			for( int i = 0; i < 4; i++ )
			{
				int rad = (sinLerp(Counter*400) * 60 ) >> 12;
				s32 sx = cosLerp(Angle + i * BRAD_PI/2) * rad;
				s32 sy = sinLerp(Angle + i * BRAD_PI/2) * rad;
				Cannons[i].x = sx + x;
				Cannons[i].y = sy + y;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
			break;
		case 13:
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].TurretPtr->Deactivate();
			}
			// rotate 4way
			Cannons[4].TurretPtr->Activate();
			Cannons[4].TurretPtr->DoPattern(1);
			Cannons[5].TurretPtr->Activate();
			Cannons[5].TurretPtr->DoPattern(1);
			break;
		case 14:
			break;
		case 15:
			Cannons[4].TurretPtr->Deactivate();
			Cannons[5].TurretPtr->Deactivate();
			
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].TurretPtr->Activate();
				Cannons[i].TurretPtr->DoPattern(3);
			}
			// stop emitter rotation
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].y = y;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
			break;
		case 16:
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].y = y;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
			break;
		case 17:
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].y = y;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
			}
			break;
		case 18:
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].x = x;
				Cannons[i].y = y;
				Cannons[i].TurretPosition.x = Cannons[i].x;
				Cannons[i].TurretPosition.y = Cannons[i].y;
				Cannons[i].TurretPtr->Deactivate();
			}
			break;
		case 19:
			// rotate 4way green
			Cannons[4].TurretPtr->Activate();
			Cannons[4].TurretPtr->DoPattern(2);
			Cannons[5].TurretPtr->Activate();
			Cannons[5].TurretPtr->DoPattern(2);
			break;
		case 20:
			break;
		case 21:
			break;
		case 22:
			Cannons[4].TurretPtr->Deactivate();
			Cannons[5].TurretPtr->Deactivate();
			break;
		case 23:
			// Straight + rings
			Cannons[4].TurretPtr->Activate();
			Cannons[4].TurretPtr->DoPattern(3);
			Cannons[5].TurretPtr->Activate();
			Cannons[5].TurretPtr->DoPattern(3);
			break;
		case 24:
			break;
		case 25:
			break;
		case 26:
			Cannons[4].TurretPtr->DoPattern(4);
			Cannons[5].TurretPtr->DoPattern(4);
			break;
		case 27:
			break;
		case 28:
			break;
		case 29:
			break;
		case 30:
			break;
		case 31:
			Cannons[4].TurretPtr->Deactivate();
			Cannons[5].TurretPtr->Deactivate();
			break;
		case 32:
			for( int i = 0; i < 4; i++ )
			{
				Cannons[i].TurretPtr->Activate();
				Cannons[i].TurretPtr->DoPattern(4);
			}
			break;
		case 33:
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
			break;
	}

	if( Path.GetIndex() == 7 )
	{
		Path.SetPoint(0, 128, 96);
		Path.SetPoint(1, 128, 96);
		Path.SetPoint(3, 215, 40);
		Path.SetPoint(4, 215, 192-40);
		Path.SetPoint(5, 215, 30);
		Path.SetPoint(6, 215, 192-30);
	}
	
	
	Path.Update();
	
	x = Path.GetX();
	y = Path.GetY();
	
	
	Targets::Instance()->AddCoord( x, y );
	
	
	
	// animate
	Frame = 0;

	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
}


//--------------------------------------
//	
//--------------------------------------
void MonkatBoss::Draw()
{
	int DrawFrame = Frame;
	
	if( (DrawMode == EN_WHITE) )
	{
		DrawMode = EN_NORMAL;
		glColor( RGB15(31,0,0) );
		glSprite( (x >> 12) - Width/2 - 8, 
				  (y >> 12) - Height/2, 
				  GL_FLIP_NONE, &Images[DrawFrame] 
				);
		glColor( RGB15(31,31,31) );
	}
	else
	{
		glSprite( (x >> 12) - Width/2 - 8, 
				  (y >> 12) - Height/2, 
				  GL_FLIP_NONE, &Images[DrawFrame] 
				);	
	}
	
	// do the emitters
	if( (Counter & 1) == 0 )
	{
		for( int i = 0; i < 4; i++ )
		{
			glSprite( (Cannons[i].x >> 12) - 8, 
					  (Cannons[i].y >> 12) - 8, 
					  GL_FLIP_NONE, &Images2[7] );	
		}		
	}
	
	DrawHp( Counter );
	
	
}


//--------------------------------------
//	
//--------------------------------------
void MonkatBoss::DrawAABB( int color ) const 
{

	AABB aabb = Aabb.Get();
	glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
	aabb.Resize(512*3);
	glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
			
}


//--------------------------------------
//	
//--------------------------------------
EntityContainer::CollisionPacket MonkatBoss::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed )
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
		}  // end if
	}

	return Ret;
	
}


//--------------------------------------
//	
//--------------------------------------
void MonkatBoss::ForceDestruction()
{
	Destroy();
}
	
//--------------------------------------
//	
//--------------------------------------
void MonkatBoss::LoadPath()
{
	Path.Init( 40 );
	
	Spline::Coord p;
	
	// start
	p.x = 300;
	p.y = 96;
	
	// we should use 2 points
	Path.AddPoint( p );  // 1
	Path.AddPoint( p );  // 2
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 3
 	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 4
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 5
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 6
	
	p.x = 200;
	p.y = 40;
	Path.AddPoint( p );  // 7
	
	p.x = 210;
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
	
	p.x = 210;
	p.y = 40;
	Path.AddPoint( p );  // 13
	
	p.x = 180;
	p.y = 96;
	Path.AddPoint( p );  // 14
	
	p.x = 128;
	p.y = 20;
	Path.AddPoint( p );  // 15
	
	p.x = 128;
	p.y = 172;
	Path.AddPoint( p );  // 16
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 17
	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 18
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 19
	
	p.x = 200;
	p.y = 96;
	Path.AddPoint( p );  // 20
	
	
	p.x = 160;
	p.y = 96;
	Path.AddPoint( p );  // 21
	
	p.x = 180;
	p.y = 96;
	Path.AddPoint( p );  // 22
	
	p.x = 200;
	p.y = 192-40;
	Path.AddPoint( p );  // 23
	
	p.x = 200;
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
	
	p.x = 200;
	p.y = 50;
	Path.AddPoint( p );  // 28
	
	p.x = 190;
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
void MonkatBoss::Destroy()
{
	for( int i = 0; i < 6; i++ )
	{
		if( Cannons[i].TurretPtr->IsActivated() )
		{
			Cannons[i].TurretPtr->Deactivate();
		}
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
void MonkatBoss::BombMe( PlayerShip &Vic, int BombDamageValue )
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

void LoadStarWave( TurretArray &Turrets,
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
	Tp.BulletsPerShot	= 34;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 2;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 60;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = 384/4;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 31;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	Turrets.Spawn( Tp, 
				   Position, 
				   Target,
				   TurretPtr
				 );


}


// set target to player
void LoadSimpleTarget( TurretArray &Turrets,
					   Vector2df32 *Position,
					   Vector2df32 *Target,
					   std::vector<Turret>::iterator &TurretPtr,
					   int delay )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 20;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 1200;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/12;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= delay;				// 
	Tp.MaxSpawnDelay	= 45;			// 
	Tp.BulletsPerShot	= 2;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 16;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)/5;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 34;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	Turrets.Spawn( Tp, 
				   Position, 
				   Target,
				   TurretPtr
				 );


}

void Add4wayRotateWave( Vector2df32 *Position,
						Vector2df32 *Target,
						std::vector<Turret>::iterator &TurretPtr,
						int Direction,
						int Delay )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 0;				// 
	Tp.Yradius			= 0;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 1200;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/2;			// distance of each bullet on a spread
	Tp.AngleDirection	= Direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= Delay;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 4;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 16;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)*2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 11;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 11;
	Tp.Bp.Height = 11;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	TurretPtr->AddPattern( Tp );
	
}


void AddSingleStraight( Vector2df32 *Position,
						Vector2df32 *Target,
						std::vector<Turret>::iterator &TurretPtr, 
						int delay )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 0;				// 
	Tp.Yradius			= 0;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= 138;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= delay;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 2;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 7;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 220;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)/6;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 28;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	TurretPtr->AddPattern( Tp );
	
}


void Add2WayRotate(  Vector2df32 *Position,
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
	Tp.SpreadAngle		= BRAD_PI;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 180;			// 
	Tp.BulletsPerShot	= 2;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 24;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(1.25);
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
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

void AddKetsiuTrafalgar(  Vector2df32 *Position,
						   Vector2df32 *Target,
					       std::vector<Turret>::iterator &TurretPtr,
						   int Direction,
						   int Delay,
						   s32 Angle )
{
	Turret::Params Tp;

	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= Angle;					// StartAngle
	Tp.MinAngle			= BRAD_PI/2;				// 
	Tp.MaxAngle			= BRAD_2PI-BRAD_PI/2;
	Tp.AngleDelta		= 512;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/4;			// distance of each bullet on a spread
	Tp.AngleDirection	= Direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= Delay;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 3;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 7;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)*3;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 1;
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


void Add4WayRotate(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 0;				// 
	Tp.Yradius			= 0;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 523;				// + rotation speed
	Tp.SpreadAngle		= BRAD_2PI/4;			// distance of each bullet on a spread
	Tp.AngleDirection	= direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 120;			// 
	Tp.BulletsPerShot	= 4;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 32;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 220;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(1)*4;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 20;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 11;
	Tp.Bp.Height = 11;
	Tp.Bp.Type = Bullet::DECEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	TurretPtr->AddPattern( Tp );
	

}


void AddSmartRings(  Vector2df32 *Position,
					Vector2df32 *Target,
					std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 15;				// 
	Tp.Yradius			= 15;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= BRAD_2PI/8;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 120;			// 
	Tp.BulletsPerShot	= 8;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 2;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 30;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPREAD_RINGS;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(1.25)/2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 10;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 11;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 12;
	Tp.Bp.Height = 12;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	TurretPtr->AddPattern( Tp );
	

}



void AddStraight4WaySpread(  Vector2df32 *Position,
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
	Tp.MinAngle			= (BRAD_PI/8)*9;				// 
	Tp.MaxAngle			= BRAD_2PI-(BRAD_PI/8)*9;
	Tp.AngleDelta		= 128;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/4;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 2;			// 
	Tp.BulletsPerShot	= 4;
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
	Tp.Bp.BaseFrame = 28;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	TurretPtr->AddPattern( Tp );
	

}


void AddWasAgSingleLayer(  Vector2df32 *Position,
						   Vector2df32 *Target,
						   std::vector<Turret>::iterator &TurretPtr )
{

	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 5;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 3776;				// + rotation speed
	Tp.SpreadAngle		= 6656;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 2;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 30;			// 
	Tp.BulletsPerShot	= 24;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= -320;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_NORMAL;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = 384;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
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

void AddProgearWhip2(  Vector2df32 *Position,
					  Vector2df32 *Target,
					  std::vector<Turret>::iterator &TurretPtr )
{
	
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 20;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 1200;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/12;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 30;			// 
	Tp.BulletsPerShot	= 2;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 16;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_WAVE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = inttof32(1)/5;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 20;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 34;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	TurretPtr->AddPattern( Tp );
	
}


void AddFullWaveOffseted(  Vector2df32 *Position,
						   Vector2df32 *Target,
						   std::vector<Turret>::iterator &TurretPtr,
						   int delay )
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
	Tp.SpawnDelay		= delay;				// 
	Tp.MaxSpawnDelay	= 180;			// 
	Tp.BulletsPerShot	= 34;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 2;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= -2360;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(0.7);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPREAD_OFFSET;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(2.0);
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 5;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 12;
	Tp.Bp.Height = 12;
	Tp.Bp.Type = Bullet::DECEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;


	TurretPtr->AddPattern( Tp );
	
}
