#ifndef CHESS_LOGIC_H

#define CHESS_LOGIC_H

#include <iostream>
#include <vector>
using namespace std;

class Figures{
protected:
	bool IsDead;
	int x;
	int y;
private:
	Figures(Figures const & rhs){}
public:
	Figures(){IsDead = 1; x = 0; y = 0;}
	Figures(int OutsideX, int OutsideY){
		IsDead = false;
		x = OutsideX;
		y = OutsideY;
	}
	~Figures(){IsDead = 1; x = 0; y = 0;}
	void operator = (Figures const & rhs){
		x = rhs.x;
		y = rhs.y;
	}
public:
	virtual bool ValidMove(int x1, int y1, int x2, int y2){return 0;}
	bool SameCoordinates(int OutsideX, int OutsideY){
		if(x == OutsideX && y == OutsideY && !IsDead){return true;}
		return false;
	}
	void SetValue(int OutsideX,int OutsideY){
		x = OutsideX;
		y = OutsideY;
	}
	void Kill(){
		IsDead = true;
		x = -10;
		y = -10;
	}
	virtual bool IsKing(){return false;}
	virtual bool IsBishop(){return false;}
	virtual bool IsQueen(){return false;}
	virtual bool IsRook(){return false;}
	virtual bool IsPawn(){return false;}
	virtual void IsLeft(){return;}
	virtual void IsRight(){return;}
	virtual void IsntLeft(){return;}
	virtual void IsntRight(){return;}
	virtual void IsSomeoneInFront(){return;}
	virtual void IsNooneInFront(){return;}
};



class Pawn : public Figures{
public:
	bool MadeFirstMove;
	int MoveCounter;
	bool White;
	bool Promote;
	bool SomeoneInFront;
	bool Left;
	bool Right;
private:
	Pawn(Pawn const & rhs){}
	void operator = (Pawn const & rhs){}
public:
	Pawn(int OutsideX, int OutsideY, bool IsWhite){
		MadeFirstMove = false;
		MoveCounter = 0;
		White = IsWhite;
		Promote = false;
		IsDead = false;
		x = OutsideX;
		y = OutsideY;
		SomeoneInFront = false;
		Left = false;
		Right = false;
	}
	~Pawn(){MadeFirstMove = 0; IsDead = 1; x = 0; y = 0;}
public:
	virtual void IsLeft(){Left = true;}
	virtual void IsRight(){Right = true;}
	virtual void IsntLeft(){Left = false;}
	virtual void IsntRight(){Right = false;}
	virtual void IsSomeoneInFront(){SomeoneInFront = true;}
	virtual void IsNooneInFront(){SomeoneInFront = false;}
	virtual bool IsPawn(){return true;}
	virtual bool ValidMove(int, int, int, int);
};

bool Pawn::ValidMove(int xFirst, int yFirst, int xSecond, int ySecond){
	if(White){
		if(!MadeFirstMove && !SomeoneInFront){
			MadeFirstMove = true;
			if(ySecond + 1 == yFirst && xFirst == xSecond){MoveCounter++; return true;}
			else if(ySecond + 2 == yFirst && xFirst == xSecond){MoveCounter += 2; return true;}
		}
		else if(!SomeoneInFront){
			if(ySecond + 1 == yFirst && xFirst == xSecond){MoveCounter++; return true;}
		}
		else if(Left && (ySecond + 1 == yFirst && xFirst == xSecond + 1)){
			{MoveCounter++; return true;}
		}
		else if (Right &&(ySecond + 1 == yFirst && xFirst == xSecond - 1)){
			{MoveCounter++; return true;}
		}
		return false;
	}
	else{
		if(!MadeFirstMove && !SomeoneInFront){
			MadeFirstMove = true;
			if(ySecond - 1 == yFirst && xFirst == xSecond){MoveCounter++; return true;}
			else if(ySecond - 2 == yFirst && xFirst == xSecond){MoveCounter += 2; return true;}
		}
		else if(!SomeoneInFront){
			if(ySecond - 1 == yFirst && xFirst == xSecond){MoveCounter++; return true;}
		}
		else if(Left && (ySecond - 1 == yFirst && xFirst == xSecond + 1)){
			{MoveCounter++; return true;}
		}
		else if(Right && (ySecond - 1 == yFirst && xFirst == xSecond - 1)){
			{MoveCounter++; return true;}
		}

		return false;
	}
}



