
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

enum { Enter = 13, Esc = 27, Left = 75, Right = 77, Up = 72, Down = 80 };
int const WhiteCell = 0xF0, BlackCell = 0x07, ActiveCell = 0x1F;

int X, Y, counter, vertical, horizontal;
int current[15];

void gotoXY(short x, short y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void  Header(int X, int Y, char* TextLogo, int ColorEntered = 111)
{
	char MyLine[80] = "";
	char LU = char(201), TU = char(205), RU = char(187);	// Left upper, Top upper, Right upper
	char BS = char(186);									// Both sides
	char LD = char(200), RD = char(188);					// Left down, Right Down

	int LenLogo = strlen(TextLogo) + 2;
	for (int SZ = 0; SZ < LenLogo; SZ++)
	{
		MyLine[SZ] = TU;
	}

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	int StoredColor = csbi.wAttributes;

	SetConsoleTextAttribute(hStdout, ColorEntered);
	gotoXY(X, Y);
	cout << LU << MyLine << RU;
	gotoXY(X, Y + 1);
	cout << BS << " " << TextLogo << " " << BS;
	gotoXY(X, Y + 2);
	cout << LD << MyLine << RD;
	SetConsoleTextAttribute(hStdout, StoredColor);
}
void WriteColored(char* Text, int ColorEntered)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	int StoredColor = csbi.wAttributes;
	SetConsoleTextAttribute(hStdout, ColorEntered);
	cout << Text;
	SetConsoleTextAttribute(hStdout, StoredColor);
}
void ChangeColor(int x, int y, int how, int ColorEntered)
{
	DWORD l;
	COORD point;
	point.X = x; point.Y = y;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	int StoredColor = csbi.wAttributes;
	SetConsoleTextAttribute(hStdout, ColorEntered);
	FillConsoleOutputAttribute(hStdout, ColorEntered, how, point, &l);
	SetConsoleTextAttribute(hStdout, StoredColor);
}

void WriteCentered(char* Text, int Row, int MinCol, int MaxCol = 80, int ColorEntered = 256)
{
	int LenText = strlen(Text);
	if (ColorEntered == 256) {
		gotoXY(MinCol + (MaxCol - MinCol - LenText) / 2, Row);
		cout << Text;
	}
	else {
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hStdout, &csbi);
		int StoredColor = csbi.wAttributes;
		SetConsoleTextAttribute(hStdout, ColorEntered);
		gotoXY(MinCol + (MaxCol - MinCol - LenText) / 2, Row);
		cout << Text;
		SetConsoleTextAttribute(hStdout, StoredColor);
	}
}
void Chessbox(int X, int Y, int color = WhiteCell, bool EmptyCell = true)
{

	int const Size = 3;
	int count = 0;
	for (int j = 0; j < Size; j++) {
		for (int i = 0; i < Size + 2; i++) {
			gotoXY(X + i, Y + j);
			if (EmptyCell != true) {
				cout << char(current[count++]);
			}
			else {
				WriteColored(" ", color);
			}
		}
		if (EmptyCell != true) { ChangeColor(X, Y + j, 5, color); }
	}
}

void MoveDot(int hor, int vert, bool WriteMoves)
{
	gotoXY(3 + (hor - 1) * 5, 2 + (vert - 1) * 3);
	if (WriteMoves == true) {
		cout << 'x';
	}
	else {
		cout << ' ';
	}
	ChangeColor(3 + (hor - 1) * 5, 2 + (vert - 1) * 3, 1, ((hor + vert) % 2 != 0) ? WhiteCell : BlackCell);
}

void RookMoves(bool WriteMoves)
{
	if (vertical < 8) {
		for (int i = vertical + 1; i <= 8; i++) {
			MoveDot(horizontal, i, WriteMoves);
		}
	}
	if (vertical > 1) {
		for (int i = vertical - 1; i >= 1; i--) {
			MoveDot(horizontal, i, WriteMoves);
		}
	}
	if (horizontal < 8) {
		for (int i = horizontal + 1; i <= 8; i++) {
			MoveDot(i, vertical, WriteMoves);
		}
	}
	if (horizontal > 1) {
		for (int i = horizontal - 1; i >= 1; i--) {
			MoveDot(i, vertical, WriteMoves);
		}
	}
}
void BishopMoves(bool WriteMoves)
{
	int h1 = horizontal, v1 = vertical;
	int h2 = horizontal, v2 = vertical;

	if (vertical < 8) {
		for (int i = vertical + 1; i <= 8; i++) {
			if (h1 < 8) {
				MoveDot(++h1, i, WriteMoves);
			}
		}
	}
	if (vertical > 1) {
		for (int i = vertical - 1; i >= 1; i--) {
			if (h2 > 1) {
				MoveDot(--h2, i, WriteMoves);
			}
		}
	}
	if (horizontal < 8) {
		for (int i = horizontal + 1; i <= 8; i++) {
			if (v1 > 1) {
				MoveDot(i, --v1, WriteMoves);
			}
		}
	}
	if (horizontal > 1) {
		for (int i = horizontal - 1; i >= 1; i--) {
			if (v2 < 8) {
				MoveDot(i, ++v2, WriteMoves);
			}
		}
	}
}

