#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

int int_input, rows, cols, width;

void print_board(char *board, string player, bool test) {
  cout << player << (player.back() == 'S' || player.back() == 's' ? "' board."
                                                                  : "'s board.")
       << endl;
  int healthy = 0, hit = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (board[i * cols + j] == 'A' || board[i * cols + j] == 'B' ||
          board[i * cols + j] == 'C' || board[i * cols + j] == 'D' ||
          board[i * cols + j] == 'M' || board[i * cols + j] == 'S') {
        healthy++;
      }
      if (board[i * cols + j] == '*') {
        hit++;
      }
    }
  }
  cout << "Healthy units: " << healthy << ". Hit units: " << hit << "." << endl
       << "\t";
  for (int i = 0; i < cols; i++) {
    cout << setw(width) << i;
  }
  cout << endl;
  for (int i = 0; i < rows; i++) {
    cout << i << "\t";
    for (int j = 0; j < cols; j++) {
      if (test) {
        cout << setw(width) << board[i * cols + j];
      } else if (board[i * cols + j] == 'O' || board[i * cols + j] == 'X' ||
                 board[i * cols + j] == '*') {
        cout << setw(width) << board[i * cols + j];
      } else {
        cout << setw(width) << 'O';
      }
    }
    cout << endl;
  }
}

bool control_board_start(bool *board_check) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < rows; j++) {
      if (board_check[i * rows + j]) {
        return true;
      }
    }
  }
  return false;
}

int control_board_adding(bool *board_check, int n) {
  int count, ver = 0, hor = 0;
  for (int i = 0; i < rows; i++) {
    count = 0;
    for (int j = 0; j < cols; j++) {
      if (!board_check[i * cols + j]) {
        count++;
      }
      if (count == n) {
        hor++;
      }
    }
  }
  for (int i = 0; i < cols; i++) {
    count = 0;
    for (int j = 0; j < rows; j++) {
      if (!board_check[i * rows + j]) {
        count++;
      }
      if (count == n) {
        ver++;
      }
    }
  }
  return max(hor, ver);
}

bool add_ship(char *board, bool *board_check, int beg_row, int end_row,
              int beg_col, int end_col, int n, char c) {
  int counter = 0;
  if (beg_row == end_row) {
    for (int i = beg_col; i <= end_col; i++) {
      if (board[beg_row * cols + i] == 'O') {
        counter++;
      }
    }
    if (counter == n) {
      for (int i = beg_col; i <= end_col; i++) {
        board_check[beg_row * cols + i] = true;
        board[beg_row * cols + i] = c;
      }
      return false;
    }
    return true;
  } else if (beg_col == end_col) {
    for (int i = beg_row; i <= end_row; i++) {
      if (board[i * cols + beg_col] == 'O') {
        counter++;
      }
    }
    if (counter == n) {
      for (int i = beg_row; i <= end_row; i++) {
        board_check[i * cols + beg_col] = true;
        board[i * cols + beg_col] = c;
      }
      return false;
    }
    return true;
  }
  return true;
}

void add_ship_random(char *board, bool *board_check, int i, int n,
                     string ship) {
  cout << i << ". " << ship << " is placing randomly." << endl;
  int beg_row = 0, beg_col = 0, end_row = 0, end_col = 0;
  do {
    if (rand() % 2 == 0) {
      beg_col = rand() % cols;
      end_col = beg_col;
      beg_row = rand() % (rows - n + 1);
      end_row = beg_row + n - 1;
    } else {
      beg_row = rand() % rows;
      end_row = beg_row;
      beg_col = rand() % (cols - n + 1);
      end_col = beg_col + n - 1;
    }
  } while (add_ship(board, board_check, beg_row, end_row, beg_col, end_col, n,
                    ship[0]));
}

