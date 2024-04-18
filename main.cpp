#include <raylib.h>
#include <iostream>
#include <cstdint>
#include <string>

#if _DEBUG
	#define WinMain main
#endif // _DEBUG


#define IsKeyActive(x) (IsKeyPressed(x) || IsKeyDown(x))
#define GRAVITY -2.50f
#define MU 0.1f

struct Player {
	Rectangle box = { 240, 0, 50, 50 };
	Vector2 acceleration = { 0.0f, 0.0f };
	Vector2 velocity = { 0.0f, 0.0f };
	bool onGround = false;
	bool onLeftWall = false;
	bool onRightWall = false;
};

float CalculateFriction(Player &player);
void ProcessPlayerPhysics(Player &player);
void ProcessInput(Player &player);

int WinMain() {

	Player player;

	InitWindow(500, 500, "Jump Test");
	SetTargetFPS(60);
	DrawFPS(0, 0);

	while (!WindowShouldClose()) {
		
		ProcessInput(player);
		ProcessPlayerPhysics(player);
		

		BeginDrawing();
			DrawRectangle(player.box.x, player.box.y, player.box.width, player.box.height, BLACK);
			ClearBackground(BLUE);
		EndDrawing();
	}

}

float CalculateFriction(Player &player) {
	float frictionForce = 0.0f;
	short int direction = 0;
	(player.velocity.x < 0) ? direction = 1 : direction = -1;

	frictionForce = ((GRAVITY * direction)) * MU;

	if (player.velocity.x == 0) {
		frictionForce = 0;
	}

	return frictionForce;
}

void ProcessPlayerPhysics(Player &player) {
	
	float frictionForce = 0.0f;
	player.velocity.y -= player.acceleration.y * GetFrameTime();

	if ((player.box.y > 500 - player.box.height) && !(player.box.y == 500 - player.box.height) && player.onGround == false) {
		player.velocity.y = 0;
		player.acceleration.y = 0;
		player.box.y = 500 - player.box.height;
		player.onGround = true;
	}
	else if(player.box.y >= 500 - player.box.height) {
		player.onGround = true;
	} 
	else {
		player.acceleration.y += GRAVITY;
		player.onGround = false;
	}

	if (player.box.x < 0 && !player.onLeftWall) {
		player.velocity.x = 0;
		player.acceleration.x = 0;
		player.onLeftWall = true;
	} 
	if (player.box.x > 500 - player.box.width && !player.onRightWall) {
		player.velocity.x = 0;
		player.acceleration.x = 0;
		player.onRightWall = true;		
	}

	if (player.onLeftWall == true) {
		player.onLeftWall = false;
		player.box.x = 0;
	}
	if (player.onRightWall == true) {
		player.onRightWall = false;
		player.box.x = 500 - player.box.width;
	}

	if (player.onGround) {
		frictionForce = CalculateFriction(player);
	}
	
	player.velocity.x -= frictionForce;

	DrawText(std::to_string(player.velocity.x).c_str(), 1, 1, 25, BLACK );
	DrawText(std::to_string(frictionForce).c_str(), 1, 26, 25, GREEN);

	player.box.x += player.velocity.x;
	player.box.y += player.velocity.y + ((player.acceleration.y * GetFrameTime() * GetFrameTime()) / 2);
}

void ProcessInput(Player &player) {

	if (IsKeyActive(KEY_SPACE) && player.onGround) {

		player.velocity.y -= 10.0f;

		if (IsKeyActive(KEY_LEFT_SHIFT))
			player.velocity.y -= 10.0f;

		std::clog << "Jump" << std::endl;
		ProcessPlayerPhysics(player);

	} if (IsKeyActive(KEY_A)) {
		player.velocity.x -= 1.0f;
	} if (IsKeyActive(KEY_D)) {
		player.velocity.x += 1.0f;
	}

}
