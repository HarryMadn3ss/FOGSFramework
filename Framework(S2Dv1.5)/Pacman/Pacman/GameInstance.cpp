#include "GameInstance.h"

#include <sstream>
#include <iostream>
#include <Windows.h>

using namespace std;

GameInstance::GameInstance(int argc, char* argv[]) : Game(argc, argv), _cSpeed(0.1f), _cFrameTime(250), _cCollectableFrameTime(500), _cProjectileFrameTime(500)
{

	for (int i = 0; i < COLLECTABLECOUNT; i++)
	{
		_collectable[i] = new Collectable();
		_collectable[i]->_frame = 0;
		_collectable[i]->_currentFrameTime = 0;
	}

	for (int i = 0; i < HEARTCOUNT; i++)
	{
		_heart[i] = new Collectable();
		_heart[i]->_frame = 0;
		_heart[i]->_currentFrameTime = 0;
	}

	_player = new Player();
	_player->_dead = false;
	_player->_score = 0;
	_player->_health = 3;
	_player->_invincible = false;
	_player->_isFiring = false;
	_player->_cooldownBetShot = 500;
	
	_mainMenu = new MainMenu();
	_mainMenu->_start = true;

	_background = new Background;
	_background->_texture = new Texture2D();
	

	_pauseMenu = new PauseMenu();
	_pauseMenu->_paused = false;
	_pauseMenu->_pKeyDown = false;

	
	for (int i = 0; i < PROJECTILECOUNT; i++) {
	_projectile[i] = new Projectile();	
	_projectile[i]->_frame = 0;
	_projectile[i]->_currentFrame = 0;
	}

	
	
	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		_enemy[i] = new ChasingEnemy();
		_enemy[i]->_direction = 0;
		_enemy[i]->_speed = 0.1f;
		_enemy[i]->_health = 2;
		_enemy[i]->_rotation = 0;
		_enemy[i]->_scale = 1;
		_enemy[i]->_color = new Color(1, 1, 1, 1);
		_enemy[i]->_origin = new Vector2(32, 32);
	}
	

	_player->_frame = 0;
	_player->_currentFrameTime = 0;
	_player->_speedMultiplier = 1.0f;


	
	_soundManager = new SoundManager;	//initialise all sounds in the SoundManager struct
	

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Cold Circle London", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

GameInstance::~GameInstance()
{
	delete _player->_texture;
	delete _player->_sourceRect;	
	delete _player;
	_player = nullptr;


	for (int i = 0; i < PROJECTILECOUNT; i++) {
		delete _projectile[i]->_texture;
		delete _projectile[i]->_position;
		delete _projectile[i]->_sourceRect;
		delete _projectile[i];
		_projectile[i] = nullptr;
	}
	
	delete _soundManager->_coin;
	delete _soundManager->_gunShot;
	delete _soundManager->_playerHurt;
	delete _soundManager->_dead;
	delete _soundManager->_enemyHurt;
	delete _soundManager->_heart;
	delete _soundManager->_enemySpawn;
	delete _soundManager;
	_soundManager = nullptr;

	for (int i = 0; i < COLLECTABLECOUNT; i++) {
		delete _collectable[i]->_texture;
		delete _collectable[i]->_rect;
		delete _collectable[i]->_position;
		delete _collectable[i];
		_collectable[i] = nullptr;
	}

	for (int i = 0; i < HEARTCOUNT; i++) {
		delete _heart[i]->_texture;
		delete _heart[i]->_rect;
		delete _heart[i]->_position;
		delete _heart[i];
		_heart[i] = nullptr;
	}

	for (int i = 0; i < ENEMYCOUNT; i++) {
		delete _enemy[i]->_texture;
		delete _enemy[i]->_sourceRect;
		delete _enemy[i]->_position;
		delete _enemy[i]->_color;
		delete _enemy[i]->_origin;
		delete _enemy[i];
		_enemy[i] = nullptr;
	}

	delete _pauseMenu->_menuBackground;
	delete _pauseMenu->_menuRectangle;
	delete _pauseMenu->_menuStringPosition;
	delete _pauseMenu;
	_pauseMenu = nullptr;
	delete _mainMenu->_mainMenuBackground;
	delete _mainMenu->_mainMenuRectangle;
	delete _mainMenu->_mainMenuStringPosition;
	delete _mainMenu;
	_mainMenu = nullptr;
	delete _background->_sourceRect;
	delete _background->_texture;
	_background = nullptr;
}