void add_ship_user(string player, int i, int ships, char *board,
                   bool *board_check, int n, string ship) {
  print_board(board, player, true);
  cout << "Place " << i << ". " << ship << ". Do you want random placement?"
       << endl;
  cout << "Type 0 for Yes and type any integer for No." << endl;
  cin >> int_input;
  if (int_input == 0) {
    add_ship_random(board, board_check, i, n, ship);
  } else {
    int beg_row = 0, beg_col = 0, end_row = 0, end_col = 0;
    while (true) {
      if (n == 1) {
        cout << "Enter the row number." << endl;
        cin >> beg_row;
        end_row = beg_row;
        cout << "Enter the column number." << endl;
        cin >> beg_col;
        end_col = beg_col;
      } else if (n > 1) {
        cout << "Type 0 for horizontal and type any integer for vertical "
                "placement."
             << endl;
        cin >> int_input;
        if (int_input != 0) {
          cout << "Enter the column number." << endl;
          cin >> beg_col;
          end_col = beg_col;
          cout << "Enter the row number of the beginning." << endl;
          cin >> beg_row;
          end_row = beg_row + n - 1;
        } else {
          cout << "Enter the row number." << endl;
          cin >> beg_row;
          end_row = beg_row;
          cout << "Enter the column number of the beginning." << endl;
          cin >> beg_col;
          end_col = beg_col + n - 1;
        }
      }
      if (!add_ship(board, board_check, beg_row, end_row, beg_col, end_col, n,
                    ship[0])) {
        break;
      }
      cout << "This area is occupied by another ship, try again." << endl;
    }
  }
  cout << i << ". " << ship << " is placed." << endl;
  if (i == ships) {
    print_board(board, player, true);
  }
}

void add_ship_main(bool first, bool opponent, string player1, string player2,
                   char *player1_board, bool *player1_board_check,
                   char *player2_board, bool *player2_board_check, int n,
                   string ship) {
  if (rows >= n || cols >= n) {
    int maxShips = min(control_board_adding(player1_board_check, n),
                       control_board_adding(player2_board_check, n));
    if (maxShips > 0) {
      int ships;
      cout << "How many " << ship
           << "s do you want to have? (Enter 0 if you do not want)" << endl;
      cin >> ships;
      if (ships > 0) {
        srand((unsigned)time(NULL));
        for (int i = 1; i <= ships; i++) {
          if ((control_board_adding(player1_board_check, n) < 1) ||
              (control_board_adding(player2_board_check, n) < 1)) {
            cout << "There is no space left in this board for " << ship << "."
                 << endl;
            break;
          }
          if (first) {
            add_ship_user(player1, i, ships, player1_board, player1_board_check,
                          n, ship);
          }
          if (opponent) {
            add_ship_random(player2_board, player2_board_check, i, n, ship);
            cout << i << ". " << ship << " is placed." << endl;
          } else {
            add_ship_user(player2, i, ships, player2_board, player2_board_check,
                          n, ship);
          }
          if (!first) {
            add_ship_user(player1, i, ships, player1_board, player1_board_check,
                          n, ship);
          }
        }
      }
    } else {
      cout << "There is no space left in this board for " << ship << "."
           << endl;
    }
  } else {
    cout << ship << " is too big for this board." << endl;
  }
}

bool attack_ship(string player1, string player2, int row, int col, char *board,
                 bool *board_check) {
  if (board_check[row * cols + col]) {
    board_check[row * cols + col] = false;
    board[row * cols + col] = '*';
    cout << player1 << " hit " << player2 << "." << endl;
    return true;
  } else {
    board[row * cols + col] = 'X';
    cout << player1 << " could not hit " << player2 << "." << endl;
    return false;
  }
}

bool attack_ship_random(string player1, string player2, char *board,
                        bool *board_check) {
  if (player1.compare("Computer") == 0) {
    cout << player1 << " attacks." << endl;
  }
  int row = 0, col = 0;
  do {
    row = rand() % rows;
    col = rand() % cols;
  } while (board[row * cols + col] == '*' || board[row * cols + col] == 'X');
  return attack_ship(player1, player2, row, col, board, board_check);
}

bool attack_ship_user(string player1, string player2, char *other_board,
                      char *board, bool *board_check) {
  cout << player1 << " attacks." << endl;
  print_board(other_board, player1, true);
  print_board(board, player2, false);
  cout << "Do you want random hit?" << endl;
  cout << "Type 0 for Yes and type any integer for No." << endl;
  cin >> int_input;
  if (int_input == 0) {
    return attack_ship_random(player1, player2, board, board_check);
  } else {
    int row = 0, col = 0;
    while (true) {
      cout << "Enter the row number." << endl;
      cin >> row;
      cout << "Enter the column number." << endl;
      cin >> col;
      if (board[row * cols + col] != '*' && board[row * cols + col] != 'X') {
        break;
      }
      cout << "Area has already hit before, try again." << endl;
    }
    return attack_ship(player1, player2, row, col, board, board_check);
  }
}