class Bishop : public Figures{
private:
	Bishop(Bishop const & rhs){}
	void operator = (Bishop const & rhs){}
public:
	Bishop(int x, int y) : Figures(x, y){}
	~Bishop(){IsDead = 1; x = 0; y = 0;}
public:
	virtual bool IsBishop(){return true;}
	virtual bool ValidMove(int, int, int, int);
};

bool Bishop::ValidMove(int xFirst, int yFirst, int xSecond, int ySecond){
	return (abs(xFirst - xSecond) == abs(yFirst - ySecond));
}



class Horse : public Figures{
private:
	Horse(Horse const & rhs){}
	void operator = (Horse const  & rhs){}
public:
	Horse(int x, int y) : Figures(x, y){}
	~Horse(){IsDead = 1; x = 0; y = 0;}
public:
	virtual bool ValidMove(int, int, int, int);
};

bool Horse::ValidMove(int xFirst, int yFirst, int xSecond, int ySecond){
	return (xFirst == xSecond - 2 && yFirst == xSecond + 1)||
		   (xFirst == xSecond - 1 && yFirst == xSecond + 2)||
		   (xFirst == xSecond + 1 && yFirst == xSecond + 2)||
		   (xFirst == xSecond + 2 && yFirst == xSecond + 1)||
		   (xFirst == xSecond - 2 && yFirst == xSecond - 1)||
		   (xFirst == xSecond - 1 && yFirst == xSecond - 2)||
		   (xFirst == xSecond + 1 && yFirst == xSecond - 2)||
		   (xFirst == xSecond + 2 && yFirst == xSecond - 1);
}



class Rook : public Figures{
private:
	Rook(Rook const  & rhs){}
	void operator = (Rook const & rhs){}
public:
	Rook(int x, int y) : Figures(x, y){}
	~Rook(){IsDead = 1; x = 0; y = 0;}
public:
	virtual bool IsRook(){return true;}
	virtual bool ValidMove(int, int, int, int);
};

bool Rook::ValidMove(int xFirst, int yFirst, int xSecond, int ySecond){
	return (xSecond > xFirst && yFirst == ySecond)||
		   (xSecond < xFirst && yFirst == ySecond)||
		   (xSecond == xFirst && yFirst > ySecond)||
		   (xSecond == xFirst && yFirst < ySecond);
}



class Queen : public Figures{
private:
	Queen(Queen const & rhs){}
	void operator =  (Queen const & rhs){}
public:
	Queen(int x, int y) : Figures(x, y){}
	~Queen(){IsDead = 1; x = 0; y = 0;}
public:
	virtual bool IsQueen(){return true;}
	virtual bool ValidMove(int, int, int, int);
};

bool Queen::ValidMove(int xFirst, int yFirst, int xSecond, int ySecond){
	Rook A(x,y);
	Bishop B(x,y);
	return A.ValidMove(xFirst,yFirst,xSecond,ySecond)||
		   B.ValidMove(xFirst,yFirst,xSecond,ySecond);
}



class King : public Figures{
private:
	King (King const & rhs){}
	void operator = (King const & rhs){}
public:
	King(int x, int y) : Figures(x, y){}
	~King(){IsDead = 1; x = 0; y = 0;}
public:
	virtual bool IsKing(){return true;}
	virtual bool ValidMove(int, int, int, int);

};

bool King::ValidMove(int xFirst, int yFirst, int xSecond, int ySecond){
	return (abs(xFirst - xSecond) == 1 && abs(yFirst - ySecond) == 1);
}



class GameBoard{
public:
	vector<Figures*> BlackFigures;
	vector<Figures*> WhiteFigures;
	bool WhiteCheck;
	bool BlackCheck;
	bool CheckMate;
	bool StaleMate;
	bool WhiteMove;
	int NumberOfBlack;
	int NumberOfWhite;
private:
	GameBoard(GameBoard const & rhs){}
	void operator = (GameBoard const & rhs){}
public:
	GameBoard();
	~GameBoard();
public:
	bool CanMakeAMove(int, int, int, int);
	bool BlackBlockedPathAndInvalidMove(int, int, int, int,int);
	bool WhiteBlockedPathAndInvalidMove(int, int, int, int,int);
};