void GameInstance::LoadContent()
{
	//seeding rand
	srand(time(NULL));

	// Load Player
	_player->_texture = new Texture2D();
	_player->_texture->Load("Textures/player.png", false);
	_player->_position = new Vector2(350.0f, 350.0f);
	_player->_sourceRect = new Rect(0.0f, 0.0f, 40, 40);


	//load projectile
	Texture2D* _bulletTex = new Texture2D();
	_bulletTex->Load("Textures/bullet.png", false);

	for (int i = 0; i < PROJECTILECOUNT; i++) {
		_projectile[i]->speedY = 0;
		_projectile[i]->speedX = 0;
		_projectile[i]->_texture = _bulletTex;
		_projectile[i]->_position = new Vector2(350, 350);
		_projectile[i]->_sourceRect = new Rect(0.0f, 0.0f, 10, 10);	
		_projectile[i]->_beenFired = false;
	}	

	//load enemy
	Texture2D* enemyTex = new Texture2D();
	enemyTex->Load("Textures/zombie.png", false);

	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		_enemy[i]->_texture = enemyTex;		
		_enemy[i]->_position = new Vector2(10000, 10000);
		_enemy[i]->_sourceRect = new Rect(0.0f, 0.0f, 64, 64);
	}

	

	// Load Collectable
	Texture2D* collectableTex = new Texture2D();
	collectableTex->Load("Textures/Coin.png", false);

	for (int i = 0; i < COLLECTABLECOUNT; i++)
	{
		_collectable[i]->_texture = collectableTex;
		_collectable[i]->_position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));		
		_collectable[i]->_rect = new Rect(0.0f, 0.0f, 32, 32);
		
	}

	CheckOverlapCollectable();

	Texture2D* heartTex = new Texture2D();
	heartTex->Load("Textures/Heart.png", false);

	for (int i = 0; i < HEARTCOUNT; i++)
	{
		_heart[i]->_texture = heartTex;
		_heart[i]->_position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_heart[i]->_rect = new Rect(0.0f, 0.0f, 32, 32);
		
	}

	//load Audio
	_soundManager->Load();

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Pause Menu
	_pauseMenu->_menuBackground = new Texture2D();

	_pauseMenu->_menuBackground->Load("Textures/Transparency.png", false);

	_pauseMenu->_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	_pauseMenu->_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//Main Menu
	_mainMenu->_mainMenuBackground = new Texture2D();

	_mainMenu->_mainMenuBackground->Load("Textures/Transparency.png", false);

	_mainMenu->_mainMenuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	_mainMenu->_mainMenuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//background
	_background->_sourceRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_background->_texture->Load("Textures/background.png", false);
}

void GameInstance::Update(int elapsedTime)
{

	if (!_mainMenu->_start) {
		gameRunTime += static_cast<float>(elapsedTime) / 1000;
		timeSinceSpawn += static_cast<float>(elapsedTime) / 1000;

		if (timeSinceSpawn >= 2)
		{
			for(int i = 0; i < ENEMYCOUNT; i++)
			{
				if (_enemy[i]->isActive != true)
				{
					_enemy[i]->isActive = true;
					_enemy[i]->_position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
					_soundManager->Play(SoundManager::SOUND_NAMES::ENEMYSPAWN);
					timeSinceSpawn = 0;
					break;
				}
			}
		}
	}
	

	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::SPACE)) {
		_mainMenu->_start = false;
	}

	if (!_mainMenu->_start) {


		if (keyboardState->IsKeyDown(Input::Keys::P) && !_pauseMenu->_pKeyDown) {
			_pauseMenu->_pKeyDown = true;
			_pauseMenu->_paused = !_pauseMenu->_paused;
		}

		if (keyboardState->IsKeyUp(Input::Keys::P)) {
			_pauseMenu->_pKeyDown = false;
		}
		if (!_pauseMenu->_paused) {
			if (!_player->_dead) {
				Input(elapsedTime, keyboardState, mouseState);				
			}			
			UpdatingPlayer(elapsedTime);
			UpdatingCollectable(elapsedTime);

			for(int i = 0; i < ENEMYCOUNT; i++)
			{
				_enemy[i]->updateEnemy(_player, elapsedTime);
			}

			
			CheckSimpleEnemyCollision();
			CheckCollectableCollision();
			UpdatingHeartCollectable(elapsedTime);
			CheckHeartCollision();
			BulletCollision();
			for (int i = 0; i < PROJECTILECOUNT; i++) {
				UpdateBullet(_projectile[i], elapsedTime);
			}
			
		}
	}
}

