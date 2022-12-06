#include "GameInstance.h"

#include <sstream>
#include <iostream>

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
	_player->dead = false;
	_player->score = 0;
	_player->health = 3;
	_player->invincible = false;
	_player->isFiring = false;
	
	
	
	
	for (int i = 0; i < PROJECTILECOUNT; i++) {
	_projectile[i] = new Projectile();	
	_projectile[i]->_frame = 0;
	_projectile[i]->_currentFrame = 0;
	}

	
	
	for (int i = 0; i < SIMPLEENEMYCOUNT; i++)
	{
		_ghost[i] = new SimpleEnemy();
		_ghost[i]->direction = 0;
		_ghost[i]->speed = 0.2f;
	}

	_paused = false;
	_pKeyDown = false;
	_start = true;

	_player->_frame = 0;
	_player->_currentFrameTime = 0;
	_player->speedMultiplier = 1.0f;

	_soundManager = new SoundManager;	
	_soundManager->_coin = new SoundEffect;
	_soundManager->_gunShot = new SoundEffect;
	_soundManager->_playerHurt = new SoundEffect;
	_soundManager->_dead = new SoundEffect;
	_soundManager->_enemyHurt = new SoundEffect;
	_soundManager->_heart = new SoundEffect;

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


	for (int i = 0; i < PROJECTILECOUNT; i++) {
		delete _projectile[i]->_texture;
		delete _projectile[i]->_position;
		delete _projectile[i]->_sourceRect;
		delete _projectile[i];
	}
	
	delete _soundManager->_coin;
	delete _soundManager->_gunShot;
	delete _soundManager->_playerHurt;
	delete _soundManager->_dead;
	delete _soundManager->_enemyHurt;
	delete _soundManager->_heart;
	delete _soundManager;

	for (int i = 0; i < COLLECTABLECOUNT; i++) {
		delete _collectable[i]->_texture;
		delete _collectable[i]->_rect;
		delete _collectable[i]->_position;
		delete _collectable[i];
	}

	for (int i = 0; i < HEARTCOUNT; i++) {
		delete _heart[i]->_texture;
		delete _heart[i]->_rect;
		delete _heart[i]->_position;
		delete _heart[i];
	}

	for (int i = 0; i < SIMPLEENEMYCOUNT; i++) {
		delete _ghost[i]->texture;
		delete _ghost[i]->sourceRect;
		delete _ghost[i]->position;
		delete _ghost[i];
	}

	delete _menuBackground;
	delete _menuRectangle;
	delete _menuStringPosition;
	delete _mainMenuBackground;
	delete _mainMenuRectangle;
	delete _mainMenuStringPosition;
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
		_projectile[i]->speed = 0.2;
		_projectile[i]->_texture = _bulletTex;
		_projectile[i]->_position = new Vector2(350, 350);
		_projectile[i]->_sourceRect = new Rect(0.0f, 0.0f, 10, 10);		
	}	

	//load ghost
	Texture2D* ghostTex = new Texture2D();
	ghostTex->Load("Textures/GhostBlue.png", false);

	for (int i = 0; i < SIMPLEENEMYCOUNT; i++)
	{
		_ghost[i]->texture = ghostTex;
		_ghost[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ghost[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
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

	checkOverlapCollectable();

	Texture2D* heartTex = new Texture2D();
	heartTex->Load("Textures/Heart.png", false);

	for (int i = 0; i < HEARTCOUNT; i++)
	{
		_heart[i]->_texture = heartTex;
		_heart[i]->_position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_heart[i]->_rect = new Rect(0.0f, 0.0f, 32, 32);
		
	}

	//load Audio
	_soundManager->_coin->Load("Audio/coin.wav");
	_soundManager->_dead->Load("Audio/dead.wav");
	_soundManager->_enemyHurt->Load("Audio/enemyHurt.wav");
	_soundManager->_gunShot->Load("Audio/gunShot.wav");
	_soundManager->_playerHurt->Load("Audio/playerHurt.wav");
	_soundManager->_heart->Load("Audio/heart.wav");

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Pause Menu
	_menuBackground = new Texture2D();

	_menuBackground->Load("Textures/Transparency.png", false);

	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//Main Menu
	_mainMenuBackground = new Texture2D();

	_mainMenuBackground->Load("Textures/Transparency.png", false);

	_mainMenuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	_mainMenuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void GameInstance::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::SPACE)) {
		_start = false;
	}

	if (!_start) {


		if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown) {
			_pKeyDown = true;
			_paused = !_paused;
		}

		if (keyboardState->IsKeyUp(Input::Keys::P)) {
			_pKeyDown = false;
		}
		if (!_paused) {
			if (!_player->dead) {
				Input(elapsedTime, keyboardState, mouseState);
				
			}			
			updatingPlayer(elapsedTime);
			updatingCollectable(elapsedTime);
			updateSimpleEnemy(_ghost[0], elapsedTime);
			checkSimpleEnemyCollision();
			checkCollectableCollision();
			updatingHeartCollectable(elapsedTime);
			checkHeartCollision();
			
		}
	}
}

