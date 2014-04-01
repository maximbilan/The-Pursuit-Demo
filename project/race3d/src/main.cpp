#include "game.h"

// "main" is the S3E entry point
int main()
{
	Game* game = Game::GetInstance();
	
	game->Init();
	game->Run();
	game->Shutdown();
	
	game->FreeInstance();

	return 0;
}