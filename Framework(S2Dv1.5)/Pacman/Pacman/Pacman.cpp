#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cSpeed(0.1f), _cFrameTime(250), _cCollectableFrameTime(500)
{
		
	for (int i = 0; i < COLLECTABLECOUNT; i++) {
		_collectable[i] = new Collectable();
		_collectable[i]->_frame = 0;
		_collectable[i]->_currentFrameTime = 0;
	}

	_player = new Player();

	
			

	_paused = false;
	_pKeyDown = false;
	_start = true;

	_player-> _frame = 0;
	_player-> _currentFrameTime = 0;
	_player->speedMultiplier = 1.0f;

	
	
	

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{	
	delete _player-> _texture;
	delete _player-> _sourceRect;
	delete _player;
	
	for (int i = 0; i < COLLECTABLECOUNT; i++) {
		delete _collectable[i]->_texture;
		delete _collectable[i]->_rect;
	}
	
	delete _collectable;

	delete _menuBackground;
	delete _menuRectangle;
	delete _menuStringPosition;
	delete _mainMenuBackground;
	delete _mainMenuRectangle;
	delete _mainMenuStringPosition;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_player-> _texture = new Texture2D();
	_player-> _texture->Load("Textures/Pacman.tga", false);
	_player-> _position = new Vector2(350.0f, 350.0f);
	_player-> _sourceRect = new Rect(0.0f, 0.0f, 32, 32);


	//seeding rand
	srand(time(NULL));
	// Load Collectable
	Texture2D* collectableTex = new Texture2D();
	collectableTex->Load("Textures/CollectableTexture.png", false);

	for (int i = 0; i < COLLECTABLECOUNT; i++) {
		_collectable[i]->_texture = new Texture2D();
		_collectable[i]->_texture = collectableTex;
		_collectable[i]->_position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_collectable[i]->_rect = new Rect(0.0f, 0.0f, 32, 32);
	}
	

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

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::SPACE)) {
		_start = false;
	}

	if (!_start) {

	
		if (keyboardState->IsKeyDown(Input::Keys::P)&& !_pKeyDown) {
			_pKeyDown = true;
			_paused = !_paused;
		}

		if (keyboardState->IsKeyUp(Input::Keys::P)) {
			_pKeyDown = false;
		}
		if (!_paused) {

			Input(elapsedTime, keyboardState, mouseState);

			updatingPlayer(elapsedTime);

			updatingCollectable(elapsedTime);		

		
		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Player X: " << _player-> _position->X << " Y: " << _player-> _position->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_player-> _texture, _player-> _position, _player-> _sourceRect); // Draws Pacman

	for (int i = 0; i < COLLECTABLECOUNT; i++) {
		SpriteBatch::Draw(_collectable[i]->_texture, _collectable[i]->_position, _collectable[i]->_rect); //Draw Collectable
	}


	/*if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_collectableInvertedTexture, _collectableRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_collectableBlueTexture, _collectableRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		

		if (_frameCount >= 60)
			_frameCount = 0;
	}*/
	
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


void Pacman::Input(int elaspedTime, Input::KeyboardState* state, Input::MouseState* mouseState) {

	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	
	
	//Sprint
	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT)) {
		_player->speedMultiplier = 5.0f;
	}
	else {
		_player->speedMultiplier = 1.0f;
	}

	// Keyboard
	if (keyboardState->IsKeyDown(Input::Keys::D)) {
		_player->_position->X += _cSpeed * elaspedTime * _player->speedMultiplier; //Moves Pacman across X axis
		_player->_direction = 0;
	}
	if (keyboardState->IsKeyDown(Input::Keys::A)) {
		_player->_position->X -= _cSpeed * elaspedTime * _player->speedMultiplier;
		_player->_direction = 2;
	}

	if (keyboardState->IsKeyDown(Input::Keys::W)) {
		_player->_position->Y -= _cSpeed * elaspedTime * _player->speedMultiplier;
		_player->_direction = 3;
	}
	if (keyboardState->IsKeyDown(Input::Keys::S)) {
		_player->_position->Y += _cSpeed * elaspedTime * _player->speedMultiplier;
		_player->_direction = 1;
	}	

	//Mouse
	if (mouseState->LeftButton == Input::ButtonState::PRESSED) {
		//change to attack
		/*_collectable->_position->X = mouseState->X;    
		_collectable->_position->Y = mouseState->Y;*/
	}
}

void Pacman::updatingPlayer(int elapsedTime) {
	_player->_currentFrameTime += elapsedTime;

	if (_player->_currentFrameTime > _cFrameTime) {

		_player->_frame++;

		if (_player->_frame >= 2) {
			_player->_frame = 0;
		}

		_player->_currentFrameTime = 0;
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

	_player->_sourceRect->Y = _player->_sourceRect->Height * _player->_direction;
	_player->_sourceRect->X = _player->_sourceRect->Width * _player->_frame;
}

void Pacman::updatingCollectable(int elapsedTime) {
	//animating the collectable

	for (int i = 0; i < COLLECTABLECOUNT; i++) {

	
	_collectable[i]->_currentFrameTime += elapsedTime;

	if (_collectable[i]->_currentFrameTime > _cCollectableFrameTime) {
		_collectable[i]->_frame++;

		if (_collectable[i]->_frame >= 2) {
			_collectable[i]->_frame = 0;
		}

		_collectable[i]->_currentFrameTime = 0;

	}

	//Changing the colectable sheet

	_collectable[i]->_rect->X = _collectable[i]->_rect->Width * _collectable[i]->_frame;
	}
}