GameBoard::GameBoard(){
	WhiteCheck = false;
	BlackCheck = false;
	CheckMate = false;
	StaleMate = false;
	WhiteMove = true;
	NumberOfBlack = 16;
	NumberOfWhite = 16;
	for(int cnt = 0; cnt < 8; cnt++){
		Pawn a(1 +cnt,7,false);
		BlackFigures.push_back(&a);
	}
	Horse A(2,8),B(7,8);
	BlackFigures.push_back(&A);
	BlackFigures.push_back(&B);
	Bishop C(3,8);
	Bishop D(6,8);
	BlackFigures.push_back(&C);
	BlackFigures.push_back(&D);
	Rook E(1,8),F(8,8);
	BlackFigures.push_back(&E);
	BlackFigures.push_back(&F);
	Queen G(4,8);
	King H(5,8);
	BlackFigures.push_back(&G);
	BlackFigures.push_back(&H);

	for(int cnt = 0; cnt < 8; cnt++){
		Pawn a(1 +cnt,2,true);
		WhiteFigures.push_back(&a);
	}
	Horse I(2,1),J(7,1);
	WhiteFigures.push_back(&I);
	WhiteFigures.push_back(&J);
	Bishop K(6,1);
	Bishop L(3,1);
	WhiteFigures.push_back(&K);
	WhiteFigures.push_back(&L);
	Rook M(1,1),N(8,1);
	WhiteFigures.push_back(&M);
	WhiteFigures.push_back(&N);
	Queen O(4,1);
	King P(5,1);
	WhiteFigures.push_back(&O);
	WhiteFigures.push_back(&P);
}

GameBoard::~GameBoard(){
	WhiteCheck = false;
	BlackCheck = false;
	CheckMate = false;
	StaleMate = false;
	WhiteMove = true;
	NumberOfBlack = 0;
	NumberOfWhite = 0;
	BlackFigures.clear();
	WhiteFigures.clear();
}

enum MovementBishop{
	UpperLeftCorner,
	UpperRightCorner,
	DownRightCorner,
	DownLeftCorner
};

int BishopDirX[4] = {-1,+1,+1,-1};
int BishopDirY[4] = {-1,-1,+1,+1};

enum MovementRook{
	Up,
	Right,
	Down,
	Left
};

int RookDirX[4] = {0,+1,0,-1};
int RookDirY[4] = {-1,0,+1,0};

bool GameBoard::WhiteBlockedPathAndInvalidMove(int xFirst, int yFirst, int xSecond, int ySecond, int ClickedPiece){
	if(WhiteFigures[ClickedPiece]->IsPawn()){
		int tempX = xFirst - 1;
		int tempY = yFirst + 1;

		for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
			if(BlackFigures[cnt]->SameCoordinates(tempX,tempY)){
				WhiteFigures[ClickedPiece]->IsLeft();
			}
			else{
				WhiteFigures[ClickedPiece]->IsntLeft();
			}
		}

		tempX = xFirst + 1;
		tempY = yFirst + 1;

		for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
			if(BlackFigures[cnt]->SameCoordinates(tempX,tempY)){
				WhiteFigures[ClickedPiece]->IsRight();
			}
			else{
				WhiteFigures[ClickedPiece]->IsntRight();
			}
		}

		tempX = xFirst;
		tempY = yFirst + 1;

		for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
			if(BlackFigures[cnt]->SameCoordinates(tempX,tempY)){
				WhiteFigures[ClickedPiece]->IsSomeoneInFront();
			}
			else{
				WhiteFigures[ClickedPiece]->IsNooneInFront();
			}
		}
		if(!WhiteFigures[ClickedPiece]->ValidMove(xFirst,yFirst,xSecond,ySecond)){return true;}
		return false;
	}
	if(WhiteFigures[ClickedPiece]->IsBishop()){
		if(!WhiteFigures[ClickedPiece]->ValidMove(xFirst,yFirst,xSecond,ySecond)){return true;}
		BISHOPWHITE:
		int DirectionToGo;
		if(ySecond > yFirst){
			if(xSecond > xFirst){
				DirectionToGo = UpperRightCorner;
			}
			else{
				DirectionToGo = DownRightCorner;
			}
		}
		else{
			if(xSecond > xFirst){
				DirectionToGo = UpperLeftCorner;
			}
			else{
				DirectionToGo = DownLeftCorner;
			}
		}

		int tempX = xFirst + BishopDirX[DirectionToGo];
		int tempY = yFirst + BishopDirY[DirectionToGo];
		while((tempX != xSecond) && (tempY != ySecond)){
			for(int cnt = 0; cnt < NumberOfBlack; ++cnt){
				if(BlackFigures[cnt]->SameCoordinates(tempX,tempY)){
					return true;
				}
			}
			for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
				if(WhiteFigures[cnt]->SameCoordinates(tempX,tempY)){
					return true;
				}
			}
			tempX += BishopDirX[DirectionToGo];
			tempY += BishopDirY[DirectionToGo];
		}
		return false;
	}

	else if(WhiteFigures[ClickedPiece]->IsRook()){
		if(!WhiteFigures[ClickedPiece]->ValidMove(xFirst,yFirst,xSecond,ySecond)){return true;}
		ROOKWHITE:
		int DirectionToGo;
		if(ySecond == yFirst){
			if(xSecond > xFirst){
				DirectionToGo = Right;
			}
			else{
				DirectionToGo = Left;
			}
		}
		if(xSecond == xFirst){
			if(ySecond > yFirst){
				DirectionToGo = Up;
			}
			else{
				DirectionToGo = Down;
			}
		}

		int tempX = xFirst + RookDirX[DirectionToGo];
		int tempY = yFirst + RookDirY[DirectionToGo];
		while((tempX != xSecond) && (tempY != ySecond)){
			for(int cnt = 0; cnt < NumberOfBlack; ++cnt){
				if(BlackFigures[cnt]->SameCoordinates(tempX,tempY)){
					return true;
				}
			}
			for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
				if(WhiteFigures[cnt]->SameCoordinates(tempX,tempY)){
					return true;
				}
			}
			tempX += RookDirX[DirectionToGo];
			tempY += RookDirY[DirectionToGo];
		}
		return false;
	}

	else if(WhiteFigures[ClickedPiece]->IsQueen()){
		if(!WhiteFigures[ClickedPiece]->ValidMove(xFirst,yFirst,xSecond,ySecond)){return true;}
		if(abs(xFirst - xSecond) == abs(yFirst - ySecond)){
			goto BISHOPWHITE;
		}
		else{
			goto ROOKWHITE;
		}
		return false;
	}
	else{
		if(!WhiteFigures[ClickedPiece]->ValidMove(xFirst,yFirst,xSecond,ySecond)){return true;}
		return false;
	}
}