void GameInstance::Draw(int elapsedTime)
{
	// player coords
	std::stringstream stream;
	stream << "Player X: " << _player->_position->X << " Y: " << _player->_position->Y << " Player Score: " << _player->_score << " Player Health: " << _player->_health;

	SpriteBatch::BeginDraw(); // Starts Drawing

	SpriteBatch::Draw(_background->_texture, _background->_sourceRect, nullptr);
	
	SpriteBatch::Draw(_player->_texture, _player->_position, _player->_sourceRect);

	for (int i = 0; i < PROJECTILECOUNT; i++) {
		if (_projectile[i]->_beenFired) {
			SpriteBatch::Draw(_projectile[i]->_texture, _projectile[i]->_position, _projectile[i]->_sourceRect);
		}
		
	}
	

	for (int i = 0; i < COLLECTABLECOUNT; i++) {
		SpriteBatch::Draw(_collectable[i]->_texture, _collectable[i]->_position, _collectable[i]->_rect); //Draw Collectable
	}

	for (int i = 0; i < HEARTCOUNT; i++) {
		SpriteBatch::Draw(_heart[i]->_texture, _heart[i]->_position, _heart[i]->_rect);
	}

	for (int i = 0; i < ENEMYCOUNT; i++) {
		SpriteBatch::Draw(_enemy[i]->_texture, _enemy[i]->_position, _enemy[i]->_sourceRect, _enemy[i]->_origin, _enemy[i]->_scale, _enemy[i]->_rotation, _enemy[i]->_color, SpriteEffect::NONE);
	}

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

	// Pause menu
	if (_pauseMenu->_paused) {
		std::stringstream menuStream;
		menuStream << "Paused!";

		SpriteBatch::Draw(_pauseMenu->_menuBackground, _pauseMenu->_menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _pauseMenu->_menuStringPosition, Color::Red);
	}
	if (_mainMenu->_start) {
		std::stringstream menuStream;
		menuStream << "Press Spacebar to begin";

		SpriteBatch::Draw(_pauseMenu->_menuBackground, _pauseMenu->_menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _pauseMenu->_menuStringPosition, Color::Red);
	}

	

	SpriteBatch::EndDraw(); // Ends Drawing
}