void GameInstance::Draw(int elapsedTime)
{
	// player coords
	std::stringstream stream;
	stream << "Player X: " << _player->_position->X << " Y: " << _player->_position->Y << " Player Score: " << _player->score << " Player Health: " << _player->health;

	SpriteBatch::BeginDraw(); // Starts Drawing

	
	SpriteBatch::Draw(_player->_texture, _player->_position, _player->_sourceRect);

	for (int i = 0; i < PROJECTILECOUNT; i++) {
		SpriteBatch::Draw(_projectile[i]->_texture, _projectile[i]->_position, _projectile[i]->_sourceRect);
	}
	

	for (int i = 0; i < COLLECTABLECOUNT; i++) {
		SpriteBatch::Draw(_collectable[i]->_texture, _collectable[i]->_position, _collectable[i]->_rect); //Draw Collectable
	}

	for (int i = 0; i < HEARTCOUNT; i++) {
		SpriteBatch::Draw(_heart[i]->_texture, _heart[i]->_position, _heart[i]->_rect);
	}

	for (int i = 0; i < SIMPLEENEMYCOUNT; i++) {
		SpriteBatch::Draw(_ghost[i]->texture, _ghost[i]->position, _ghost[i]->sourceRect);
	}

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

	// Pause menu
	if (_paused) {
		std::stringstream menuStream;
		menuStream << "Paused!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	if (_start) {
		std::stringstream menuStream;
		menuStream << "Press Spacebar to begin";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}


void GameInstance::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState) {

	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	

	//Sprint
	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT)) {
		_player->speedMultiplier = 5.0f;
	}
	else {
		_player->speedMultiplier = 1.0f;
	}

	// Keyboard


	if (keyboardState->IsKeyDown(Input::Keys::W)) {
		_player->_position->Y -= _cSpeed * elapsedTime * _player->speedMultiplier;
		_player->_direction = 2;
		_player->isMoving = true;
	}	
	if (keyboardState->IsKeyDown(Input::Keys::A)) {
		_player->_position->X -= _cSpeed * elapsedTime * _player->speedMultiplier;
		_player->_direction = 3;
		_player->isMoving = true;
	}	
	if (keyboardState->IsKeyDown(Input::Keys::S)) {
		_player->_position->Y += _cSpeed * elapsedTime * _player->speedMultiplier;
		_player->_direction = 0;
		_player->isMoving = true;
	}
	if (keyboardState->IsKeyDown(Input::Keys::D)) {
		_player->_position->X += _cSpeed * elapsedTime * _player->speedMultiplier; //Moves Pacman across X axis
		_player->_direction = 1;
		_player->isMoving = true;
	}
	if (!keyboardState->IsKeyDown(Input::Keys::W) && !keyboardState->IsKeyDown(Input::Keys::A) && !keyboardState->IsKeyDown(Input::Keys::S) && !keyboardState->IsKeyDown(Input::Keys::D)) {
		_player->isMoving = false;
	}
		
	if (keyboardState->IsKeyDown(Input::Keys::UP)) {		
		_player->_direction = 2;
		_player->isFiring = true;

	}
	else if (keyboardState->IsKeyDown(Input::Keys::LEFT)) {
		_player->_direction = 3;
		_player->isFiring = true;
		playerFiring(elapsedTime);
	}
	else if (keyboardState->IsKeyDown(Input::Keys::DOWN)) {
		_player->_direction = 0;
		_player->isFiring = true;
		playerFiring(elapsedTime);
	}
	else if (keyboardState->IsKeyDown(Input::Keys::RIGHT)) {
		_player->_direction = 1;
		_player->isFiring = true;
		playerFiring(elapsedTime);
	}
	else if (!keyboardState->IsKeyDown(Input::Keys::UP) && !keyboardState->IsKeyDown(Input::Keys::LEFT) && !keyboardState->IsKeyDown(Input::Keys::DOWN) && !keyboardState->IsKeyDown(Input::Keys::RIGHT)) {
		_player->isFiring = false;
		playerFiring(elapsedTime);
	}

	//Mouse
	if (mouseState->LeftButton == Input::ButtonState::PRESSED) {
		//change to attack
		/*_collectable->_position->X = mouseState->X;
		_collectable->_position->Y = mouseState->Y;*/
	}
}

