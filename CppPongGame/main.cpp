#include <iostream>
#include <time.h>
#include <conio.h>
#include <thread>
#include <windows.h>

using namespace std;

enum BallDirections { 
	STOPPED = 0, 
	LEFT = 1, 
	UPLEFT = 2, 
	DOWNLEFT = 3, 
	RIGHT = 4, 
	UPRIGHT = 5, 
	DOWNRIGHT = 6
};

class PongGame
{
private:
	int ballX, ballY;
	int originalX, originalY;
	BallDirections ballDirection;

public:
	PongGame(int posX, int posY)
		{
			originalX = posX;
			originalY = posY;
			ballX = posX;
			ballY = posY;
			ballDirection = STOPPED;
		}

	void Reset()
	{
		ballX = originalX;
		ballY = originalY;
		ballDirection = STOPPED;
	}

	void ChangeDirection(BallDirections newDirection)
	{
		ballDirection = newDirection;
	}

	void RandomDirection()
	{
		ballDirection = (BallDirections)((rand() % 6) + 1); // 1 to 6
	}

	void Move()
	{
		switch (ballDirection)
		{
		case STOPPED:
			break;
		case LEFT:
			ballX--;
			break;
		case RIGHT:
			ballX++;
			break;
		case UPLEFT:
			ballX--; 
			ballY--;
			break;
		case DOWNLEFT:
			ballX--;
			ballY++;
			break;
		case UPRIGHT:
			ballX++;
			ballY--;
			break;
		case DOWNRIGHT:
			ballX++;
			ballY++;
			break;
		default:
			break;
		}
	}

	friend ostream & operator << (ostream & o, PongGame c)
	{
		o << "Ball [" << c.ballX << "," << c.ballY << "][" << c.ballDirection << "]";
		return o;
	}

inline int getBallX() { return ballX; }
inline int getBallY() { return ballY; }
inline BallDirections getDirection() { return ballDirection; }
};

class PlayerPaddle
{
private:
	int paddleX, paddleY;
	int originalX, originalY;

public:
	PlayerPaddle()
	{
		paddleX = 0;
		paddleY = 0;
	}

	PlayerPaddle(int posX, int posY) : PlayerPaddle()
	{
		originalX = posX;
		originalY = posY;
		paddleX = posX;
		paddleY = posY;
	}

	inline void Reset()
	{
		paddleX = originalX;
		paddleY = originalY;
	}

	friend ostream& operator << (ostream& o, PlayerPaddle c)
	{
		o << "Paddle [" << c.paddleX << "," << c.paddleY << "]";
		return o;
	}

	inline int getPaddleX() { return paddleX; }
	inline int getPaddleY() { return paddleY; }
	inline void movePaddleUp() { paddleY--; }
	inline void movePaddleDown() { paddleY++; }
};

class GameManager
{
private:
	int screenWidth, screenHeight;
	int player1Score, player2Score;
	char player1UpKey, player1DownKey, player2UpKey, player2DownKey;
	bool quitGame;
	PongGame* pongGamePtr;
	PlayerPaddle* player1Ptr;
	PlayerPaddle* player2Ptr;
public:
	GameManager(int width, int height)
	{
		HideCursor();
		srand(time(NULL));
		quitGame = false;
		player1UpKey = 'w';
		player2UpKey = 'i';
		player1DownKey = 's';
		player2DownKey = 'k';
		player1Score = 0;
		player2Score = 0;
		screenWidth = width;
		screenHeight = height;
		pongGamePtr = new PongGame(width / 2, height / 2);
		player1Ptr = new PlayerPaddle(1, height / 2 - 3);
		player2Ptr = new PlayerPaddle(width - 2, height / 2 - 3);
	}

	~GameManager()
	{
		delete pongGamePtr, player1Ptr, player2Ptr;
	}

	void ScoreUp(PlayerPaddle* player)
	{
		if (player == player1Ptr)
			player1Score++;
		else if (player == player2Ptr)
			player2Score++;

		pongGamePtr->Reset();
		player1Ptr->Reset();
		player2Ptr->Reset();
	}