void GameInstance::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState) {

	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	

	//Sprint
	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT)) {
		_player->_speedMultiplier = 4.0f;
	}
	else {
		_player->_speedMultiplier = 2.0f;
	}

	// Keyboard


	if (keyboardState->IsKeyDown(Input::Keys::W)) {
		_player->_position->Y -= _cSpeed * elapsedTime * _player->_speedMultiplier;
		_player->_direction = 2;
		_player->_isMoving = true;
	}	
	if (keyboardState->IsKeyDown(Input::Keys::A)) {
		_player->_position->X -= _cSpeed * elapsedTime * _player->_speedMultiplier;
		_player->_direction = 3;
		_player->_isMoving = true;
	}	
	if (keyboardState->IsKeyDown(Input::Keys::S)) {
		_player->_position->Y += _cSpeed * elapsedTime * _player->_speedMultiplier;
		_player->_direction = 0;
		_player->_isMoving = true;
	}
	if (keyboardState->IsKeyDown(Input::Keys::D)) {
		_player->_position->X += _cSpeed * elapsedTime * _player->_speedMultiplier; //Moves Pacman across X axis
		_player->_direction = 1;
		_player->_isMoving = true;
	}
	if (!keyboardState->IsKeyDown(Input::Keys::W) && !keyboardState->IsKeyDown(Input::Keys::A) && !keyboardState->IsKeyDown(Input::Keys::S) && !keyboardState->IsKeyDown(Input::Keys::D)) {
		_player->_isMoving = false;
	}
		
	//_player->_firingCooldown = 0;
	//while (_player->_firingCooldown != 0)
	//{
	//	_player->_firingCooldown++;
	//	if (_player->_firingCooldown >= 10) {
	//		_player->_firingCooldown = 0;
	//	}
	//}

	if (_player->_firingCooldown > 0)
	{		
		if (_player->_firingCooldown < _player->_cooldownBetShot - 250) {
			_player->_isFiring = false;
		}
		_player->_firingCooldown -= elapsedTime;
	}
	//else
	{
		if (keyboardState->IsKeyDown(Input::Keys::UP)/* && _player->_firingCooldown == 0*/) {		
			_player->_direction = 2;
			if (_player->_firingCooldown <= 0)
			{
				_player->_isFiring = true;
				_player->_bulletCount++;
				CreateBullet(elapsedTime);
				_player->_firingCooldown = _player->_cooldownBetShot; //10 seconds from the 16ms per frame
			}
		}
		else if (keyboardState->IsKeyDown(Input::Keys::LEFT)/* && _player->_firingCooldown == 0*/) {
			_player->_direction = 3;
			if (_player->_firingCooldown <= 0)
			{
				_player->_isFiring = true;
				_player->_bulletCount++;
				CreateBullet(elapsedTime);
				_player->_firingCooldown = _player->_cooldownBetShot; //10 seconds from the 16ms per frame
			}
		}
		else if (keyboardState->IsKeyDown(Input::Keys::DOWN)/* && _player->_firingCooldown == 0*/) {
			_player->_direction = 0;
			if (_player->_firingCooldown <= 0)
			{
				_player->_isFiring = true;
				_player->_bulletCount++;
				CreateBullet(elapsedTime);
				_player->_firingCooldown = _player->_cooldownBetShot; //10 seconds from the 16ms per frame
			}
		}
		else if (keyboardState->IsKeyDown(Input::Keys::RIGHT)/* && _player->_firingCooldown == 0*/) {
			_player->_direction = 1;
			if (_player->_firingCooldown <= 0)
			{
				_player->_isFiring = true;
				_player->_bulletCount++;
				CreateBullet(elapsedTime);
				_player->_firingCooldown = _player->_cooldownBetShot; //10 seconds from the 16ms per frame
			}
		}
		else if (!keyboardState->IsKeyDown(Input::Keys::UP) && !keyboardState->IsKeyDown(Input::Keys::LEFT) && !keyboardState->IsKeyDown(Input::Keys::DOWN) && !keyboardState->IsKeyDown(Input::Keys::RIGHT)) {
			_player->_isFiring = false;		
		}
	}

	

	//Mouse
	if (mouseState->LeftButton == Input::ButtonState::PRESSED) {
		//
		/*_collectable->_position->X = mouseState->X;
		_collectable->_position->Y = mouseState->Y;*/
	}
}

