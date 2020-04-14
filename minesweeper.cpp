#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <time.h>
#include <fstream>
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
void executing(vector<vector<char>> &board, vector<vector<char>> &displayBoard, int x, int y);
/*
Hàm executing - Hàm chính của chương trình. Sử dụng đệ quy để mở các ô không có mìn xung quanh ô tại vị trí x, y.
*/
void printBoard(vector<vector<char>> &displayboard);
/*
Hàm hiển thị giao diện game. Nhận dữ liệu từ displayboard.
*/
int main()
{
	initialization();
	vector<vector<char>> board = generateBoard();				//Mảng chứa dữ liệu ban đầu. Số lượng min của mỗi ô.
	vector<vector<char>> displayBoard = generateDisplayBoard(); //Mảng chứa dữ liệu để hiển thị thông tin cho người chơi.
	string input;
	int x = -1, y = -1;
	while (1)
	{
		printBoard(displayBoard);
		cout << endl
			 << endl
			 << "1.Option:\n\t'o'-Open\n\t'f' -Flag\nYour choice: ";
		cin >> input;
		cin.clear();
		cout << "2.Enter x: ";
		cin >> x;
		cout << "3.Enter y: ";
		cin >> y;
		//Chuẩn hóa dữ liệu vào
		if (input != "f" && input != "o")
		{
			cout << "Enter again" << endl;
			continue;
		}
		if (x < 0 || x >= height || y < 0 || y >= width)
		{
			cout << "Enter again" << endl
				 << endl;
			continue;
		}

		//Tiến hành chơi với các lựa chọn
		if (input == "f")
		{
			if (displayBoard[x][y] == '*')
			{
				displayBoard[x][y] = 'F';
			}
			else
				cout << "Can't flag" << endl
					 << endl;
		}

		else
		{
			if (board[x][y] == 'M')
			{
			    printBoard(board);
				cout << "You hit a mine. Bummer." << endl;
				break;

			}

			else if (board[x][y] != '0')
			{
				displayBoard[x][y] = board[x][y];
			}

			else
			{
				executing(board, displayBoard, x, y);
			}
		}
		if (isWin(board, displayBoard))
		{   printBoard(board);
			cout <<endl<< "You win!" << endl;
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

			board[i][j] = (char)mines_adjacent + 48; // chuyển kiêu int thành ký tự char.
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

void executing(vector<vector<char>> &board, vector<vector<char>> &displayBoard, int x, int y)
{

	if (displayBoard[x][y] == '*'&&board[x][y]!='M') //Trường hợp ô đang xét là ô chưa mở. Nếu đã mở thì bỏ qua.
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
					if (u < 0 || v < 0 || u >= height || v >= width)
						continue;
					executing(board, displayBoard, u, v);
				}
			}
		}
	}
}

void printBoard(vector<vector<char>> &displayboard)
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
        if(displayboard[i][j]=='0'){
            printf("%2c%2c", ' ', '|');
        }
		else{
			printf("%2c%2c", displayboard[i][j], '|');
		}
		cout << endl;
		for (int i = 0; i <= height * 4 + 3; i++)
		{
			cout << "-";
		}
	}
}

bool isWin(vector<vector<char>> board, vector<vector<char>> display)
{
	int c1 = 0;
	int c2 = 0;
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