bool control_finish(bool *board_check) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (board_check[i * cols + j]) {
        return true;
      }
    }
  }
  return false;
}

int main() {
  bool first_play_choice, continue_attack_choice, opponent;
  string str_input, player1, player2;
  ifstream cfg("BattleshipConfiguration.ini");
  cfg >> int_input;
  first_play_choice = int_input != 0;
  getline(cfg, str_input);
  cfg >> int_input;
  continue_attack_choice = int_input != 0;
  getline(cfg, str_input);
  cfg >> int_input;
  opponent = int_input == 0;
  getline(cfg, str_input);
  getline(cfg, player1);
  getline(cfg, player2);
  if (opponent) {
    player2 = "Computer";
  }
  cfg >> rows;
  getline(cfg, str_input);
  cfg >> cols;
  getline(cfg, str_input);
  cfg.close();
  int mw = max(rows - 1, cols - 1);
  while (mw > 0) {
    mw /= 10;
    width++;
  }
  width += 2;
  char player1_board[rows * cols], player2_board[rows * cols];
  bool player1_board_check[rows * cols], player2_board_check[rows * cols];
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      player1_board[i * cols + j] = 'O';
      player1_board_check[i * cols + j] = 0;
      player2_board[i * cols + j] = 'O';
      player2_board_check[i * cols + j] = 0;
    }
  }
  do {
    add_ship_main(first_play_choice, opponent, player1, player2, player1_board,
                  player1_board_check, player2_board, player2_board_check, 5,
                  "Aircraft carrier");
    add_ship_main(first_play_choice, opponent, player1, player2, player1_board,
                  player1_board_check, player2_board, player2_board_check, 4,
                  "Cruiser");
    add_ship_main(first_play_choice, opponent, player1, player2, player1_board,
                  player1_board_check, player2_board, player2_board_check, 3,
                  "Destroyer");
    add_ship_main(first_play_choice, opponent, player1, player2, player1_board,
                  player1_board_check, player2_board, player2_board_check, 2,
                  "Frigate");
    add_ship_main(first_play_choice, opponent, player1, player2, player1_board,
                  player1_board_check, player2_board, player2_board_check, 1,
                  "Boat");
  } while (!control_board_start(player1_board_check));
  while (control_finish(player1_board_check) &&
         control_finish(player2_board_check)) {
    srand((unsigned)time(NULL));
    if (first_play_choice) {
      while (control_finish(player1_board_check) &&
             control_finish(player2_board_check)) {
        if (!attack_ship_user(player1, player2, player1_board, player2_board,
                              player2_board_check) ||
            !continue_attack_choice) {
          break;
        }
      }
    }
    while (control_finish(player1_board_check) &&
           control_finish(player2_board_check)) {
      bool round;
      if (opponent) {
        round = attack_ship_random(player2, player1, player1_board,
                                   player1_board_check);
      } else {
        round = attack_ship_user(player2, player1, player2_board, player1_board,
                                 player1_board_check);
      }
      if (!round || !continue_attack_choice) {
        break;
      }
    }
    if (!first_play_choice) {
      while (control_finish(player1_board_check) &&
             control_finish(player2_board_check)) {
        if (!attack_ship_user(player1, player2, player1_board, player2_board,
                              player2_board_check) ||
            !continue_attack_choice) {
          break;
        }
      }
    }
  }
  cout << "Game over." << endl;
  if (control_finish(player1_board_check)) {
    cout << player1 << " won." << endl;
  } else if (control_finish(player2_board_check)) {
    cout << player2 << " won." << endl;
  } else {
    cout << "It is a tie." << endl;
  }
  print_board(player1_board, player1, true);
  print_board(player2_board, player2, true);
  cout << "Enter anything to exit." << endl;
  cin >> str_input;
  return 0;
}