void GameInstance::UpdatingPlayer(int elapsedTime) {
	_player->_currentFrameTime += elapsedTime;
	
	if (!_player->_invincible && !_player->_isFiring) {
		if (_player->_currentFrameTime > _cFrameTime) {

			_player->_frame++;

			if (_player->_frame >= 2) {
				_player->_frame = 0;
			}

			_player->_currentFrameTime = 0;
		}
	}
	else if(_player->_invincible)
	{
		if (_player->_currentFrameTime > _cFrameTime) {
			

			_player->_frame += 4;

			if (_player->_frame > 4)
			{
				_player->_frame = 0;
			}

			_player->_currentFrameTime = 0.0f;
			_player->_iCurrentFrameTime += 0.5;
			if (_player->_iCurrentFrameTime >= 3.0f) {
				_player->_invincible = false;
				_player->_iCurrentFrameTime = 0;
			}
		}
	}
	else if (_player->_isFiring) {
		if (_player->_currentFrameTime > _cFrameTime) {


			_player->_frame += 3;
			_soundManager->Play(SoundManager::SOUND_NAMES::GUNSHOT);

			if (_player->_frame >= 4)
			{
				_player->_frame = 0;
			}

			_player->_currentFrameTime = 0.0f;
			_player->_iCurrentFrameTime += 0.5;
			if (_player->_iCurrentFrameTime >= 3.0f) {
				_player->_iCurrentFrameTime = 0;
			}
		}
	}
	else if (_player->_dead) {
		_player->_frame = 5;
	}
	
	CheckPlayerWallCollison();
	

	//changing the direction of the player
	if (_player->_isMoving || _player->_invincible || _player->_isFiring) {
		_player->_sourceRect->X = _player->_sourceRect->Width * _player->_direction;
		_player->_sourceRect->Y = _player->_sourceRect->Height * _player->_frame;
	}
	if (_player->_dead) {
		_player->_sourceRect->X = _player->_sourceRect->Height * _player->_direction;
		_player->_sourceRect->Y = _player->_sourceRect->Width * 5;
	}
	
}

void GameInstance::CheckPlayerWallCollison() {
	
	if (_player->_position->X + _player->_sourceRect->Width > Graphics::GetViewportWidth()) {
		_player->_position->X = Graphics::GetViewportWidth() - _player->_sourceRect->Width;
	}

	if (_player->_position->X < 0) {
		_player->_position->X = 0;
	}

	if (_player->_position->Y < 0) {
		_player->_position->Y = 0;
	}

	if (_player->_position->Y + _player->_sourceRect->Height > Graphics::GetViewportHeight()) {
		_player->_position->Y = Graphics::GetViewportHeight() - _player->_sourceRect->Height;
	}
}

void GameInstance::UpdatingCollectable(int elapsedTime) {
	//animating the collectable

	for (int i = 0; i < COLLECTABLECOUNT; i++) {


		_collectable[i]->_currentFrameTime += elapsedTime;

		if (_collectable[i]->_currentFrameTime > _cCollectableFrameTime) {
			_collectable[i]->_frame++;

			if (_collectable[i]->_frame >= 6) {
				_collectable[i]->_frame = 0;
			}

			_collectable[i]->_currentFrameTime = 0;

		}

		//Changing the colectable sheet

		_collectable[i]->_rect->X = _collectable[i]->_rect->Width * _collectable[i]->_frame;
	}
}

void GameInstance::UpdatingHeartCollectable(int elapsedTime) {

	for (int i = 0; i <	HEARTCOUNT; i++) {

		_heart[i]->_currentFrameTime += elapsedTime;

		if (_heart[i]->_currentFrameTime > _cCollectableFrameTime) {
			_heart[i]->_frame++;

			if (_heart[i]->_frame >= 3) {
				_heart[i]->_frame = 0;
			}

			_heart[i]->_currentFrameTime = 0;

		}

		//Changing the colectable sheet

		_heart[i]->_rect->X = _heart[i]->_rect->Width * _heart[i]->_frame;
	}
}

//void GameInstance::updateEnemy(Enemy* ghost, int elapsedTime) {
//	if (ghost->_direction == 0) {
//		ghost->_position->X += ghost->_speed * elapsedTime;
//	}
//	else if (ghost->_direction == 1) {
//		ghost->_position->X -= ghost->_speed * elapsedTime;
//	}
//
//	if (ghost->_position->X + ghost->_sourceRect->Width >= Graphics::GetViewportWidth()) {
//		ghost->_direction = 1;
//	}
//	else if (ghost->_position->X <= 0) {
//		ghost->_direction = 0;
//	}
//}

