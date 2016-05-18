#include "Targets.h"

Targets* Targets::ms_instance = 0;

Targets::Targets()
{
}

Targets::~Targets()
{
}

Targets* Targets::Instance()
{
	if(ms_instance == 0){
		ms_instance = new Targets();
	}
	return ms_instance;
}

void Targets::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}

