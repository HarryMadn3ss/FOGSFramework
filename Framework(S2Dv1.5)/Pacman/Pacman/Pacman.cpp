#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.1f), _cPlayerFrameTime(250), _cCollectableFrameTime(500)
{
	
	_paused = false;
	_pKeyDown = false;
	_start = true;

	_playerFrame = 0;
	_playerCurrentFrameTime = 0;

	_collectableFrame = 0;
	_collectableCurrentFrameTime = 0;
	

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _playerTexture;
	delete _playerSourceRect;
	delete _collectableTexture;
	
	delete _collectableRect;
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
	_playerTexture = new Texture2D();
	_playerTexture->Load("Textures/Pacman.tga", false);
	_playerPosition = new Vector2(350.0f, 350.0f);
	_playerSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Collectable
	_collectableTexture = new Texture2D();
	_collectableTexture->Load("Textures/CollectableTexture.png", false);
	_collectablePosition = new Vector2(100.0f, 450.0f);
	_collectableRect = new Rect(0.0f, 0.0f, 32, 32);

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

			// Checks if D key is pressed
			if (keyboardState->IsKeyDown(Input::Keys::D)) {
				_playerPosition->X += _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
				_playerDirection = 0;
			}
			if (keyboardState->IsKeyDown(Input::Keys::A)){
				_playerPosition->X -= _cPlayerSpeed * elapsedTime;
			_playerDirection = 2;
		}
				
			if (keyboardState->IsKeyDown(Input::Keys::W)) {
				_playerPosition->Y -= _cPlayerSpeed * elapsedTime;
				_playerDirection = 3;
			}
			if (keyboardState->IsKeyDown(Input::Keys::S)) {
				_playerPosition->Y += _cPlayerSpeed * elapsedTime;
				_playerDirection = 1;
			}




			//animating the player

			_playerCurrentFrameTime += elapsedTime;

			if (_playerCurrentFrameTime > _cPlayerFrameTime) {

				_playerFrame++;

				if (_playerFrame >= 2) {
					_playerFrame = 0;
				}

				_playerCurrentFrameTime = 0;
			}


			//animating the collectable

			_collectableCurrentFrameTime += elapsedTime;

			if (_collectableCurrentFrameTime > _cCollectableFrameTime) {
				_collectableFrame++;

				if (_collectableFrame >= 2) {
					_collectableFrame = 0;
				}

				_collectableCurrentFrameTime = 0;

			}

			//changing the direction of the player

			_playerSourceRect->Y = _playerSourceRect->Height * _playerDirection;
			_playerSourceRect->X = _playerSourceRect->Width * _playerFrame;

			//Changing the colectable sheet

			_collectableRect->X = _collectableRect->Width * _collectableFrame;




			/*switch (_playerDirection) {
			case 1: //right
				//_playerSourceRect = new Rect(0.0f, 0.0f, 32, 32);
				_playerSourceRect->Y = 0.0f;
				break;
			case 2: //left
				//_playerSourceRect = new Rect(0.0f,64.0f, 32, 32);
				_playerSourceRect->Y = 64.0f;
				break;
			case 3://up
				//_playerSourceRect = new Rect(0.0f, 96.0f, 32, 32);
				_playerSourceRect->Y = 96.0f;
				break;
			case 4://down
				//_playerSourceRect = new Rect(0.0f, 32.0f, 32, 32);
				_playerSourceRect->Y = 32.0f;
				break;
			}*/
	//Pause
	

	/*
	//Coliding with walls
	//right
	if (_pacmanPosition->X + _pacmanSourceRect->Width > 1024) {
		_pacmanPosition->X = 1024 - _pacmanSourceRect->Width;
	}
	//left
	if (_pacmanPosition->X < 0) {
		_pacmanPosition->X = 0;
	}
	//up
	if (_pacmanPosition->Y < 0) {
		_pacmanPosition->Y = 0;
	}
	//down
	if (_pacmanPosition->Y + _pacmanSourceRect->Height > 768) {
		_pacmanPosition->Y = 768 - _pacmanSourceRect->Height;
	}
*/



	//Getting pacman to wrap around the screen
	//right
			if (_playerPosition->X + _playerSourceRect->Width > Graphics::GetViewportWidth()) {
				_playerPosition->X = 0 * Graphics::GetViewportWidth();
			}
	//left
			if (_playerPosition->X < 0 * Graphics::GetViewportWidth()) {
				_playerPosition->X = Graphics::GetViewportWidth() - _playerSourceRect->Width;
			}
	//up
			if (_playerPosition->Y < 0 * Graphics::GetViewportHeight()) {
				_playerPosition->Y = Graphics::GetViewportHeight() - _playerSourceRect->Height;
				}
	//down
			if (_playerPosition->Y + _playerSourceRect->Height > Graphics::GetViewportHeight()) {
				_playerPosition->Y = 0 * Graphics::GetViewportHeight();
			}
			//_frameCount++;
	
		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Player X: " << _playerPosition->X << " Y: " << _playerPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_playerTexture, _playerPosition, _playerSourceRect); // Draws Pacman


	SpriteBatch::Draw(_collectableTexture, _collectablePosition, _collectableRect); //Draw Collectable



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