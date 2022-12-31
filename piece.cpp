#include "headers/libHeaders.h"
#include "headers/piece.h"
#include "headers/gFunctions.h"
#include "headers/maths.h"
#include "headers/music.h"
using namespace std;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int holeWidth;
extern int holeHeight;
extern int striked;
extern int totalPieces;
extern int lastPottedPieceID;
extern bool pottedLastMove;

extern Pair strikerLimit[4];
extern Pair teamPieces[2];
extern int redID;
extern bool redPottedLastMove;

extern vector<int> pottedPieces[2];

extern piece pieces[]; 

piece::piece(int initialPosX, int initialPosY)
{
	mVelX = 0;
	mVelY = 0;
    beforeStrikeSpeed = 10;

	mPosX = initialPosX;
	mPosY = initialPosY;

    potted = 0;
    possibleToStrike = 1;

    collider.x = mPosX + (texture.getWidth()) / 2;
    collider.y = mPosY + (texture.getHeight()) / 2;
    collider.r = texture.getWidth() / 2;
}

void piece::initialize(double initialPosX, double initialPosY)
{
    speed = 0;
    mPosX = initialPosX;
    mPosY = initialPosY;

    mVelX = 0;
    mVelY = 0;
    beforeStrikeSpeed = 10;

    potted = 0;
    possibleToStrike = 1;

    collider.x = mPosX + (texture.getWidth()) / 2;
    collider.y = mPosY + (texture.getHeight()) / 2;
    collider.r = texture.getWidth() / 2;
}

void piece::handleEvent(SDL_Event e, int currPlayerID, int totalPlayers)
{
    if(striked) return;
    int mousePosX;
    int mousePosY;
    SDL_GetMouseState(&mousePosX, &mousePosY);
    if(e.type == SDL_MOUSEBUTTONDOWN) std::cout << mousePosX << " " << mousePosY << std::endl; 

    // Adjusts the velocity of the striker before it is striked
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 && !striked){
        handleBeforeStrikeMovement(e, currPlayerID, totalPlayers);
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 && !striked){
        adjustVelocity_KeyReleased(e, currPlayerID, totalPlayers);
    }
    
    if(possibleToStrike){
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE){
            if(speed <= 60) speed += 1;
        }
        else if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE){
            int mousePosX;
            int mousePosY;
            SDL_GetMouseState(&mousePosX, &mousePosY);
            striked = 1;

            mVelX = (double) speed * (mousePosX - collider.x) / dist(collider.x, collider.y, mousePosX, mousePosY);
            mVelY = (double) speed * (mousePosY - collider.y) / dist(collider.x, collider.y, mousePosX, mousePosY);
        }
    }
}

void piece::move(int id, int teamID, int currPlayerID)
{
    if(striked){
        mPosX += mVelX;
        bool stepTakenX = 1;
        if(mPosX <= 35){
            mPosX -= mVelX, mVelX *= -1, stepTakenX = 0;
        }
        else if(mPosX + texture.getWidth() >= 945){
            mPosX -= mVelX, mVelX *= -1, stepTakenX = 0;
        }
        shiftColliders();

        mPosY += mVelY;
        bool stepTakenY = 1;
        if(mPosY <= 35){
            mPosY -= mVelY, mVelY *= -1, stepTakenY = 0;
        }
        else if(mPosY + texture.getHeight() >= 945){
            mPosY -= mVelY, mVelY *= -1, stepTakenY = 0;
        }
        shiftColliders();

        // handling collisions
        for(int i = 0; i < totalPieces; i++){
            // a piece can not collide with itself, so collision has to be checked with other pieces
            // collisions have to be checked with pieces that have not been potted yet
            
            if(i != id && !pieces[i].potted){
                if(checkCollision(pieces[id].collider, pieces[i].collider)){
                    // cout << "called " << striked << endl;
                    // if(striked) playSelectSound();
                     if(striked) playCollisionSound();
                    handleCollision(pieces[id], pieces[i]);
                }
            }
        }

        if(id != 0 && isPotted(id, teamID)) potted = 1;
        
        mVelX -= 0.02 * mVelX;
        mVelY -= 0.02 * mVelY;

        if(abs(mVelX) < 0.005) mVelX = 0;
        if(abs(mVelY) < 0.005) mVelY = 0;

        shiftColliders();
    }
    else{
        mPosX += mVelX;
        if(mPosX <= strikerLimit[currPlayerID].left){
            mPosX -= mVelX;
        }
        else if(mPosX + texture.getWidth() >= strikerLimit[currPlayerID].right){
            mPosX -= mVelX;
        }
        shiftColliders();

        mPosY += mVelY;
        if(mPosY <= strikerLimit[currPlayerID].left){
            mPosY -= mVelY;
        }
        else if(mPosY + texture.getWidth() >= strikerLimit[currPlayerID].right){
            mPosY -= mVelY;
        }
        shiftColliders();

        if(id == 0){
            bool collisionFlag = 0;
            for(int i = 1; i < totalPieces; i++){
                if( !pieces[i].potted &&  checkCollision(pieces[0].collider, pieces[i].collider )){
                    collisionFlag = 1;
                    break;
                }
            }
            if(collisionFlag) possibleToStrike = 0;
            else possibleToStrike = 1;
        }
    }
}

