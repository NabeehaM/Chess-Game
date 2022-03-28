//Chess Project by Nabeeha Mudassir and Nisa Nadeem 20L-1080 and 20L-1141
#include<iostream>
#include<string>
#include<conio.h>
#include<fstream>

#include "GameData.cpp"
using namespace std;

struct Chess{
    GameData GameState;
    string PlayerNames[2];
    bool GameActive = false;
} Game;

string getPlayerinfo(){
    string Name="";
    cout<< "\n\t\t\t\tEnter Player Name:\t";
    do{
        cin.clear();
        fflush(stdin);
        getline(cin, Name);
    }while(Name.length() == 0);
    return Name;
}

int SaveGame(){
    // If we have an active game then save it for future along with player names
    // The function will ask the user to specify a file name and save the entire game into it
    if (!Game.GameActive){
        cout<<"\n\t\t\t\tThe Game is NOT ACTIVE.\n";
        return -1;
    }
    string NewFile;
    do {
        cout<<"\n Enter File Name: ";
        cin.clear();
        fflush(stdin);
        getline(cin,NewFile);
        }while (NewFile.length()==0);

    ofstream F(NewFile);
    if (!F)
    {
        cout<<"\n\n\t\t\tUnable to open this file.\n";
        return -2;
    }
    F<<Game.PlayerNames[0]<<endl<<Game.PlayerNames[1]<<endl;
    F<<Game.GameState.TurningPlayer<<endl;

    for (int i=0;i<8;i++)
    {
        for (int j=0;j<8;j++)
        {
            F<<Game.GameState.Board[i][j].PieceColor<<" "<<Game.GameState.Board[i][j].MoveCount<<" "<<Game.GameState.Board[i][j].Code<<endl;
        }
    }
    F.close();
    cout<<"\n\nSaved Successfully!\n\n";
    return 0;
}

int LoadGame(){
   // The function will ask the user to specify a file name and save the entire game into it
    string NewFile;
    do{
        cout<<"Enter File Name\n";
        cin.clear();
        fflush(stdin);
        getline(cin,NewFile);
        }while(NewFile.length() == 0);

        ifstream F(NewFile);

        if (!F)
        {
            cout<<"Could not open file";
            return 1;
        }

        getline(F,Game.PlayerNames[0]);
        getline(F,Game.PlayerNames[1]);

        short turning;
        if (Game.GameState.TurningPlayer==WHITE)
            {
                turning=1;
            }
        else
           {
             turning=2;
           }

        F>>turning;

    for (int i=0;i<8;i++)
    {
        for (int j=0;j<8;j++)
        {
            short color, movecount, code;

            if (Game.GameState.Board[i][j].PieceColor==WHITE)
            {
                color=1;
                F>>color;
            }
            else
                {
                    color=0;
                    F>>color;
                }
            movecount=Game.GameState.Board[i][j].MoveCount;
            F>>movecount;

            code=Game.GameState.Board[i][j].Code;
            F>>code;
        }
    }
    F.close();

    cout<<"\n\nLoaded Successfully.\n\n";

    return 0;
}

void ShowGame(){

    cout<<"\t\t\t"<< Game.PlayerNames[1]<<"\tvs\t"<<Game.PlayerNames[0]<<endl;

    for(int r = 0; r < 8; r++ ){
        cout<<endl<<r;
        for(int c = 0; c < 8; c++){
            switch(Game.GameState.Board[r][c].Code){
                case NONE:
                    cout<<"\t..";
                break;
                default:
                    cout<<"\t";
                    if(Game.GameState.Board[r][c].PieceColor == BLACK)
                        cout<<"B";
                    else
                        cout<<"W";

                    switch(Game.GameState.Board[r][c].Code){
                    case PAWN:
                        cout<<"P";
                    break;

                    case ROOK:
                        cout<<"R";
                    break;

                    case KNIGHT:
                        cout<<"kN";
                    break;

                    case BISHOP:
                        cout<<"B";
                    break;

                    case QUEEN:
                        cout<<"Q";
                    break;

                    case KING:
                        cout<<"K";
                    break;
                    }
                break;
            }
        }
        cout<<endl<<endl;
    }
    cout<<endl;
    for(int c = 0; c< 8; c++)
        cout<<"\t"<<c;
    cout<<"\n";
}

