#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

#define COLLECTABLECOUNT 50
#define HEARTCOUNT 3

#define SIMPLEENEMYCOUNT 1

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;



struct Player
{
	Vector2* _position;
	Rect* _sourceRect;
	Texture2D* _texture;

	int _frame;
	int _currentFrameTime;
	int _direction;
	int _iFrame;
	float _iCurrentFrameTime;
	float speedMultiplier;
	int score;
	int health;
	bool dead;
	bool isMoving;
	bool isFiring;
	bool invincible;
	
};

struct Collectable 
{
	Rect* _rect;
	Texture2D* _texture;
	Vector2* _position;
	
	int _frame;
	int _currentFrameTime;
};

struct SimpleEnemy 
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;

	int direction;
	float speed;
};

struct SoundManager 
{
	SoundEffect* _coin;
	SoundEffect* _heart;

	SoundEffect* _playerHurt;
	SoundEffect* _dead;

	SoundEffect* _enemyHurt;

	SoundEffect* _gunShot;
};



// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class GameInstance : public Game  //rename once i have a game title
{
private:
	
	Player* _player;
	Collectable* _collectable[COLLECTABLECOUNT];
	Collectable* _heart[HEARTCOUNT];
	SimpleEnemy* _ghost[SIMPLEENEMYCOUNT];

	const float _cSpeed;
	const int _cFrameTime;

	const int _cCollectableFrameTime;

	// Position for String
	Vector2* _stringPosition;

	//Data for Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _pKeyDown;

	//Data for MainMenu
	Texture2D* _mainMenuBackground;
	Rect* _mainMenuRectangle;
	Vector2* _mainMenuStringPosition;
	bool _start;

	SoundManager* _soundManager;
	
	

	public:
	/// <summary> Constructs the Pacman class. </summary>
	GameInstance(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~GameInstance();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	void Input(int elaspedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	void updatingPlayer(int elapsedTime);

	void updatingCollectable(int elapsedTime);

	void checkCollectableCollision();

	void checkSimpleEnemyCollision();

	void updateSimpleEnemy(SimpleEnemy*, int elaspedTime);

	void checkPlayerDead();

	void updatingHeartCollectable(int elapsedTime);

	void checkHeartCollision();

	void checkOverlapCollectable();
};