bool GameBoard::BlackBlockedPathAndInvalidMove(int xFirst, int yFirst, int xSecond, int ySecond, int ClickedPiece){
	if(BlackFigures[ClickedPiece]->IsPawn()){
		int tempX = xFirst + 1;
		int tempY = yFirst - 1;

		for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
			if(WhiteFigures[cnt]->SameCoordinates(tempX,tempY)){
				BlackFigures[ClickedPiece]->IsLeft();
			}
			else{
				BlackFigures[ClickedPiece]->IsntLeft();
			}
		}

		tempX = xFirst - 1;
		tempY = yFirst - 1;

		for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
			if(WhiteFigures[cnt]->SameCoordinates(tempX,tempY)){
				BlackFigures[ClickedPiece]->IsRight();
			}
			else{
				BlackFigures[ClickedPiece]->IsntRight();
			}
		}

		tempX = xFirst;
		tempY = yFirst - 1;

		for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
			if(WhiteFigures[cnt]->SameCoordinates(tempX,tempY)){
				BlackFigures[ClickedPiece]->IsSomeoneInFront();
			}
			else{
				BlackFigures[ClickedPiece]->IsNooneInFront();
			}
		}

		if(!BlackFigures[ClickedPiece]->ValidMove(xFirst,yFirst,xSecond,ySecond)){return true;}
		return false;
	}
	if(BlackFigures[ClickedPiece]->IsBishop()){
		if(!BlackFigures[ClickedPiece]->ValidMove(xFirst,yFirst,xSecond,ySecond)){return true;}
		BISHOPBLACK:
		int DirectionToGo;
		if(ySecond > yFirst){
			if(xSecond > xFirst){
				DirectionToGo = UpperRightCorner;
			}
			else{
				DirectionToGo = DownRightCorner;
			}
		}
		else{
			if(xSecond > xFirst){
				DirectionToGo = UpperLeftCorner;
			}
			else{
				DirectionToGo = DownLeftCorner;
			}
		}

		int tempX = xFirst + BishopDirX[DirectionToGo];
		int tempY = yFirst + BishopDirY[DirectionToGo];
		while((tempX != xSecond) && (tempY != ySecond)){
			for(int cnt = 0; cnt < NumberOfBlack; ++cnt){
				if(BlackFigures[cnt]->SameCoordinates(tempX,tempY)){
					return true;
				}
			}
			for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
				if(WhiteFigures[cnt]->SameCoordinates(tempX,tempY)){
					return true;
				}
			}
			tempX += BishopDirX[DirectionToGo];
			tempY += BishopDirY[DirectionToGo];
		}
		return false;
	}

	else if(BlackFigures[ClickedPiece]->IsRook()){
		if(!BlackFigures[ClickedPiece]->ValidMove(xFirst,yFirst,xSecond,ySecond)){return true;}
		ROOKBLACK:
		int DirectionToGo;
		if(ySecond == yFirst){
			if(xSecond > xFirst){
				DirectionToGo = Right;
			}
			else{
				DirectionToGo = Left;
			}
		}
		if(xSecond == xFirst){
			if(ySecond > yFirst){
				DirectionToGo = Up;
			}
			else{
				DirectionToGo = Down;
			}
		}

		int tempX = xFirst + RookDirX[DirectionToGo];
		int tempY = yFirst + RookDirY[DirectionToGo];
		while((tempX != xSecond) && (tempY != ySecond)){
			for(int cnt = 0; cnt < NumberOfBlack; ++cnt){
				if(BlackFigures[cnt]->SameCoordinates(tempX,tempY)){
					return true;
				}
			}
			for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
				if(WhiteFigures[cnt]->SameCoordinates(tempX,tempY)){
					return true;
				}
			}
			tempX += RookDirX[DirectionToGo];
			tempY += RookDirY[DirectionToGo];
		}
		return false;
	}

	else if(BlackFigures[ClickedPiece]->IsQueen()){
		if(!BlackFigures[ClickedPiece]->ValidMove(xFirst,yFirst,xSecond,ySecond)){return true;}
		if(abs(xFirst - xSecond) == abs(yFirst - ySecond)){
			goto BISHOPBLACK;
		}
		else{
			goto ROOKBLACK;
		}
		return false;
	}
	else{
		if(!BlackFigures[ClickedPiece]->ValidMove(xFirst,yFirst,xSecond,ySecond)){return true;}
		return false;
	}
}

