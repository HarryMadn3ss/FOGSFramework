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
#include <vector>

#define COLLECTABLECOUNT 50
#define HEARTCOUNT 3
#define PROJECTILECOUNT 50
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
	int noBullets;

	bool dead;
	bool isMoving;
	bool isFiring;
	bool invincible;
	
};

struct Projectile {
	Vector2* _position;
	Rect* _sourceRect;
	Texture2D* _texture;
	
	int _frame;
	int _currentFrame;
	int speedX;
	int speedY;
	int velocity;
	
	bool beenFired;
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
	enum SOUND_NAMES{ COIN, HEART, PLAYERHURT, DEAD, ENEMYHURT, GUNSHOT};


	SoundEffect* _coin;
	SoundEffect* _heart;

	SoundEffect* _playerHurt;
	SoundEffect* _dead;

	SoundEffect* _enemyHurt;

	SoundEffect* _gunShot;

	SoundManager() {
		_coin = new SoundEffect;
		_gunShot = new SoundEffect;
		_playerHurt = new SoundEffect;
		_dead = new SoundEffect;
		_enemyHurt = new SoundEffect;
		_heart = new SoundEffect;
	};

	void Load() {
		_coin->Load("Audio/coin.wav");
		_dead->Load("Audio/dead.wav");
		_enemyHurt->Load("Audio/enemyHurt.wav");
		_gunShot->Load("Audio/gunShot.wav");
		_playerHurt->Load("Audio/playerHurt.wav");
		_heart->Load("Audio/heart.wav");
	};

	void Play(SOUND_NAMES soundEffect) {
		switch (soundEffect) {
		case COIN:
			Audio::Play(_coin);
			break;
		case HEART:
			Audio::Play(_heart);
			break;
		case PLAYERHURT:
			Audio::Play(_playerHurt);
			break;
		case DEAD:
			Audio::Play(_dead);
			break;
		case ENEMYHURT:
			Audio::Play(_enemyHurt);
			break;
		case GUNSHOT:
			Audio::Play(_gunShot);
			break;
		}
	}

};

struct MainMenu {
	Texture2D* _mainMenuBackground;
	Rect* _mainMenuRectangle;
	Vector2* _mainMenuStringPosition;
	bool _start;
};

struct PauseMenu {
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _pKeyDown;
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
	Projectile* _projectile[PROJECTILECOUNT];
	MainMenu* _mainMenu;
	PauseMenu* _pauseMenu;


	const float _cSpeed;
	const int _cFrameTime;

	const int _cProjectileFrameTime;

	const int _cCollectableFrameTime;

	// Position for String
	Vector2* _stringPosition;

	//Data for Menu
	

	//Data for MainMenu
	

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

	void CheckPlayerWallCollison();

	void updatingCollectable(int elapsedTime);

	void checkCollectableCollision();

	void checkSimpleEnemyCollision();

	void updateSimpleEnemy(SimpleEnemy*, int elaspedTime);

	void checkPlayerDead();

	void updatingHeartCollectable(int elapsedTime);

	void checkHeartCollision();

	void checkOverlapCollectable();

	void createBullet(int elapsedTime);

	void updateBullet(Projectile*, int elapsedTime);

	void bulletCollision();

	void gameOver();
};