void KnightMoves(bool WriteMoves)
{
	if (vertical < 7 && horizontal < 8) {
			MoveDot(horizontal + 1, vertical + 2, WriteMoves);
	}

	if (vertical<7 && horizontal > 1) {
			MoveDot(horizontal - 1, vertical + 2, WriteMoves);
	}

	if (horizontal> 1 && vertical>2) {
			MoveDot(horizontal - 1, vertical - 2, WriteMoves);
	}

	if (vertical>2 && horizontal <8) {
			MoveDot(horizontal + 1, vertical - 2, WriteMoves);
	}
	
	if (horizontal >2 && vertical>1) {
			MoveDot(horizontal - 2, vertical - 1, WriteMoves);
	}
	
	if (horizontal <7 && vertical<=7) {
			MoveDot(horizontal + 2, vertical + 1, WriteMoves);
	}
	
	if (horizontal >2 && vertical <= 7) {
		MoveDot(horizontal - 2, vertical + 1, WriteMoves);
	}

	if (horizontal <7 && vertical > 1) {
			MoveDot(horizontal + 2, vertical - 1, WriteMoves);
	}
}
void QueenMoves(bool WriteMoves)
{
	int h1 = horizontal, v1 = vertical;
	int h2 = horizontal, v2 = vertical;

	if (vertical < 8) {
		for (int i = vertical + 1; i <= 8; i++) {
			MoveDot(horizontal, i, WriteMoves);
			if (h1 < 8) {
				MoveDot(++h1, i, WriteMoves);
			}
		}
	}
	if (vertical > 1) {
		for (int i = vertical - 1; i >= 1; i--) {
			MoveDot(horizontal, i, WriteMoves);
			if (h2 > 1) {
				MoveDot(--h2, i, WriteMoves);
			}
		}
	}
	if (horizontal < 8) {
		for (int i = horizontal + 1; i <= 8; i++) {
			MoveDot(i, vertical, WriteMoves);
			if (v1 > 1) {
				MoveDot(i, --v1, WriteMoves);
			}
		}
	}
	if (horizontal > 1) {
		for (int i = horizontal - 1; i >= 1; i--) {
			MoveDot(i, vertical, WriteMoves);
			if (v2 < 8) {
				MoveDot(i, ++v2, WriteMoves);
			}
		}
	}
}
void Selected(int figure, bool WriteMoves = true)
{
	enum Figs { Rook = 1, Bishop = 2, Knight = 3, Queen = 4 };
	switch (figure) {
	case Rook:
		RookMoves(WriteMoves);
		break;
	case Bishop:
		BishopMoves(WriteMoves);
		break;
	case Knight:
		KnightMoves(WriteMoves);
		break;
	case Queen:
		QueenMoves(WriteMoves);
		break;
	}

}
void MyASCII()
{
	for (int i = 0; i < 255; i++) {
		cout << setw(6) << i << "=" << char(i);
	}

}

void ChessBoard(int offX, int offY)
{
	int const size = 5;
	for (int i = 1; i <= 8; i++) {
		gotoXY(0, i*(size - 2) - 1);
		cout << i;
	}
	int counter = 1;
	int color;
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
			color = (counter % 2 == 0) ? WhiteCell : BlackCell;
			Chessbox(offX + i * size, offY + j * (size - 2), color);
			counter++;
		}
		counter++;
	}
	for (int i = 1; i <= 8; i++) {
		gotoXY(i*size - 2, 0);
		cout << char(96 + i);
	}
	gotoXY(42, 0);
}
int KeyControl()
{
	while (true) {
		int key = _getch();
		return key;
	}
}

void Move(int dx, int dy)
{
	Chessbox(X, Y, (counter % 2 == 0) ? WhiteCell : BlackCell);
	X += dx;
	Y += dy;
	Chessbox(X, Y, ActiveCell, false);
}


void Cursor(bool visible)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(hStdout, &structCursorInfo);
	structCursorInfo.bVisible = visible;
	SetConsoleCursorInfo(hStdout, &structCursorInfo);
}