void GameInstance::updatingPlayer(int elapsedTime) {
	_player->_currentFrameTime += elapsedTime;
	
	if (!_player->invincible && !_player->isFiring) {
		if (_player->_currentFrameTime > _cFrameTime) {

			_player->_frame++;

			if (_player->_frame >= 2) {
				_player->_frame = 0;
			}

			_player->_currentFrameTime = 0;
		}
	}
	else if(_player->invincible)
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
				_player->invincible = false;
				_player->_iCurrentFrameTime = 0;
			}
		}
	}
	else if (_player->isFiring) {
		if (_player->_currentFrameTime > _cFrameTime) {


			_player->_frame += 3;
			Audio::Play(_soundManager->_gunShot);

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
	else if (_player->dead) {
		_player->_frame = 5;
	}
	

	//crossing offscreen
	if (_player->_position->X + _player->_sourceRect->Width > Graphics::GetViewportWidth()) {
		_player->_position->X = 0 * Graphics::GetViewportWidth();
	}
	//left
	if (_player->_position->X < 0 * Graphics::GetViewportWidth()) {
		_player->_position->X = Graphics::GetViewportWidth() - _player->_sourceRect->Width;
	}
	//up
	if (_player->_position->Y < 0 * Graphics::GetViewportHeight()) {
		_player->_position->Y = Graphics::GetViewportHeight() - _player->_sourceRect->Height;
	}
	//down
	if (_player->_position->Y + _player->_sourceRect->Height > Graphics::GetViewportHeight()) {
		_player->_position->Y = 0 * Graphics::GetViewportHeight();
	}

	//changing the direction of the player
	if (_player->isMoving || _player->invincible || _player->isFiring) {
		_player->_sourceRect->X = _player->_sourceRect->Width * _player->_direction;
		_player->_sourceRect->Y = _player->_sourceRect->Height * _player->_frame;
	}
	if (_player->dead) {
		_player->_sourceRect->X = _player->_sourceRect->Height * _player->_direction;
		_player->_sourceRect->Y = _player->_sourceRect->Width * 5;
	}
	
}

void GameInstance::updatingCollectable(int elapsedTime) {
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

void GameInstance::updatingHeartCollectable(int elapsedTime) {

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

void GameInstance::updateSimpleEnemy(SimpleEnemy* ghost, int elapsedTime) {
	if (ghost->direction == 0) {
		ghost->position->X += ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 1) {
		ghost->position->X -= ghost->speed * elapsedTime;
	}

	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) {
		ghost->direction = 1;
	}
	else if (ghost->position->X <= 0) {
		ghost->direction = 0;
	}
}

void GameInstance::checkSimpleEnemyCollision() {
	int playerTop = _player->_position->Y;
	int playerRight = _player->_position->X + _player->_sourceRect->Width;
	int playerBottom = _player->_position->Y + _player->_sourceRect->Height;
	int playerLeft = _player->_position->X;

	int enemyTop = 0;
	int enemyRight = 0;
	int enemyBottom = 0;
	int enemyLeft = 0;

	if (!_player->invincible) {
		for (int i = 0; i < SIMPLEENEMYCOUNT; i++) {
			enemyTop = _ghost[i]->position->Y;
			enemyRight = _ghost[i]->position->X + _ghost[i]->sourceRect->Width;
			enemyBottom = _ghost[i]->position->Y + _ghost[i]->sourceRect->Height;
			enemyLeft = _ghost[i]->position->X;

			if ((playerBottom > enemyTop) && (playerTop < enemyBottom) && (playerLeft < enemyRight) && (playerRight > enemyLeft)) {
				_player->health -= 1;
				Audio::Play(_soundManager->_enemyHurt);
				checkPlayerDead();				i = SIMPLEENEMYCOUNT;
				_player->invincible = true;
			}
		}
	}
	

}

void GameInstance::checkCollectableCollision() {
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
			_player->score += 100;
			Audio::Play(_soundManager->_coin);
		}
	}
}

