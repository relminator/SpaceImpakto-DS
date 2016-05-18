/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Viper class


*******************************************************************************
******************************************************************************/

#include "PlayerShip.h"

#ifndef VIPER_H
#define VIPER_H


class Viper : public PlayerShip
{
public:
	
	virtual void Init( int Palette, glImage* const Sprites );
	virtual bool Update( int KeyH, int KeyD, Bomb &Bomber  );
	virtual bool Replay( int Frame, Bomb &Bomber, int Stage );
	virtual void Draw() const;
	virtual void DrawShots() const;
	virtual void DrawAABB( int color ) const;
	
	
private:
	
	void SpawnNormalShot();
	void SpawnWaveShot();
	void SpawnMissile();
	void Animate();
	
		
};





#endif // VIPER_H