void OneBar(int x, int y, int len, int color, bool PrintFrames = true)
{
	gotoXY(x, y);
	WriteColored((PrintFrames != true) ? " " : ">", color);
	gotoXY(x + len, y);
	WriteColored((PrintFrames != true) ? " " : "<", color);
	ChangeColor(x + 1, y, len, color);
}
int VerticalMenu(int x, int y, int len, int puncts)
{
	int const Highlight = 240, Normal = 7;

	OneBar(x, y, len, Highlight);
	int minY = y, maxY = y + puncts - 1;
	while (true) {
		int What = KeyControl();
		switch (What) {
		case Down:
			if (y < maxY) {
				OneBar(x, y, len, Normal, false);
				y++;
				OneBar(x, y, len, Highlight);
			}
			else {
				OneBar(x, y, len, Normal, false);
				y = minY;
				OneBar(x, y, len, Highlight);
			}
			break;
		case Up:
			if (y > minY) {
				OneBar(x, y, len, Normal, false);
				y--;
				OneBar(x, y, len, Highlight);
			}
			else {
				OneBar(x, y, len, Normal, false);
				y = maxY;
				OneBar(x, y, len, Highlight);
			}
			break;
		case Esc:
			break;
		case Enter:
			break;
		}
		if (What == Esc) {
			return What;
			break;
		}
		if (What == Enter) {
			return y - minY + 1;
			break;
		}

	}
}
void main()
{
	char FigureName;

	const int Rook[] =
	{ 220,' ',220,' ',220,
	  ' ',219,219,219,' ',
	  223,223,223,223,223 };

	const int Bishop[] =
	{ ' ',220,219,220,' ',
	  ' ',223,219,223,' ',
	  ' ',223,223,223,' ' };

	const int Knight[] =
	{ ' ',220,219,219,220,
	  ' ',' ',220,219,223,
	  223,223,223,223,223 };

	const int Queen[] =
	{ 220,' ',219,' ',220,
	  ' ',219,219,219,' ',
	  223,223,223,223,223 };

	Cursor(false);
	system("color 07"); // black background, white ink

	while (true)
	{
		system("cls");
		SetConsoleTitle(L"My Chess Etudes v.1.0");
		ChessBoard(1, 1);

		Header(42, 0, "      My Chess Etudes v.1.0      ");
		WriteCentered("This version allows only", 4, 41);
		WriteCentered("to view the moves", 5, 41);
		WriteCentered("of some figures", 6, 41);

		gotoXY(58, 8);
		cout << char(249) << " " << char(249) << " " << char(249);

		WriteCentered("Please select your figure or quit:", 10, 41, 80, 2);
		WriteCentered("The Rook", 12, 41);
		WriteCentered("The Bishop", 13, 41);
		WriteCentered("The Knight", 14, 41);
		WriteCentered("The Queen", 15, 41);
		WriteCentered("Exit", 16, 41);

		WriteCentered("(c) 2016 A.Sh for test aims", 18, 41, 80, 2);
		gotoXY(58, 20);
		cout << char(249) << " " << char(249) << " " << char(249);

		Header(42, 22, "    In chess notation:           ", 137);

		int FigureNum = VerticalMenu(52, 12, 15, 5);

		if (FigureNum != Esc && FigureNum != 5)
		{

			switch (FigureNum) {
			case 1:
				FigureName = 'R';
				for (int i = 0; i < 15; i++) { current[i] = Rook[i]; }
				break;
			case 2:
				FigureName = 'B';
				for (int i = 0; i < 15; i++) { current[i] = Bishop[i]; }
				break;
			case 3:
				FigureName = 'N';
				for (int i = 0; i < 15; i++) { current[i] = Knight[i]; }
				break;
			case 4:
				FigureName = 'Q';
				for (int i = 0; i < 15; i++) { current[i] = Queen[i]; }
				break;
			}

			X = 1, Y = 1, counter = 1;
			vertical = 1, horizontal = 1;

			Chessbox(X, Y, ActiveCell, false);
			Selected(FigureNum, true);

			while (true) {
				int Result = KeyControl();

				switch (Result) {
				case Esc:
					break;
				case Enter:
					//Selected(X, Y, FigureNum);
					break;
				case Right:
					if (X <= 31)
					{
						Selected(FigureNum, false);
						++horizontal;
						Move(5, 0);
						counter++;
						Selected(FigureNum, true);
					}
					break;
				case Left:
					if (X > 1)
					{
						Selected(FigureNum, false);
						--horizontal;
						Move(-5, 0);
						counter--;
						Selected(FigureNum, true);
					}
					break;
				case Up:
					if (Y > 1)
					{
						Selected(FigureNum, false);
						--vertical;
						Move(0, -3);
						counter -= 7;
						Selected(FigureNum, true);
					}
					break;
				case Down:
					if (Y < 21)
					{
						Selected(FigureNum, false);
						++vertical;
						Move(0, 3);
						counter += 7;
						Selected(FigureNum, true);
					}
					break;

				}

				if (Result == Esc) {
					break;
				}

				gotoXY(67, 23);
				cout << FigureName << " " << char(96 + horizontal) << vertical << "   ";
				ChangeColor(67, 23, 7, 137);
			}
		}
		if (FigureNum == 5) {
			break;
		}
	}
	Cursor(true);
	system("cls");
}