#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
using namespace std;
int width = 0;		  //Kích thước cột
int height = 0;		  //Kích thước hàng
int numberOfMine = 0; //Số lượng mìn
void initialization();
/*
	Hàm initialization - đọc dữ liệu từ file Config.txt để lấy thông tin cấu hình game: width, height, numberOfMine.
*/
bool isWin(vector<vector<char>> board, vector<vector<char>> b);
/*
 Hàm isWin - kiểm tra xem bạn đã mở hết các ô và cắm cờ đúng những ô có mìn hay không.
 Nếu đã mở hết các ô không có mìn, và cắm cờ hết các ô có mìn. bạn thắng
*/
vector<vector<char>> generateBoard();
/*
Hàm generateBoard - Dựa vào thông số cấu hình để tạo nên một ma trận mà giá trị cuối cùng là lưu trữ thông tin về số mìn xung quanh của mỗi ô.
*/
vector<vector<char>> generateDisplayBoard();
/*
Hàm generateDisplayBoard - Khởi tạo ma trận chứa các ký tự '*'. Thể hiện giao diện game.
*/
void spread(vector<vector<char>> &board, vector<vector<char>> &displayBoard, int x, int y);
/*
Hàm spread - Hàm chính của chương trình. Sử dụng đệ quy để mở các ô không có mìn xung quanh ô tại vị trí x, y.
*/
void draw(vector<vector<char>> displayboard);
/*
Hàm hiển thị giao diện game. Nhận dữ liệu từ displayboard.
*/
void printEnding(vector<vector<char>> displayboard); //In ra mình hình vị trí mìn khi kết thúc game
/*
Hàm run để chuẩn hóa dữ liệu vào và tiên hành chơi.
*/
void run(vector<vector<char>> &board, vector<vector<char>> &displayboard, int x, int y, char input);
int main()
{
	initialization();
	vector<vector<char>> board = generateBoard();				//Mảng chứa dữ liệu ban đầu. Số lượng min của mỗi ô.
	vector<vector<char>> displayBoard = generateDisplayBoard(); //Mảng chứa dữ liệu để hiển thị thông tin cho người chơi.
	char input;
	int x, y;
	run(board, displayBoard, x, y, input);
	board.clear(); // Xoa vector
	displayBoard.clear();
	return 0;
}
void run(vector<vector<char>> &board, vector<vector<char>> &displayBoard, int x, int y, char input)
{
	char s1 = 'f';
	char s2 = 'o';
	while (1)
	{
		draw(displayBoard);
		input = 'n';
		x = -1;
		y = -1;
		while (!(input == s1) && !(input == s2))
		{
			cout << endl
				 << endl
				 << "1.Option:\n\t'o'-Open\n\t'f' -Flag/Un Flag\nYour choice: ";
			cin >> input;
		}
		while (!(x >= 0 && x < height))
		{

			cout << "\n2.Enter x: ";
			cin >> x;
		}
		while (y < 0 || y >= width)
		{

			cout << "\n3.Enter y: ";
			cin >> y;
		}

		//Tiến hành chơi với các lựa chọn
		if (input == 'f')
		{
			if (displayBoard[x][y] == '*')
			{
				displayBoard[x][y] = 'F';
			}
			else if (displayBoard[x][y] == 'F')
			{
				displayBoard[x][y] = '*';
			}
			else
				cout << "Can't flag" << endl
					 << endl;
		}
		else
		{
			if (board[x][y] == 'M')
			{
				printEnding(board);
				cout << endl
					 << "You hit a mine." << endl;
				break;
			}

			else if (board[x][y] != '0')
			{
				displayBoard[x][y] = board[x][y];
			}

			else
			{
				spread(board, displayBoard, x, y);
			}
		}
		if (isWin(board, displayBoard))
		{
			printEnding(board);
			cout << endl
				 << "You win!" << endl;
			break;
		}
	}
}
void initialization()
{
	fstream f;
	vector<int> para;
	f.open("Config.txt", ios::in);
	string line;
	while (!f.eof())
	{
		getline(f, line);
		int data = atoi(line.c_str());
		para.push_back(data);
	}
	f.close();
	width = para[0];
	height = para[1];
	numberOfMine = para[2];
	cout << width;
	para.clear();
}
vector<vector<char>> generateBoard()
{
	vector<vector<char>> board;
	int x, y;
	int mines_placed = 0;
	int mines_adjacent; //Đếm số lượng mìn xung quanh của mỗi ô. =0 Nếu xung quanh không có boom
	//Tạo dữ liệu ban đầu.
	vector<char> row;
	for (int i = 0; i < width; i++)
	{
		row.push_back('0');
	}
	for (int j = 0; j < height; j++)
	{
		board.push_back(row);
	}
	//Sinh vị trí mìn ngẫu nhiên. Với số lượng mìn cho trước.
	srand(time(0));
	while (mines_placed < numberOfMine)
	{
		x = rand() % height;
		y = rand() % width;

		if (board[x][y] == '0')
		{
			board[x][y] = 'M';
			mines_placed++;
		}
	}

	/*
	Tính toán số lượng mìn xung quanh của mỗi ô. Sau đó thay dữ liệu vector board ban đầu bằng số lượng của mìn xung quanh của mỗi ô.
	*/
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (board[i][j] == 'M')
				continue; //Nếu là mìn thì bỏ qua.

			mines_adjacent = 0;

			for (int u = i - 1; u <= i + 1; ++u)
			{
				for (int v = j - 1; v <= j + 1; ++v)
				{
					if (u < 0 || u >= height || v < 0 || v >= width)
						continue;
					if (board[u][v] == 'M')
					{
						mines_adjacent++;
					}
				}
			}

			board[i][j] = (char)mines_adjacent + 48; // kiêu int thành ký tự char. ví dụ 1 -> '1'
		}
	}
	return board;
}
vector<vector<char>> generateDisplayBoard()
{
	vector<vector<char>> displayBoard;
	for (int i = 0; i < height; i++)
	{
		vector<char> row;
		for (int j = 0; j < width; j++)
		{
			row.push_back('*');
		}
		displayBoard.push_back(row);
	}
	return displayBoard;
}
void draw(vector<vector<char>> displayboard)
{
	system("cls"); //làm mới màn hình
	cout << "WELLCOME TO MY MINESWEEPER" << endl;
	printf("x\\y%c", '|');
	for (int i = 0; i < height; i++)
	{
		printf("%2d%2c", i, '|');
	}
	cout << endl;
	for (int i = 0; i <= height * 4 + 3; i++)
	{
		cout << "-";
	}
	for (int i = 0; i < height; i++)
	{
		cout << endl;
		printf("%2d%2c", i, '|');
		for (int j = 0; j < width; j++)
			if (displayboard[i][j] == '0')
			{
				printf("%2c%2c", ' ', '|');
			}
			else
			{
				printf("%2c%2c", displayboard[i][j], '|');
			}
		cout << endl;
		for (int i = 0; i <= height * 4 + 3; i++)
		{
			cout << "-";
		}
	}
}
void printEnding(vector<vector<char>> displayboard)
{
	system("cls"); //làm mới màn hình
	cout << "WELLCOME TO MY MINESWEEPER" << endl;
	printf("x\\y%c", '|');
	for (int i = 0; i < height; i++)
	{
		printf("%2d%2c", i, '|');
	}
	cout << endl;
	for (int i = 0; i <= height * 4 + 3; i++)
	{
		cout << "-";
	}
	for (int i = 0; i < height; i++)
	{
		cout << endl;
		printf("%2d%2c", i, '|');
		for (int j = 0; j < width; j++)
			if (displayboard[i][j] != 'M')
			{
				printf("%2c%2c", ' ', '|');
			}
			else
			{
				printf("%2c%2c", displayboard[i][j], '|');
			}
		cout << endl;
		for (int i = 0; i <= height * 4 + 3; i++)
		{
			cout << "-";
		}
	}
}
/*
    hàm spread: Mở các ô liền kề.
    Sử dụng đệ quy và thuật toán tìm kiếm theo chiều sâu.
    Mô tả: Kiểm tra vị trí x, y đã được mở chưa? có mìn không?
    Nếu chưa mở và không có mìn. Ô đấy sẽ được thể hiện ra.
       Trong trường hợp ô đó là ô '0' - KHông có mìn thì tiến hành kiểm tra các ô xung quanh nó
*/
void spread(vector<vector<char>> &board, vector<vector<char>> &displayBoard, int x, int y)
{
	if (displayBoard[x][y] == '*' && board[x][y] != 'M') //Trường hợp ô đang xét là ô chưa mở. Nếu đã mở thì bỏ qua.
	{
		displayBoard[x][y] = board[x][y]; // đặt giá trị của displayboard bằng với board tại vị trí x, y. kiểm tra xem nó có phải là ô trống hay không?
		if (board[x][y] == '0')
		{
			/* để loang các ô liền kề
		*/
			for (int u = x - 1; u <= x + 1; ++u)
			{
				for (int v = y - 1; v <= y + 1; ++v)
				{
					if (u < 0 || v < 0 || u >= height || v >= width) //Loại trừ các vị trí x, y không phù hợp
						continue;
					spread(board, displayBoard, u, v);
				}
			}
		}
	}
}

bool isWin(vector<vector<char>> board, vector<vector<char>> display)
{
	int c1 = 0;//Điều kiện 1
	int c2 = 0;//Điều kiện 2
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (board[i][j] == 'M' && display[i][j] == 'F')
			{
				c1++;
			}
			else if (board[i][j] != 'M' && display[i][j] == board[i][j])
			{
				c2++;
			}
		}
	}
	if (c1 + c2 == height * width)
	{
		return true;
	}
	return false;
}
