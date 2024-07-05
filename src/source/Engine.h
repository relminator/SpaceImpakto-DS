/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.phatcode.net

	Engine class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <gl2d.h>
#include <maxmod9.h>


#include <vector>
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>

#include "UTIL.h"
#include "Button.h"
#include "MemoryFeedback.h"
#include "Vector2df32.h"
#include "cearn_atan.h"
#include "GLFont.h"
#include "OamFont.h"
#include "Level.h"

// ------------ 3D Stuff --------------
#include "VertexBuffer.h"
#include "SuperShape.h"


// ------------ Singletons --------------
#include "Sound.h"
#include "Fx3D.h"
#include "Fx2D.h"
#include "Explosion.h"
#include "Palettes.h"

// ------------ Player --------------
#include "Shot.h"
#include "PlayerShip.h"
#include "Viper.h"
#include "R9.h"
#include "VicViper.h"
#include "Bomb.h"


// ------------ Bullet Rel --------------

#include "Bullet.h"
#include "Turret.h"


// ------------ Enemies --------------

#include "EntityContainer.h"

#include "PowerUp.h"
#include "BomberUp.h"
#include "ShipUp.h"
#include "BonusCarrier.h"

#include "Popcorn.h"
#include "Rotator.h"
#include "Booger.h"
#include "Looper.h"
#include "Plato.h"
#include "Centipede.h"
#include "ElectricBlob.h"
#include "Worm.h"
#include "Mahorn.h"
#include "BumbleBee.h"
#include "Phoenix.h"
#include "Diver.h"
#include "AbtikBawi.h"
#include "Tictanic.h"
#include "Alienator.h"
#include "Cannoneer.h"
#include "LightBall.h"
#include "Battery.h"
#include "Amoeba.h"
#include "Kalavera.h"
#include "Frisbee.h"

// ------------ Bossses --------------

#include "Boss.h"
#include "PredatorBoss.h"
#include "TentakillBoss.h"
#include "TranspormaBoss.h"
#include "MonkatBoss.h"
#include "GluteusBoss.h"

// ------------ GRIT Images --------------

#include "viper_sprite.h"
#include "r9_sprite.h"
#include "vic_sprite.h"
#include "bullets_sprite.h"
#include "explosions_sprite.h"
#include "flares_sprite.h"


#include "powerup_sprite.h"
#include "bomberup_sprite.h"
#include "shipup_sprite.h"

#include "blob_sprite.h"
#include "enemies_sprite.h"
#include "centipede_segments_sprite.h"
#include "centipede_head_sprite.h"
#include "mmx_shot_sprite.h"
#include "tentakillboss_sprite.h"
#include "transpormaboss_sprite.h"
#include "monkatboss_sprite.h"
#include "gluteus_boss_sprite.h"

// --------------Level-----------------------
#include "mode7.h"
#include "stage02bg.h"
#include "stage03bg.h"
#include "stage04bg.h"
#include "organ.h"

#include "font_si.h"
#include "font_si_16x16.h"
#include "font_oam.h"
#include "font_oam_big.h"

#include "plasma_pal.h"
#include "plasma_pal2.h"
#include "plasma_pal3.h"
#include "plasma_pal4.h"
#include "plasma_pal5.h"
#include "plasma_pal6.h"


// Misc
#include "si_ds_title.h"
#include "bluecross.h"
#include "sepiacircle.h"
#include "cockpit.h"

#include "logo.h"
#include "hblogo.h"
#include "gbatemplogo.h"

// ------------ MAXMOD headers --------------

#include "soundbank_bin.h"
#include "soundbank.h"


// ------------ DEMO ------------------------

#include "demo_bin.h"


#ifndef ENGINE_H
#define ENGINE_H

class Engine 
{

public:
	// ------------ HighScore Stuff --------------
	struct HighScore
	{
		char Name[16];
		char Ship[16];
		int Score;
		int ShipID;
		
	};
	
	
//	struct GameStates
//	{
//		bool DrawAABB;
//		bool DrawDebug;
//		bool PressStart;
//		bool GameOver;
//		int CanPressStartCounter;
//		int CanContinueCounter;
//		bool PrintGameOver;		
//	};
	
	Engine();
	~Engine();

	void PlayGame();
	
private:
	void InitHardware();
	void InitEverything();
	void InitSubScreen();
	void LoadSounds();
	void LoadFonts();
	void LoadImages();
	void Load3D();
	
	
	void GameInitAllEntities( std::vector<PlayerShip*> &Players,
							  Bomb &Bombs,
							  BulletArray &Bullets,
							  TurretArray &Turrets,
							  std::vector<EntityContainer*> &Enemies,
							  std::vector<Boss*> &Bosses );

	void GameUpdateAll( int Frame,
						int KeyH,
						int KeyD,
						int Stage,
						bool &StageFinished,
						Level &Lvl,
						PlayerShip &Ship,
						Bomb &Bombs,
						BulletArray &Bullets,
						TurretArray &Turrets,
						PowerUpContainer &PowerUps,
						std::vector<EntityContainer*> &Enemies,
						Boss &Bossing );
						
	void GameDrawAll( int Frame,
					  int Stage,
					  bool FadeOut,
					  s32 &FadeInterp,
					  s32 &BGInterp,
					  s32 &FontInterp,
					  Level &Lvl,
					  const PlayerShip &Ship,
					  const Bomb &Bombs,
					  BulletArray &Bullets,
					  std::vector<EntityContainer*> &Enemies,
					  Boss &Bossing,
					  bool DrawAABB,
					  bool DrawDebug,
					  bool IsPaused,
					  bool PressStart,
					  bool GameOver,
					  int CanPressStartCounter,
					  int CanContinueCounter,
					  bool &PrintGameOver,
					  int &YorNActive );

	void GameDrawDebug( int Frame,
						int Stage,
						const PlayerShip &Ship,
						const Bomb &Bombs,
						BulletArray &Bullets,
						std::vector<EntityContainer*> &Enemies,
						Boss &Bossing );
						
	int  GamePause( int Frame,
					int Stage,
					bool FadeOut,
					s32 &FadeInterp,
					s32 &BGInterp,
					s32 FontInterp,
					Level &Lvl,
				    const PlayerShip &Ship,
				    BulletArray &Bullets,
					const Bomb &Bombs,
					std::vector<EntityContainer*> &Enemies,
					Boss &Bossing );	
	
	void GameDoSubScreen( int Frame,
						  int Stage,
						  const PlayerShip &Ship,
						  s32 &BGInterp, 
						  bool FadeOut,
						  bool RotatePal );

	int MainMenu( int &GameMode, bool &IsReplay );
	int Menu();
	int Replay( PlayerShip &Ship, int &GameMode, int &Stage );
	int TextEntry(int x, int y, int width, const char *filler, int length,char *entry, GLFont &Font, GLFont &TitleFont);
	void PrintHighScore( HighScore &PlayerStat, int GameMode );
	void PrintHighScore();
	void Options();
	void Credits();
	void Ending( int LoopNumber );
	void Exit();
	void Splash();
	void Story();
	int SelectPlayer();
	void Intermission( int Stage, PlayerShip &Ship );
	void PrintKeyMapping( int Keys[3], const char *MenuText[], const char *Text[],
						  int RowCoords[], int StartRow, int StartColumn,
						  int ColumnOffset );
	void EditControls( int Keys[3], int Choices[6], const char *Text[], 
					   const char *MenuText[], int RowCoords[] );
	
	void KeyConfig();
	
	void SaveHiScore();
	void LoadHiScore();
	void SetMode( int &GameMode );
	
};

#endif // ENGINE_H