void GameInstance::CheckSimpleEnemyCollision() {
	int playerTop = _player->_position->Y;
	int playerRight = _player->_position->X + _player->_sourceRect->Width;
	int playerBottom = _player->_position->Y + _player->_sourceRect->Height;
	int playerLeft = _player->_position->X;

	int enemyTop = 0;
	int enemyRight = 0;
	int enemyBottom = 0;
	int enemyLeft = 0;

	if (!_player->_invincible) {
		for (int i = 0; i < ENEMYCOUNT; i++) {
			enemyTop = _enemy[i]->_position->Y;
			enemyRight = _enemy[i]->_position->X + _enemy[i]->_sourceRect->Width;
			enemyBottom = _enemy[i]->_position->Y + _enemy[i]->_sourceRect->Height;
			enemyLeft = _enemy[i]->_position->X;

			if ((playerBottom > enemyTop) && (playerTop < enemyBottom) && (playerLeft < enemyRight) && (playerRight > enemyLeft)) {
				_player->_health -= 1;
				_soundManager->Play(SoundManager::SOUND_NAMES::PLAYERHURT);
				CheckPlayerDead();				
				i = ENEMYCOUNT;
				_player->_invincible = true;
			}
		}
	}
	

}

void GameInstance::CheckCollectableCollision() {
	int playerTop = _player->_position->Y;
	int playerRight = _player->_position->X + _player->_sourceRect->Width;
	int playerBottom = _player->_position->Y + _player->_sourceRect->Height;
	int playerLeft = _player->_position->X;

	int collectableTop = 0;
	int collectableRight = 0;
	int collectableBottom = 0;
	int collectableLeft = 0;

	for (int i = 0; i < COLLECTABLECOUNT; i++) {
		collectableTop = _collectable[i]->_position-> Y;
		collectableRight = _collectable[i]->_position->X + _collectable[i]->_rect->Width;
		collectableBottom = _collectable[i]->_position->Y + _collectable[i]->_rect->Height;
		collectableLeft = _collectable[i]->_position->X;

		if ((playerBottom > collectableTop) && (playerTop < collectableBottom) && (playerLeft < collectableRight) && (playerRight > collectableLeft)) {
			_collectable[i]->_position->Y = 1000;
			_player->_score += 100;
			_soundManager->Play(SoundManager::SOUND_NAMES::COIN);
		}
	}
}

void GameInstance::CheckHeartCollision() {
	int playerTop = _player->_position->Y;
	int playerRight = _player->_position->X + _player->_sourceRect->Width;
	int playerBottom = _player->_position->Y + _player->_sourceRect->Height;
	int playerLeft = _player->_position->X;

	int heartTop = 0;
	int heartRight = 0;
	int heartBottom = 0;
	int heartLeft = 0;

	for (int i = 0; i < HEARTCOUNT; i++) {
		heartTop = _heart[i]->_position->Y;
		heartRight = _heart[i]->_position->X + _heart[i]->_rect->Width;
		heartBottom = _heart[i]->_position->Y + _heart[i]->_rect->Height;
		heartLeft = _heart[i]->_position->X;

		if ((playerBottom > heartTop) && (playerTop < heartBottom) && (playerLeft < heartRight) && (playerRight > heartLeft)) {
			_heart[i]->_position->Y = 1000;
			_player->_health ++;
			_soundManager->Play(SoundManager::SOUND_NAMES::HEART);
		}
	}
}

void GameInstance::CheckPlayerDead() {
	if (_player->_health < 1) {
		_player->_dead = true;
		GameOver();
		_soundManager->Play(SoundManager::SOUND_NAMES::DEAD);
		if (_player->_health < 0) {
			_player->_health = 0;
		}
	}
	
}

