#include <iostream>
#include <algorithm>
#include <windows.h>
#include <vector>
#include <map>
using namespace std;

map<pair<vector<int>, bool>, vector<int>> dp;
void draw_board(vector<int> board) {
    cout << "\t";
    for(int i = 5; i >= 0; i--) cout << board[i] << "\t";
    cout << endl << endl << board[6] << "\t\t\t\t\t\t\t" << board[13] << endl << endl << "\t";
    for(int i = 7; i < 13; i++) cout << board[i] << "\t";
}
vector<int> calculation(vector<int> board, bool first, int n, bool (*play)(vector<int>&,int, bool)) {
    vector<int> res = {-1, 0};
    if(n == 0) return res;
    board[6] = 0; board[13] = 0;
    int score = -10000, tmp_score;
    if(none_of(board.begin()+7, board.end()-1, [](int a){return a;})) {
        score = 0;
        for(auto a : board) score+=a;
        if(!first) score*=-1;
        return {-1, score};
    } if(none_of(board.begin(), board.begin()+6, [](int a){return a;})) {
        score = 0;
        for(auto a : board) score+=a;
        if(first) score*=-1;
        return {-1, score};
    }
    if(dp.find({board, first}) != dp.end()) return dp[{board, first}];
    vector<int> tmp = board, temp;
    for(int i = 1; i <= 6; i++) {
        if((first && !tmp[i+6]) || (!first && !tmp[6-i])) continue;
        if(play(tmp, i, first)) {
            temp = calculation(tmp, first, n, play);
            tmp_score = first ? tmp[13] - tmp[6] : tmp[6] - tmp[13];
            if(tmp_score + temp[1] > score) {
                score = tmp_score + temp[1];
                res[0] = i;
            }
        } else {
            temp = calculation(tmp, !first, n-1, play);
            tmp_score = first ? tmp[13] - tmp[6] : tmp[6] - tmp[13];
            if(tmp_score - temp[1] > score) {
                score = tmp_score - temp[1];
                res[0] = i;
            }
        } tmp = board;
    }
    res[1] = score;
    dp[{board, first}] = res;
    return res;
}
bool finish(vector<int> board, bool first) {
    system("cls");
    if(first) for(int i = 0; i < 6; i++) board[13] += board[i];
    else for(int i = 7; i < 14; i++) board[6] += board[i];
    cout << "p1 : " << board[13] << "\tp2: " << board[6] << endl;
    return board[13] > board[6] ? true : false;
}
bool play(vector<int> &board, int p, bool first) {
    p = first ? p+6 : 6-p;
    if(board[p%14] == 1) {
        board[(p++)%14] = 0;
        board[p%14]++;
    } else if(board[p%14] == 0) {return false;}
    else {
        int left = board[p%14];
        board[p%14] = 0;
        while(left--) board[(p++)%14]++;
        int last = (--p)%14;
        if(board[p%14]%2 == 0) {
            if(p%14 >= 7 && p%14 <= 12 && !first) {
                board[6] += board[p%14];
                board[p%14] = 0;
            } 
            if(p%14 >= 0 && p%14 <= 5 && first) {
                board[13] += board[p%14];
                board[p%14] = 0;
            } 
        }
    }
    if(board[p%14] == 1 && board[(12-p)%14] > 0 && p%14 != 6 && p%14 != 13) {
            if(first) board[13] += board[p%14] + board[(12-p)%14];
            else board[6] += board[p%14] + board[(12-p)%14];
            board[p%14] = 0;
            board[(12-p)%14] = 0;
    }
    return (p%14 == 6 && !first) || (p%14 == 13 && first) ? true : false;
}
int main() {
    bool res;
    int p, choice;
    while(true) {
        vector<int> board = {4,4,4,4,4,4,0,4,4,4,4,4,4,0};
        cout << "Select one:\n1) 2-Players\n2) Against Computer\n";
        cin >> choice;
        system("cls");
        if(choice == 1) {
            while(true) {
                do {
                    system("cls");
                    cout << "Player 1:\n\n";
                    draw_board(board);
                    cout << endl;
                    cin >> p;
                } while(play(board, p, true));
                if(none_of(board.begin()+7, board.end()-1, [](int a){return a;})) {res = finish(board, true); break;}
                do {
                    system("cls");
                    cout << "Player 2:\n\n";
                    draw_board(board);
                    cout << endl;
                    cin >> p;
                } while(play(board, p, false));
                if(none_of(board.begin(), board.begin()+6, [](int a){return a;})) {res = finish(board, false); break;}
            }
        } else if(choice == 2) { 
            int n;
            cout << "Enter calculation depth";
            cin >> n;
            while(true) {
                do {
                    if(all_of(board.begin()+7, board.end()-1, [](int a){return a == 0;})) {res = finish(board, true); break;}
                    if(all_of(board.begin(), board.begin()+6, [](int a){return a == 0;})) {system("cls"); res = finish(board, false); break;}
                    system("cls");
                    cout << "Player 1:\n\n";
                    draw_board(board);
                    cout << endl;
                    cin >> p;
                } while(play(board, p, true));
                do {
                    if(all_of(board.begin()+7, board.end()-1, [](int a){return a == 0;})) {res = finish(board, true); break;}
                    if(all_of(board.begin(), board.begin()+6, [](int a){return a == 0;})) {system("cls"); res = finish(board, false); break;}
                    system("cls");
                    cout << "Computers turn..\n\n";
                    draw_board(board);
                    cout << endl;
                    p = calculation(board, false, n, &play)[0];
                    cout << "Computer made his decision: " << p;
                    Sleep(1500);
                } while(play(board, p, false));
                dp.clear();
            }
        }
        if(res) cout << "Player 1 has won!";
        else cout << "Player 2 has won!";
        cout << endl << "Select one:\n1) Play Again\n2)Close";
        cin >> choice;
        if(choice == 2) break;
        system("cls");
    }
    return 0;
}