void piece::render()
{
	texture.render((int) mPosX, (int) mPosY);
}

void piece::freeTexture()
{
	texture.free();
}

void piece::shiftColliders()
{
    collider.x = mPosX + (texture.getWidth()) / 2;
    collider.y = mPosY + (texture.getHeight()) / 2;
    collider.r = texture.getWidth() / 2;
}

void piece::handleCollision(piece &piece1, piece &piece2)
{
    // handle velocity
    // derivation of the formula: https://physics.stackexchange.com/questions/599278/how-can-i-calculate-the-final-velocities-of-two-spheres-after-an-elastic-collisi
    
    circle circle1 = piece1.collider;
    circle circle2 = piece2.collider;

    double n_x = circle1.x - circle2.x;
    double n_y = circle1.y - circle2.y;

    double c = (n_x * (piece2.mVelX - piece1.mVelX) + n_y * (piece2.mVelY - piece1.mVelY)) / (distanceSq(n_x, n_y, 0, 0));

    piece1.mVelX += c * n_x;
    piece2.mVelX -= c * n_x;

    piece1.mVelY += c * n_y;
    piece2.mVelY -= c * n_y;

    // handle position
    // for details: https://www.youtube.com/watch?v=guWIF87CmBg&ab_channel=LongNguyen
    
    double centerDistance = dist(circle1.x, circle1.y, circle2.x, circle2.y);
    double overlap = circle1.r + circle2.r - centerDistance;
    double overlapX = overlap * (circle2.x - circle1.x) / centerDistance;
    double overlapY = overlap * (circle2.y - circle1.y) / centerDistance;

    piece1.mPosX -= overlapX / 2.0;
    piece1.mPosY -= overlapY / 2.0;

    piece2.mPosX += overlapX / 2.0;
    piece2.mPosY += overlapY / 2.0;

    piece1.shiftColliders();
    piece2.shiftColliders();
}

bool piece::isPotted(int id, int teamID)
{
    double width = collider.r * 2.0;
    bool insidePocket = 0;
    if(mPosX <= 62 && mPosY <= 65) insidePocket = 1;
    else if(mPosX + width >= 917 && mPosY <= 65) insidePocket = 1;
    else if(mPosX <= 62 && mPosY + width >= 917) insidePocket = 1;
    else if(mPosX + width >= 917 && mPosY + width >= 922) insidePocket = 1;

    // if the piece hits the pocket with too much velocity, it rebounds
    double absVel = sqrt(mVelX * mVelX + mVelY * mVelY);
    if(absVel > 40) insidePocket = false;

    if(insidePocket){
        mVelX = 0;
        mVelY = 0;
        lastPottedPieceID = id;

        if(id == redID) redPottedLastMove = true;
        
        // if the piece just potted belongs to the current player, then he gets another move
        if(id >= teamPieces[teamID].left && id <= teamPieces[teamID].right) pottedLastMove = 1;

        if(id >= teamPieces[0].left && id < teamPieces[0].right) pottedPieces[0].push_back(id);
        else if(id > teamPieces[1].left && id <= teamPieces[1].right) pottedPieces[1].push_back(id);
        // cout << "Potted " << id << " " << pottedPieces[0].size() << " " << pottedPieces[1].size() << endl;
    }

    return insidePocket;
}

void piece::handleBeforeStrikeMovement(SDL_Event e, int currPlayerID, int totalPlayers)
{
    if(totalPlayers == 4){
        if(currPlayerID % 2 == 0){
            switch( e.key.keysym.sym )
            {
                case SDLK_LEFT: mVelX -= beforeStrikeSpeed; break;
                case SDLK_RIGHT: mVelX += beforeStrikeSpeed; break;
            }
        }
        else{
            switch( e.key.keysym.sym )
            {
                case SDLK_UP: mVelY -= beforeStrikeSpeed; break;
                case SDLK_DOWN: mVelY += beforeStrikeSpeed; break;
            }
        }
    }
    else{
        switch( e.key.keysym.sym )
        {
            case SDLK_LEFT: mVelX -= beforeStrikeSpeed; break;
            case SDLK_RIGHT: mVelX += beforeStrikeSpeed; break;
        }
    }
}

void piece::adjustVelocity_KeyReleased(SDL_Event e, int currPlayerID, int totalPlayers)
{
    if(totalPlayers == 4){
        if(currPlayerID % 2 == 0){
            switch( e.key.keysym.sym )
            {
                case SDLK_LEFT: mVelX += beforeStrikeSpeed; break;
                case SDLK_RIGHT: mVelX -= beforeStrikeSpeed; break;
            }
        }
        else{
            switch( e.key.keysym.sym )
            {
                case SDLK_UP: mVelY += beforeStrikeSpeed; break;
                case SDLK_DOWN: mVelY -= beforeStrikeSpeed; break;
            }
        }
    }
    else{
        switch( e.key.keysym.sym )
        {
            case SDLK_LEFT: mVelX += beforeStrikeSpeed; break;
            case SDLK_RIGHT: mVelX -= beforeStrikeSpeed; break;
        }
    }
}