void initNewGame(){
    cout<<"\t\t\t\tWHITE Player: Enter your Name: "<<endl<<"\t\t\t\t";
    Game.PlayerNames[1] = getPlayerinfo();
    cout<<"\t\t\t\tBLACK Player: Enter your Name: "<<endl<<"\t\t\t\t";
    Game.PlayerNames[0] = getPlayerinfo();
    initGame(Game.GameState);
}

int PlayGame(){
    chessMove Move;
    ShowGame();

    Game.GameActive=true;
    bool c=false;
    int UndoFR,UndoFC,UndoTR,UndoTC;
    bool pass;
    while(!isCheckmate(Game.GameState, Game.GameState.TurningPlayer) ){

        cout<<endl<<endl<<"\t\t\t\t"<<Game.PlayerNames[Game.GameState.TurningPlayer];

        if(Game.GameState.TurningPlayer == WHITE)
            cout<<endl<<"\t\t\t(WHITE) 's TURN";
        else
            cout<<endl<<"\t\t\t (BLACK) 's TURN";


        cout<<"\n\n"<<"Enter your move in this way:\n";
        cout<<" From Row: From Column: To Row: To Column:\n";
        cout<<"Negative Values To move To The MAIN MENU\n";

        cin >> Move.FromRow>>Move.FromColumn;
        cin>>Move.ToRow>>Move.ToColumn;

        if(Move.FromRow < 0 || Move.FromColumn < 0 || Move.ToColumn < 0 || Move.ToRow < 0){
            cout<< "\t\t\t\tMOVING TO MAIN MENU \n";
            return 0;
        }


        bool d,p,castle;
        d=isValidMove(Game.GameState,Move);
        p=PawnCanbePromoted(Game.GameState,Move);
        castle=Castling(Game.GameState,Move);

        bool dontchangeplayer=false;
                if (pass==true)
                {
                    //check here if the coordinates enter satisfy the enpassant coordinates
                    if (Game.GameState.Board[Move.FromRow][Move.FromColumn].Code==PAWN &&
                        Move.ToRow==Move.FromRow-1 && Move.ToColumn==Move.FromColumn+1) //top right
                    {
                        cout<<"ENPASSANT has been enabled\n";
                        pass=false;
                        cout<<"FR:"<<Move.FromRow<<"FC:"<<Move.FromColumn<<"TR"<<Move.ToRow<<"TC"<<Move.ToColumn;
                        CaptureByEnPassant(Game.GameState,Move,Move.FromRow,Move.FromColumn,Move.ToRow,Move.ToColumn);
                    }
                    else if (Game.GameState.Board[Move.FromRow][Move.FromColumn].Code==PAWN &&
                             Move.ToRow==Move.FromRow-1 && Move.ToColumn==Move.FromColumn-1 ) //top left
                    {
                        cout<<"ENPASSANT has been enabled\n";
                        pass=false;
                        CaptureByEnPassant(Game.GameState,Move,Move.FromRow,Move.FromColumn,Move.ToRow,Move.ToColumn);
                    }
                    else if (Game.GameState.Board[Move.FromRow][Move.FromColumn].Code==PAWN &&
                             Move.ToRow==Move.FromRow+1 && Move.ToColumn==Move.FromColumn-1) //bottom left
                    {
                        cout<<"ENPASSANT has been enabled\n";
                        pass=false;
                        CaptureByEnPassant(Game.GameState,Move,Move.FromRow,Move.FromColumn,Move.ToRow,Move.ToColumn);
                    }
                    else if (Game.GameState.Board[Move.FromRow][Move.FromColumn].Code==PAWN &&
                             Move.ToRow==Move.FromRow+1 && Move.ToColumn==Move.FromColumn+1) //bottom right
                    {
                        cout<<"ENPASSANT has been enabled\n";
                        pass=false;
                        CaptureByEnPassant(Game.GameState,Move,Move.FromRow,Move.FromColumn,Move.ToRow,Move.ToColumn);
                    }
                    ShowGame();
                    pass=false;
                }
        if ( d==false || pass==true) //move is not invalid if enpassant enabled
        {
                cout<<endl<<"\t\t\t This is an ILLEGAL MOVE. You will have to enter valid numbers"<<endl;
              if (castle==true)
                {
                    cout<<" \t\t\t HOWEVER under these special conditions: CASTLING IS PERMITTED.";
                    Game.GameState.Board[Move.ToRow][Move.ToColumn].MoveCount++; //for king
                }

        }
        else if (d==true && pass==false)

            {
                if (p==true)
                {
                    cout<<endl<<"You are eligible for Promoting your Pawn."<<endl;

                    cout<<"Now: Press 1 to promote it to BISHOP"<<endl;
                    cout<<"Or:  Press 2 to promote it to KNIGHT"<<endl;
                    cout<<"Or,  Press 3 to promote it to QUEEN"<<endl;
                    cout<<"Last option: press 4 to promote it to ROOK"<<endl;

                    cout<<"Your choice is: ";
                    int choice;
                    cin>>choice;
                    while (choice!=1 && choice!=2 && choice!=3 && choice!=4)
                    {
                        cout<<"You must enter a valid choice: "<<endl;
                        cin>>choice;
                    }
                    PromotethePawn(Game.GameState,Move,choice);
                    cout<<endl<<"Pawn successfully promoted."<<endl;
                }
                if (IsItEnPassant(Game.GameState,Move)==true)
                {
                    cout<<"ENPASSANT MOVE CAN BE PERFORMED";
                    MakeMove(Game.GameState, Move);
                    Game.GameState.Board[Move.ToRow][Move.ToColumn].MoveCount++;
                    pass=true;
                }
                if (pass==false && p!=true)
                {
                    cout<<endl<<"VALID Move"<<endl;
                    MakeMove(Game.GameState, Move);
                    UndoFR=Move.FromRow;
                    UndoFC=Move.FromColumn;
                    UndoTR=Move.ToRow;
                    UndoTC=Move.ToColumn;
                    Game.GameState.Board[Move.ToRow][Move.ToColumn].MoveCount++;
                }
                if (isfacingCheck(Game.GameState,Game.GameState.TurningPlayer)==true)
                        {
                            cout<<"\t\t\tBut this brings your king in C H E C K\n";
                            cout<<"\t\t\tmaking your move invalid\n";
                            cout<<"\t\t\tSo, enter new coordinates again: \n";
                            dontchangeplayer=true;
                            UNDO(Game.GameState,UndoFR,UndoFC,UndoTR,UndoTC);
                            cout<<"this is why it changes;";
                        }
                            if (dontchangeplayer==false)
                            {
                                if (Game.GameState.TurningPlayer==BLACK)
                                {
                                    Game.GameState.TurningPlayer=WHITE;
                                }
                            else
                                {
                                    Game.GameState.TurningPlayer=BLACK;
                                }
                            }

        ShowGame();
    }
    }

    if (isCheckmate(Game.GameState,Game.GameState.TurningPlayer)==true)
    {
        cout<<"\t\t\t GAME OVER!!"<<endl;
        if (Game.GameState.TurningPlayer==WHITE)
        {
           cout<<"\t\t\t"<<"BLACK has WON!";
        }
        else
        cout<<"\t\t\t"<<"WHITE HAS WON!"<<endl;
    }
    return 0;

}


int main(){
    cout<<"\t\t\t\tWELCOME TO CHESS\n\n";
    cout<<"\t\t\t\tThis Game Is Brought To You By:\n";
    cout<<"\t\t\t\tNabeeha Mudassir and Nisa Nadeem.\n";
    cout<<"\t\t\t\tSelect an option from our menu:\n\n";

    int choice = 0;
    do{
        do{
            cout<<"\t\t\t\t1. New Game\n\t\t\t\t2. Restart Game\n\t\t\t\t3. Save Game\n\t\t\t\t4. Load Game\n\t\t\t\t5. Exit\n\n\t\t\t\t\t Enter Your Choice(1-5) ";
            cin>>choice;
        }while(choice< 1 || choice > 5);

        switch(choice){
        case 1:
            initNewGame();
            PlayGame();
        break;
        case 2:
            PlayGame();
            ShowGame();
        break;
        case 3:
            SaveGame();
        break;
        case 4:
            LoadGame();
        break;
        case 5:
            return 0;
        break;
        }
    }while(1);

    return 0;
}