void GameInstance::checkHeartCollision() {
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
			_player->health ++;
			Audio::Play(_soundManager->_heart);
		}
	}
}

void GameInstance::checkPlayerDead() {
	if (_player->health < 1) {
		_player->dead = true;
		Audio::Play(_soundManager->_dead);
		if (_player->health < 0) {
			_player->health = 0;
		}
	}
	
}

void GameInstance::checkOverlapCollectable() {
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
		
		for (int i = 0; i < COLLECTABLECOUNT; i++) {
			collectableTop = _collectable[i]->_position->Y;
			collectableRight = _collectable[i]->_position->X + _collectable[i]->_rect->Width;
			collectableBottom = _collectable[i]->_position->Y + _collectable[i]->_rect->Height;
			collectableLeft = _collectable[i]->_position->X;

			for (int j = 50; j < COLLECTABLECOUNT; j--) {
				compareCollectableTop = _collectable[j]->_position->Y;
				compareCollectableRight = _collectable[j]->_position->X + _collectable[j]->_rect->Width;
				compareCollectableBottom = _collectable[j]->_position->Y + _collectable[j]->_rect->Height;
				compareCollectableLeft = _collectable[j]->_position->X;
			}
			
			

			if ((compareCollectableBottom > collectableTop) && (compareCollectableTop < collectableBottom) && (compareCollectableLeft < collectableRight) && (compareCollectableRight > collectableLeft)) {
				_collectable[i]->_position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
				isOverlapping = true;
			}
			else {
				isOverlapping = false;
			}

		} 		
		
	}while (isOverlapping);
}

void GameInstance::playerFiring(int elapsedTime) {

	/*for (int i = 0; i > PROJECTILECOUNT; i++) {
		_projectile[i]->_position = new Vector2(_player->_position->X, _player->_position->Y);
	}*/

	/*Vector2* _startPosition = new Vector2(_player->_position->X, _player->_position->Y);
		*/
	/*if (_player->isFiring) {*/
		//for (int i = 0; i < PROJECTILECOUNT; i++)
		//{		

		//	if (_player->_direction == 0) {
		//		_projectile[i]->_position = new Vector2(_player->_position->X, ((_player->_position->Y + _projectile[i]->speed) * elapsedTime));
		//		
		//	}
		//	else if (_player->_direction == 1) {
		//		_projectile[i]->_position = new Vector2(((_player->_position->X + _projectile[i]->speed)/* * elapsedTime*/), _player->_position->Y);
		//		
		//	}
		//	else if (_player->_direction == 2) {
		//		_projectile[i]->_position = new Vector2(_player->_position->X, _player->_position->Y);
		//		_projectile[i]->_position->Y -= _projectile[i]->speed * elapsedTime;
		//	}
		//	else if (_player->_direction == 3) {
		//		_projectile[i]->_position = new Vector2(_player->_position->X, _player->_position->Y);
		//		_projectile[i]->_position->Y -= _projectile[i]->speed * elapsedTime;
		//	}
		//}
	//}

	/*_player->_position->X += _cSpeed * elapsedTime * _player->speedMultiplier;*/

	

	for (int i = 0; i < PROJECTILECOUNT; i++)
	{
		if (_player->_direction == 0) {
			_projectile[i]->_position = new Vector2(_player->_position->X, _player->_position->Y);			
			/*_projectile[i]->velocity = _player->_direction * _projectile[i]->speed;*/
			_projectile[i]->_position->Y += _projectile[i]->speed * elapsedTime;

		}
	}
	
		
}