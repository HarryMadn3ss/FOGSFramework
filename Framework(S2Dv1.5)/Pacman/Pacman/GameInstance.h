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
#define ENEMYCOUNT 50

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
	float _speedMultiplier;
	int _score;
	int _health;
	int _bulletCount;

	float _firingCooldown;
	float _cooldownBetShot;


	bool _dead;
	bool _isMoving;
	bool _isFiring;
	bool _invincible;
	
	
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
	int _direction;

	

	bool _beenFired;
};

struct Collectable 
{
	Rect* _rect;
	Texture2D* _texture;
	Vector2* _position;
	
	int _frame;
	int _currentFrameTime;
};

struct Enemy 
{
	Vector2* _position;
	Vector2* _origin;
	Texture2D* _texture;
	Rect* _sourceRect;
	Color* _color;

	int _health;

	int _direction;
	float _speed;

	bool isActive;

	float _scale;
	float _rotation;

	int _frame = 0;
	int _currentFrameTime = 0;
	const int _cFrameTime = 8000;
	
	virtual void updateEnemy(Player* _player, int elapsedTime) {
		if (isActive) {
			if (_direction == 0) {
				_position->X += _speed * elapsedTime;
			}
			else if (_direction == 1) {
				_position->X -= _speed * elapsedTime;
			}

			if (_position->X + _sourceRect->Width >= Graphics::GetViewportWidth()) {
				_direction = 1;
			}
			else if (_position->X <= 0) {
				_direction = 0;
			}
		}
		
	}
};

struct ChasingEnemy : public Enemy {

	ChasingEnemy()
	{
		_scale = 1;
		_rotation = 0;
	}

	void updateEnemy(Player* _player, int elapsedTime){
		if (isActive) {
			Vector2 direction = *_player->_position - *_position;
			direction.Normalize();

			*_position += direction * (_speed * elapsedTime);

			if (direction.X > 0)
			{
				_rotation = MathHelper::ToDegrees(Vector2::Dot(Vector2(0, -1), direction));
			}
			else
			{
				_rotation = MathHelper::ToDegrees(Vector2::Dot(Vector2(0, -1), direction * -1)) + 180;
			}

			for (int i = 0; i < ENEMYCOUNT; i++) {


				_currentFrameTime += elapsedTime;

				if (_currentFrameTime > _cFrameTime) {
					_frame++;

					if (_frame >= 4) {
						_frame = 0;
					}

					_currentFrameTime = 0;

				}
								
				_sourceRect->X = _sourceRect->Width * _frame;
			}
		}
	};
};

struct Background {
	Rect* _sourceRect;
	Texture2D* _texture;
};

struct SoundManager 
{
	enum SOUND_NAMES{ COIN, HEART, PLAYERHURT, DEAD, ENEMYHURT, GUNSHOT, ENEMYSPAWN};


	SoundEffect* _coin;
	SoundEffect* _heart;

	SoundEffect* _playerHurt;
	SoundEffect* _dead;

	SoundEffect* _enemyHurt;
	SoundEffect* _enemySpawn;

	SoundEffect* _gunShot;

	SoundManager() {
		_coin = new SoundEffect;
		_gunShot = new SoundEffect;
		_playerHurt = new SoundEffect;
		_dead = new SoundEffect;
		_enemyHurt = new SoundEffect;
		_heart = new SoundEffect;
		_enemySpawn = new SoundEffect;
	};

	void Load() {
		_coin->Load("Audio/coin.wav");
		_dead->Load("Audio/dead.wav");
		_enemyHurt->Load("Audio/enemyHurt.wav");
		_gunShot->Load("Audio/gunShot.wav");
		_playerHurt->Load("Audio/playerHurt.wav");
		_heart->Load("Audio/heart.wav");
		_enemySpawn->Load("Audio/enemySpawn.wav");
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
		case ENEMYSPAWN:
			Audio::Play(_enemySpawn);
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
class GameInstance : public Game  
{
private:
	
	Player* _player;
	Collectable* _collectable[COLLECTABLECOUNT];
	Collectable* _heart[HEARTCOUNT];
	Enemy* _enemy[ENEMYCOUNT];
	Projectile* _projectile[PROJECTILECOUNT];
	MainMenu* _mainMenu;
	MainMenu* _gameOverScreen;
	PauseMenu* _pauseMenu;
	Background* _background;

	bool quit;
	bool restart;

	float gameRunTime;
	float timeSinceSpawn = 0;

	bool gameOver = false;

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

	void UpdatingPlayer(int elapsedTime);

	void CheckPlayerWallCollison();

	void UpdatingCollectable(int elapsedTime);

	void CheckCollectableCollision();

	void CheckSimpleEnemyCollision();

	/*void updateEnemy(int elaspedTime);*/

	void CheckPlayerDead();

	void UpdatingHeartCollectable(int elapsedTime);

	void CheckHeartCollision();

	void CheckOverlapCollectable();

	void CreateBullet(int elapsedTime);

	void UpdateBullet(Projectile*, int elapsedTime);

	void BulletCollision();

	void GameOver();

	
};