	void Draw()
	{
		string wall = "\xDB";
		for (int i = 0; i < screenWidth + 2; i++)
		{
			cout << wall; // top wall
		}
		cout << endl;

		for (int i = 0; i < screenHeight; i++)
		{
			for (int j = 0; j < screenWidth; j++)
			{
				int ballx = pongGamePtr->getBallX();
				int bally = pongGamePtr->getBallY();
				int player1x = player1Ptr->getPaddleX();
				int player2x = player2Ptr->getPaddleX();
				int player1y = player1Ptr->getPaddleY();
				int player2y = player2Ptr->getPaddleY();

				if (j == 0)
					cout << wall; // left wall

				if (ballx == j && bally == i)
					cout << "O"; // ball
				else if (player1x == j && player1y == i)
					cout << "\xDB"; // player1
				else if (player1x == j && player1y + 1 == i)
					cout << "\xDB"; // player1
				else if (player1x == j && player1y + 2 == i)
					cout << "\xDB"; // player1
				else if (player1x == j && player1y + 3 == i)
					cout << "\xDB"; // player1

				else if (player2x == j && player2y == i)
					cout << "\xDB"; // player2
				else if (player2x == j && player2y + 1 == i)
					cout << "\xDB"; // player2
				else if (player2x == j && player2y + 2 == i)
					cout << "\xDB"; // player2
				else if (player2x == j && player2y + 3 == i)
					cout << "\xDB"; // player2

				else 
					cout << " ";

				if (j == screenWidth - 1)
					cout << wall; // right wall
			}
			cout << endl;
		}

		for (int i = 0; i < screenWidth + 2; i++)
		{
			cout << wall; // bottom wall
		}
		cout << endl;

		cout << "Player 1: " << player1Score << endl;
		cout << "Player 2: " << player2Score << endl;
		cout << "\nPress 'Q' to quit" << endl;
	}

	void clearScreen() // this helps prevent screen flickering
	{
		HANDLE hOut;
		COORD Position;

		hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		Position.X = 0;
		Position.Y = 0;

		SetConsoleCursorPosition(hOut, Position);
	}

	void Input()
	{
		pongGamePtr->Move();

		int ballx = pongGamePtr->getBallX();
		int bally = pongGamePtr->getBallY();
		int player1x = player1Ptr->getPaddleX();
		int player2x = player2Ptr->getPaddleX();
		int player1y = player1Ptr->getPaddleY();
		int player2y = player2Ptr->getPaddleY();

		if (_kbhit())
		{
			char current = _getch();
			if (current == player1UpKey)
				if (player1y > 0)
					player1Ptr->movePaddleUp();
			if (current == player2UpKey)
				if (player2y > 0)
					player2Ptr->movePaddleUp();
			if (current == player1DownKey)
				if (player1y + 4 < screenHeight)
					player1Ptr->movePaddleDown();
			if (current == player2DownKey)
				if (player2y + 4 < screenHeight)
					player2Ptr->movePaddleDown();

			if (pongGamePtr->getDirection() == STOPPED)
			{
				pongGamePtr->RandomDirection();
			}

			if (current == 'q')
				quitGame = true;
		}
	}

	void Logic()
	{
		int ballx = pongGamePtr->getBallX();
		int bally = pongGamePtr->getBallY();
		int player1x = player1Ptr->getPaddleX();
		int player2x = player2Ptr->getPaddleX();
		int player1y = player1Ptr->getPaddleY();
		int player2y = player2Ptr->getPaddleY();

		// left paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player1x + 1)
				if (bally == player1y + i)
					pongGamePtr->ChangeDirection((BallDirections)((rand() % 3) + 4));
		// right paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player2x - 1)
				if (bally == player2y + i)
					pongGamePtr->ChangeDirection((BallDirections)((rand() % 3) + 1));
	
		// bottom wall
		if (bally == screenHeight - 1)
			pongGamePtr->ChangeDirection(pongGamePtr->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);

		// top wall
		if (bally == 0)
			pongGamePtr->ChangeDirection(pongGamePtr->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);

		// right wall
		if (ballx == screenWidth - 1)
			ScoreUp(player1Ptr);

		// left wall
		if (ballx == 0)
			ScoreUp(player2Ptr);
	}

	void HideCursor()
	{
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(consoleHandle, &info);
	}

	void Run()
	{
		while (!quitGame)
		{
			clearScreen();
			Draw();
			Input();
			Logic();
			std::this_thread::sleep_for(std::chrono::milliseconds(25)); // 25ms delay
		}
	}
};

int main() 
{
	GameManager gameManager(50, 20);
	gameManager.Run();
	return 0;
}