bool GameBoard::CanMakeAMove(int xFirst, int yFirst, int xSecond, int ySecond){
	if(CheckMate || StaleMate) {return false;}

	int ClickedPiece = -1;
	int PlaceToMove = -1;
	int IsWhite = 0;
	int IsBlack = 0;
	bool ClickedPieceIsWhite = false;
	bool ClickedPieceIsBlack = false;

	for(int cnt = 0; cnt < NumberOfBlack; ++cnt){
		if(BlackFigures[cnt]->SameCoordinates(xFirst,yFirst)){
			if(WhiteMove){return false;}
			ClickedPieceIsBlack = true;
			IsBlack++;
			ClickedPiece = cnt;
		}
		if(BlackFigures[cnt]->SameCoordinates(xSecond,ySecond)){
			IsBlack++;
			PlaceToMove = cnt;
		}
	}
	for(int cnt = 0; cnt < NumberOfWhite; ++cnt){
		if(WhiteFigures[cnt]->SameCoordinates(xFirst,yFirst)){
			if(!WhiteMove){return false;}
			ClickedPieceIsWhite = true;
			IsWhite++;
			ClickedPiece = cnt;
		}
		if(WhiteFigures[cnt]->SameCoordinates(xSecond,ySecond)){
			IsWhite++;
			PlaceToMove = cnt;
		}
	}

	if(IsWhite == 2 || IsBlack == 2){return false;}

	if(ClickedPiece == -1) {return false;}

	//if king in check - make the easyer choice - just make so noone else but him can move.

	if(ClickedPieceIsWhite && !WhiteFigures[ClickedPiece]->IsKing()){
		if(WhiteBlockedPathAndInvalidMove(xFirst, yFirst, xSecond,  ySecond, ClickedPiece)){return false;}
	}
	else if (ClickedPieceIsBlack && !BlackFigures[ClickedPiece]->IsKing()){
		if(BlackBlockedPathAndInvalidMove(xFirst, yFirst, xSecond,  ySecond, ClickedPiece)){return false;}
	}

	//king stuff -move and check for check

	if(ClickedPieceIsBlack && PlaceToMove > -1){
		WhiteFigures[PlaceToMove]->Kill();
		BlackFigures[ClickedPiece]->SetValue(xSecond,ySecond);
	}
	else if(ClickedPieceIsWhite && PlaceToMove > -1){
		BlackFigures[PlaceToMove]->Kill();
		WhiteFigures[ClickedPiece]->SetValue(xSecond,ySecond);
	}
	else if(ClickedPieceIsBlack){
		BlackFigures[ClickedPiece]->SetValue(xSecond,ySecond);
	}
	else if(ClickedPieceIsWhite){
		WhiteFigures[ClickedPiece]->SetValue(xSecond,ySecond);
	}

	if(WhiteMove){WhiteMove = false;}
	else{WhiteMove = true;}

	return true;
}

#endif // CHESS_LOGIC_H
