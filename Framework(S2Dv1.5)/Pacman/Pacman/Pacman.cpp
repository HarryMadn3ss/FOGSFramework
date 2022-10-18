#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f)
{
	_frameCount = 0;
	_paused = false;
	_pKeyDown = false;
	_start = true;
	

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
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
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

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
				_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
				_playerDirection = 1;
			}
			else if (keyboardState->IsKeyDown(Input::Keys::A)){
				_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;
			_playerDirection = 2;
		}
				
			else if (keyboardState->IsKeyDown(Input::Keys::W)) {
				_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime;
				_playerDirection = 3;
			}
			else if (keyboardState->IsKeyDown(Input::Keys::S)) {
				_pacmanPosition->Y += _cPacmanSpeed * elapsedTime;
				_playerDirection = 4;
			}
			switch (_playerDirection) {
			case 1: //right
				_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);
				break;
			case 2: //left
				_pacmanSourceRect = new Rect(0.0f,64.0f, 32, 32);
				break;
			case 3://up
				_pacmanSourceRect = new Rect(0.0f, 96.0f, 32, 32);
				break;
			case 4://down
				_pacmanSourceRect = new Rect(0.0f, 32.0f, 32, 32);
				break;
			}
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
			if (_pacmanPosition->X + _pacmanSourceRect->Width > Graphics::GetViewportWidth()) {
				_pacmanPosition->X = 0 * Graphics::GetViewportWidth();
			}
	//left
			if (_pacmanPosition->X < 0 * Graphics::GetViewportWidth()) {
				_pacmanPosition->X = Graphics::GetViewportWidth() - _pacmanSourceRect->Width;
			}
	//up
			if (_pacmanPosition->Y < 0 * Graphics::GetViewportHeight()) {
				_pacmanPosition->Y = Graphics::GetViewportHeight() - _pacmanSourceRect->Height;
				}
	//down
			if (_pacmanPosition->Y + _pacmanSourceRect->Height > Graphics::GetViewportHeight()) {
				_pacmanPosition->Y = 0 * Graphics::GetViewportHeight();
			}
			_frameCount++;
	
		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman





	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		

		if (_frameCount >= 60)
			_frameCount = 0;
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