void GameInstance::CheckOverlapCollectable() {
	bool isOverlapping = true;

	srand(time(NULL));

	int collectableTop = 0;
	int collectableRight = 0;
	int collectableBottom = 0;
	int collectableLeft = 0;

	int compareCollectableTop = 0;
	int compareCollectableRight = 0;
	int compareCollectableBottom = 0;
	int compareCollectableLeft = 0;
	
	do{
		
		isOverlapping = false;

		for (int i = 0; i < COLLECTABLECOUNT; i++) 
		{
			collectableTop = _collectable[i]->_position->Y;
			collectableRight = _collectable[i]->_position->X + _collectable[i]->_rect->Width;
			collectableBottom = _collectable[i]->_position->Y + _collectable[i]->_rect->Height;
			collectableLeft = _collectable[i]->_position->X;

			for (int j = 0; j < COLLECTABLECOUNT; j++) 
			{
				if (i != j)
				{
					compareCollectableTop = _collectable[j]->_position->Y;
					compareCollectableRight = _collectable[j]->_position->X + _collectable[j]->_rect->Width;
					compareCollectableBottom = _collectable[j]->_position->Y + _collectable[j]->_rect->Height;
					compareCollectableLeft = _collectable[j]->_position->X;

					if ((compareCollectableBottom > collectableTop) && (compareCollectableTop < collectableBottom) && (compareCollectableLeft < collectableRight) && (compareCollectableRight > collectableLeft)) 
					{
						_collectable[i]->_position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
						isOverlapping = true;
					}
				}
			}
		} 		
	}while (isOverlapping);
}

void GameInstance::CreateBullet(int elapsedTime) {
	if (_player->_bulletCount >= PROJECTILECOUNT){
		_player->_bulletCount = 0;
	}

	_projectile[_player->_bulletCount]->_position = new Vector2(_player->_position->X, _player->_position->Y);
	_projectile[_player->_bulletCount]->_direction = _player->_direction;
	_projectile[_player->_bulletCount]->_beenFired = true;
}


void GameInstance::UpdateBullet(Projectile* _bullet, int elapsedTime) {

	if (_bullet->_direction == 0) {
		_bullet->_position->Y += 1 * elapsedTime;		
		
	}
	else if (_bullet->_direction == 1) {
		_bullet->_position->X += 1 * elapsedTime;
				
	}
	if (_bullet->_direction == 3) {
		_bullet->_position->X -= 1 * elapsedTime;
				
	}
	if (_bullet->_direction == 2) {
		_bullet->_position->Y -= 1 * elapsedTime;
				
	}
	
}

void GameInstance::BulletCollision() {
	int bulletTop = 0;
	int bulletRight = 0;
	int bulletBottom = 0;
	int bulletLeft = 0;

	int enemyTop = 0;
	int enemyRight = 0;
	int enemyBottom = 0;
	int enemyLeft = 0;

	
			

		for (int i = 0; i < ENEMYCOUNT; i++)
		{
			enemyTop = _enemy[i]->_position->Y;
			enemyRight = _enemy[i]->_position->X + _enemy[i]->_sourceRect->Width;
			enemyBottom = _enemy[i]->_position->Y + _enemy[i]->_sourceRect->Height;
			enemyLeft = _enemy[i]->_position->X;

			for (int j = 0; j < PROJECTILECOUNT; j++)
			{
				if (_projectile[j]->_beenFired)
				{
					bulletTop = _projectile[j]->_position->Y;
					bulletRight = _projectile[j]->_position->X + _projectile[j]->_sourceRect->Width;
					bulletBottom = _projectile[j]->_position->Y + _projectile[j]->_sourceRect->Height;
					bulletLeft = _projectile[j]->_position->X;

					if ((bulletBottom > enemyTop) && (bulletTop < enemyBottom) && (bulletLeft < enemyRight) && (bulletRight > enemyLeft))
					{
						_projectile[j]->_beenFired = false;
						_projectile[j]->_position = new Vector2(100000, 100000);
						_enemy[i]->_health--;
						_soundManager->Play(SoundManager::SOUND_NAMES::ENEMYHURT);
						if (_enemy[i]->_health <= 0) {
							_enemy[i]->_position = new Vector2(10000, 100000);
							_player->_score += 350;
						}
						
						
					}
				}
			}
		}
	
}



void GameInstance::GameOver() {
	gameOver = true;
}