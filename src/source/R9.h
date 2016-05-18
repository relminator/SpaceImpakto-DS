/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	R9 class


*******************************************************************************
******************************************************************************/

#include "PlayerShip.h"

#ifndef R9_H
#define R9_H



class R9 : public PlayerShip
{
public:
	
	virtual void Init( int Palette, glImage* const Sprites );
	virtual bool Update( int KeyH, int KeyD, Bomb &Bomber  );
	virtual bool Replay( int Frame, Bomb &Bomber, int Stage );
	virtual void Draw() const;
	virtual void DrawShots() const;
	virtual void DrawAABB( int color ) const;
	
	
private:
		
	void SpawnSpreadShot();
	void SpawnMissile();
	void Animate();
	
	
		
};



#endif // R9_H
