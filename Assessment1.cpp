// Assessment1.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <iomanip> // Include the <iomanip> header for setprecision
#include <sstream> // Include the <sstream> header for stringstream
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	//   myEngine->AddMediaFolder( "C:\\Folder name\\Media" );
    // The media folder is located inside the Assessment1 Folder
	myEngine->AddMediaFolder( "Add Path" );

	/**** Set up your scene here ****/
	// Load the platform mesh
	IMesh* platformMesh = myEngine->LoadMesh("platform.x");

	// Create the platform object and position it at (0, -5, 0)
	IModel* platform = platformMesh->CreateModel(0.0f, -5.0f, 0.0f);

	// Load the sphere mesh
	IMesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");

	// Create the sphere1 object and position it at (-85.0f, 10.0f, -85.0f)
	IModel* sphere1 = sphereMesh->CreateModel(-85.0f, 10.0f, -85.0f);

	// Load the cube mesh
	IMesh* cubeMesh = myEngine->LoadMesh("cubemesh.x");

	// Create the cube object and position it at (0, 5.0f, 0.0f)
	IModel* cube = cubeMesh->CreateModel(0.0f, 5.0f, 0.0f);

	// Add a camera
	ICamera* camera = myEngine->CreateCamera(kManual);
	camera->SetPosition(0.0f, 200.0f, -170.0f);
	camera->RotateX(45.0f);

	// Set the speeds for sphere movement and rotation
	float sphereMovementSpeed = 50.0f;  
	float sphereRotationSpeed = 150.0f; 

	// Create sphere2 and set its initial position
	IModel* sphere2 = sphereMesh->CreateModel(85.0f, 10.0f, 85.0f);

	// Set the initial speeds for sphere2 to match sphere1
	float sphere2MovementSpeed = sphereMovementSpeed;
	float sphere2RotationSpeed = sphereRotationSpeed;

	// Set the paused var
	bool paused = false;

    // Load and create a bullet at (0, 5.0f, 0)
    IMesh* bulletMesh = myEngine->LoadMesh("bullet.x");
    IModel* bullet = bulletMesh->CreateModel(0, 5.0f, 0);

    // Set the initial state of the bullet
    bool bulletFired = false;

    // Initialize the player's score
    int score = 0;

    // Create a font object
    IFont* font = myEngine->LoadFont("Font2.bmp", 30);

    int screenWidth = myEngine->GetWidth();
    int screenHeight = myEngine->GetHeight();

    bool allSpheresRemoved = false;

    std::string superSphereSkin = "super.jpg";

    // Create sphere3 and set its initial position
    IModel* sphere3 = sphereMesh->CreateModel(-85.0f, 10.0f, 85.0f);
    sphere3->SetSkin(superSphereSkin);

    // Create sphere4 and set its initial position
    IModel* sphere4 = sphereMesh->CreateModel(85.0f, 10.0f, -85.0f);
    sphere4->SetSkin(superSphereSkin);

    enum GameState { Playing, Paused, GameWon, GameOver };
    GameState currentState = Playing;

    //Number of bullets
    int bullets = 10;

    int sphere3shot = 1;
    int sphere4shot = 1;

    //Direction
    bool clockwiseRotation = true;


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
        // Get the frame time
        float frameTime = myEngine->Timer();

        //Display the score, speed and the number of bullets
        std::string scoreText = "Score : " + std::to_string(score);
        std::string bulletsText = "Bullets : " + std::to_string(bullets);
        std::stringstream speedStream;
        speedStream << std::fixed << std::setprecision(1) << sphereMovementSpeed;
        std::string speedText = "Speed: " + speedStream.str();        
        float textWidth = font->MeasureTextWidth(bulletsText);

        font->Draw(scoreText, 20, 20, kWhite);
        font->Draw(speedText, 20, 50, kWhite);
        font->Draw(bulletsText, screenWidth-textWidth -20, 20, kWhite);

        
        // Check for keyboard input P
        if (myEngine->KeyHit(Key_P))
        {
            // Toggle pause state
            paused = !paused;
            if (paused)
                currentState = Paused;
            else
                currentState = Playing;
                
        }

        // Check for pause state
        if (currentState == Playing && !paused)
        {
            // Calculate the distances to move this frame
            float moveDistance = sphereMovementSpeed * frameTime ;

            // Calculate the angles to rotate this frame
            float rotateAngle = sphereRotationSpeed * frameTime;
            
            if (clockwiseRotation) {
                // Move the sphere1
                if (sphere1->GetX() == -85.0f && sphere1->GetZ() < 85.0f)
                {
                    float newZ = sphere1->GetZ() + moveDistance;
                    if (newZ > 85.0f)
                        newZ = 85.0f;

                    sphere1->SetPosition(-85.0f, sphere1->GetY(), newZ);
                    sphere1->RotateX(rotateAngle);
                }
                else if (sphere1->GetZ() == 85.0f && sphere1->GetX() < 85.0f)
                {
                    float newX = sphere1->GetX() + moveDistance;
                    if (newX > 85.0f)
                        newX = 85.0f;

                    sphere1->SetPosition(newX, sphere1->GetY(), 85.0f);
                    sphere1->RotateZ(-rotateAngle);
                }
                else if (sphere1->GetX() == 85.0f && sphere1->GetZ() > -85.0f)
                {
                    float newZ = sphere1->GetZ() - moveDistance;
                    if (newZ < -85.0f)
                        newZ = -85.0f;

                    sphere1->SetPosition(85.0f, sphere1->GetY(), newZ);
                    sphere1->RotateX(-rotateAngle);
                }
                else if (sphere1->GetZ() == -85.0f && sphere1->GetX() > -85.0f)
                {
                    float newX = sphere1->GetX() - moveDistance;
                    if (newX < -85.0f)
                        newX = -85.0f;

                    sphere1->SetPosition(newX, sphere1->GetY(), -85.0f);
                    sphere1->RotateZ(rotateAngle);
                }
                else
                {
                    sphere1->SetPosition(-85.0f, sphere1->GetY(), -85.0f);
                    sphere1->ResetOrientation();
                }

                
                

                // Move the sphere2
                if (sphere2->GetX() == -85.0f && sphere2->GetZ() < 85.0f)
                {
                    float newZ = sphere2->GetZ() + moveDistance;
                    if (newZ > 85.0f)
                        newZ = 85.0f;

                    sphere2->SetPosition(-85.0f, sphere2->GetY(), newZ);
                    sphere2->RotateX(rotateAngle);
                }
                else if (sphere2->GetZ() == 85.0f && sphere2->GetX() < 85.0f)
                {
                    float newX = sphere2->GetX() + moveDistance;
                    if (newX > 85.0f)
                        newX = 85.0f;

                    sphere2->SetPosition(newX, sphere2->GetY(), 85.0f);
                    sphere2->RotateZ(-rotateAngle);
                }
                else if (sphere2->GetX() == 85.0f && sphere2->GetZ() > -85.0f)
                {
                    float newZ = sphere2->GetZ() - moveDistance;
                    if (newZ < -85.0f)
                        newZ = -85.0f;

                    sphere2->SetPosition(85.0f, sphere2->GetY(), newZ);
                    sphere2->RotateX(-rotateAngle);
                }
                else if (sphere2->GetZ() == -85.0f && sphere2->GetX() > -85.0f)
                {
                    float newX = sphere2->GetX() - moveDistance;
                    if (newX < -85.0f)
                        newX = -85.0f;

                    sphere2->SetPosition(newX, sphere2->GetY(), -85.0f);
                    sphere2->RotateZ(rotateAngle);
                }
                else
                {
                    sphere2->SetPosition(85.0f, sphere2->GetY(), 85.0f);
                    sphere2->ResetOrientation();
                }



                // Move the sphere3
                if (sphere3->GetX() == -85.0f && sphere3->GetZ() < 85.0f)
                {
                    float newZ = sphere3->GetZ() + moveDistance;
                    if (newZ > 85.0f)
                        newZ = 85.0f;

                    sphere3->SetPosition(-85.0f, sphere3->GetY(), newZ);
                    sphere3->RotateX(rotateAngle);
                }
                else if (sphere3->GetZ() == 85.0f && sphere3->GetX() < 85.0f)
                {
                    float newX = sphere3->GetX() + moveDistance;
                    if (newX > 85.0f)
                        newX = 85.0f;

                    sphere3->SetPosition(newX, sphere3->GetY(), 85.0f);
                    sphere3->RotateZ(-rotateAngle);
                }
                else if (sphere3->GetX() == 85.0f && sphere3->GetZ() > -85.0f)
                {
                    float newZ = sphere3->GetZ() - moveDistance;
                    if (newZ < -85.0f)
                        newZ = -85.0f;

                    sphere3->SetPosition(85.0f, sphere3->GetY(), newZ);
                    sphere3->RotateX(-rotateAngle);
                }
                else if (sphere3->GetZ() == -85.0f && sphere3->GetX() > -85.0f)
                {
                    float newX = sphere3->GetX() - moveDistance;
                    if (newX < -85.0f)
                        newX = -85.0f;

                    sphere3->SetPosition(newX, sphere3->GetY(), -85.0f);
                    sphere3->RotateZ(rotateAngle);
                }
                else
                {
                    sphere3->SetPosition(-85.0f, sphere3->GetY(), 85.0f);
                    sphere3->ResetOrientation();
                }


                // Move the sphere4
                if (sphere4->GetX() == -85.0f && sphere4->GetZ() < 85.0f)
                {
                    float newZ = sphere4->GetZ() + moveDistance;
                    if (newZ > 85.0f)
                        newZ = 85.0f;

                    sphere4->SetPosition(-85.0f, sphere4->GetY(), newZ);
                    sphere4->RotateX(rotateAngle);
                }
                else if (sphere4->GetZ() == 85.0f && sphere4->GetX() < 85.0f)
                {
                    float newX = sphere4->GetX() + moveDistance;
                    if (newX > 85.0f)
                        newX = 85.0f;

                    sphere4->SetPosition(newX, sphere4->GetY(), 85.0f);
                    sphere4->RotateZ(-rotateAngle);
                }
                else if (sphere4->GetX() == 85.0f && sphere4->GetZ() > -85.0f)
                {
                    float newZ = sphere4->GetZ() - moveDistance;
                    if (newZ < -85.0f)
                        newZ = -85.0f;

                    sphere4->SetPosition(85.0f, sphere4->GetY(), newZ);
                    sphere4->RotateX(-rotateAngle);
                }
                else if (sphere4->GetZ() == -85.0f && sphere4->GetX() > -85.0f)
                {
                    float newX = sphere4->GetX() - moveDistance;
                    if (newX < -85.0f)
                        newX = -85.0f;

                    sphere4->SetPosition(newX, sphere4->GetY(), -85.0f);
                    sphere4->RotateZ(rotateAngle);
                }
                else
                {
                    sphere4->SetPosition(85.0f, sphere4->GetY(), -85.0f);
                    sphere4->ResetOrientation();
                }

            }
            else {
                // Move the sphere1
                if (sphere1->GetX() == -85.0f && sphere1->GetZ() > -85.0f)
                {
                    float newZ = sphere1->GetZ() - moveDistance;
                    if (newZ < -85.0f)
                        newZ = -85.0f;

                    sphere1->SetPosition(-85.0f, sphere1->GetY(), newZ);
                    sphere1->RotateX(-rotateAngle);
                }
                else if (sphere1->GetZ() == 85.0f && sphere1->GetX() > -85.0f)
                {
                    float newX = sphere1->GetX() - moveDistance;
                    if (newX < -85.0f)
                        newX = -85.0f;

                    sphere1->SetPosition(newX, sphere1->GetY(), 85.0f);
                    sphere1->RotateZ(rotateAngle);
                }
                else if (sphere1->GetX() == 85.0f && sphere1->GetZ() < 85.0f)
                {
                    float newZ = sphere1->GetZ() + moveDistance;
                    if (newZ > 85.0f)
                        newZ = 85.0f;

                    sphere1->SetPosition(85.0f, sphere1->GetY(), newZ);
                    sphere1->RotateX(rotateAngle);
                }
                else if (sphere1->GetZ() == -85.0f && sphere1->GetX() < 85.0f)
                {
                    float newX = sphere1->GetX() + moveDistance;
                    if (newX > 85.0f)
                        newX = 85.0f;

                    sphere1->SetPosition(newX, sphere1->GetY(), -85.0f);
                    sphere1->RotateZ(-rotateAngle);
                }
                else
                {
                    sphere1->SetPosition(-85.0f, sphere1->GetY(), -85.0f);
                    sphere1->ResetOrientation();
                }


                // Move the sphere2
                if (sphere2->GetX() == -85.0f && sphere2->GetZ() > -85.0f)
                {
                    float newZ = sphere2->GetZ() - moveDistance;
                    if (newZ < -85.0f)
                        newZ = -85.0f;

                    sphere2->SetPosition(-85.0f, sphere2->GetY(), newZ);
                    sphere2->RotateX(-rotateAngle);
                }
                else if (sphere2->GetZ() == 85.0f && sphere2->GetX() > -85.0f)
                {
                    float newX = sphere2->GetX() - moveDistance;
                    if (newX < -85.0f)
                        newX = -85.0f;

                    sphere2->SetPosition(newX, sphere2->GetY(), 85.0f);
                    sphere2->RotateZ(rotateAngle);
                }
                else if (sphere2->GetX() == 85.0f && sphere2->GetZ() < 85.0f)
                {
                    float newZ = sphere2->GetZ() + moveDistance;
                    if (newZ > 85.0f)
                        newZ = 85.0f;

                    sphere2->SetPosition(85.0f, sphere2->GetY(), newZ);
                    sphere2->RotateX(rotateAngle);
                }
                else if (sphere2->GetZ() == -85.0f && sphere2->GetX() < 85.0f)
                {
                    float newX = sphere2->GetX() + moveDistance;
                    if (newX > 85.0f)
                        newX = 85.0f;

                    sphere2->SetPosition(newX, sphere2->GetY(), -85.0f);
                    sphere2->RotateZ(-rotateAngle);
                }
                else
                {
                    sphere2->SetPosition(85.0f, sphere2->GetY(), 85.0f);
                    sphere2->ResetOrientation();
                }


                // Move the sphere3
                if (sphere3->GetX() == -85.0f && sphere3->GetZ() > -85.0f)
                {
                    float newZ = sphere3->GetZ() - moveDistance;
                    if (newZ < -85.0f)
                        newZ = -85.0f;

                    sphere3->SetPosition(-85.0f, sphere3->GetY(), newZ);
                    sphere3->RotateX(-rotateAngle);
                }
                else if (sphere3->GetZ() == 85.0f && sphere3->GetX() > -85.0f)
                {
                    float newX = sphere3->GetX() - moveDistance;
                    if (newX < -85.0f)
                        newX = -85.0f;

                    sphere3->SetPosition(newX, sphere3->GetY(), 85.0f);
                    sphere3->RotateZ(rotateAngle);
                }
                else if (sphere3->GetX() == 85.0f && sphere3->GetZ() < 85.0f)
                {
                    float newZ = sphere3->GetZ() + moveDistance;
                    if (newZ > 85.0f)
                        newZ = 85.0f;

                    sphere3->SetPosition(85.0f, sphere3->GetY(), newZ);
                    sphere3->RotateX(rotateAngle);
                }
                else if (sphere3->GetZ() == -85.0f && sphere3->GetX() < 85.0f)
                {
                    float newX = sphere3->GetX() + moveDistance;
                    if (newX > 85.0f)
                        newX = 85.0f;

                    sphere3->SetPosition(newX, sphere3->GetY(), -85.0f);
                    sphere3->RotateZ(-rotateAngle);
                }
                else
                {
                    sphere3->SetPosition(-85.0f, sphere3->GetY(), 85.0f);
                    sphere3->ResetOrientation();
                }


                // Move the sphere4
                if (sphere4->GetX() == -85.0f && sphere4->GetZ() > -85.0f)
                {
                    float newZ = sphere4->GetZ() - moveDistance;
                    if (newZ < -85.0f)
                        newZ = -85.0f;

                    sphere4->SetPosition(-85.0f, sphere4->GetY(), newZ);
                    sphere4->RotateX(-rotateAngle);
                }
                else if (sphere4->GetZ() == 85.0f && sphere4->GetX() > -85.0f)
                {
                    float newX = sphere4->GetX() - moveDistance;
                    if (newX < -85.0f)
                        newX = -85.0f;

                    sphere4->SetPosition(newX, sphere4->GetY(), 85.0f);
                    sphere4->RotateZ(rotateAngle);
                }
                else if (sphere4->GetX() == 85.0f && sphere4->GetZ() < 85.0f)
                {
                    float newZ = sphere4->GetZ() + moveDistance;
                    if (newZ > 85.0f)
                        newZ = 85.0f;

                    sphere4->SetPosition(85.0f, sphere4->GetY(), newZ);
                    sphere4->RotateX(rotateAngle);
                }
                else if (sphere4->GetZ() == -85.0f && sphere4->GetX() < 85.0f)
                {
                    float newX = sphere4->GetX() + moveDistance;
                    if (newX > 85.0f)
                        newX = 85.0f;

                    sphere4->SetPosition(newX, sphere4->GetY(), -85.0f);
                    sphere4->RotateZ(-rotateAngle);
                }
                else
                {
                    sphere4->SetPosition(85.0f, sphere4->GetY(), -85.0f);
                    sphere4->ResetOrientation();
                }

            }



            if (myEngine->KeyHit(Key_D))
            {
                // Increase the sphere speeds by 10% of the initial value
                sphereMovementSpeed += 0.1f * sphereMovementSpeed;
                sphereRotationSpeed += 0.1f * sphereRotationSpeed;
                sphere2MovementSpeed += 0.1f * sphere2MovementSpeed;
                sphere2RotationSpeed += 0.1f * sphere2RotationSpeed;
            }
            else if (myEngine->KeyHit(Key_A))
            {
                // Decrease the sphere speeds by 10% of the initial value
                sphereMovementSpeed -= 0.1f * sphereMovementSpeed;
                sphereRotationSpeed -= 0.1f * sphereRotationSpeed;
                sphere2MovementSpeed -= 0.1f * sphere2MovementSpeed;
                sphere2RotationSpeed -= 0.1f * sphere2RotationSpeed;

                // Check for minimum speed limits
                if (sphereMovementSpeed < 0.5f * sphereMovementSpeed)
                {
                    sphereMovementSpeed = 0.5f * sphereMovementSpeed;
                }
                if (sphere2MovementSpeed < 0.5f * sphere2MovementSpeed)
                {
                    sphere2MovementSpeed = 0.5f * sphere2MovementSpeed;
                }
                if (sphereRotationSpeed < 0.5f * sphereRotationSpeed)
                {
                    sphereRotationSpeed = 0.5f * sphereRotationSpeed;
                }
                if (sphere2RotationSpeed < 0.5f * sphere2RotationSpeed)
                {
                    sphere2RotationSpeed = 0.5f * sphere2RotationSpeed;
                }
            }

            // Check for keyboard input Up
            if (myEngine->KeyHit(Key_Up))
            {
                // Fire the bullet
                if (!bulletFired)
                {
                    bulletFired = true;
                    bullets -= 1;
                    bullet->SetPosition(0, 5.0f, 0);
                }
            }



            // Update the bullet if it is fired
            if (bulletFired)
            {
                // Calculate the distance between the bullet and the cube
                float cubeDistanceX = bullet->GetX() - cube->GetX();
                float cubeDistanceY = bullet->GetY() - cube->GetY();
                float cubeDistanceZ = bullet->GetZ() - cube->GetZ();

                // Calculate the overall distance using the length of the vector
                float cubeDistance = sqrt(cubeDistanceX * cubeDistanceX + cubeDistanceY * cubeDistanceY + cubeDistanceZ * cubeDistanceZ);

                // Move the bullet forward at 5 times the sphere movement speed
                float bulletMoveDistance = 5.0f * sphereMovementSpeed * frameTime;
                bullet->MoveZ(bulletMoveDistance);

                // Check if the bullet is out of range or hit a sphere
                if (cubeDistance >= 200.0f)
                {
                    bullet->SetPosition(0, 5.0f, 0);
                    bulletFired = false;
                }
                else
                {
                    // Check the distance between the bullet and each sphere
                    float sphere1DistanceX = bullet->GetX() - sphere1->GetX();
                    float sphere1DistanceY = bullet->GetY() - sphere1->GetY();
                    float sphere1DistanceZ = bullet->GetZ() - sphere1->GetZ();
                    float sphere1Distance = sqrt(sphere1DistanceX * sphere1DistanceX + sphere1DistanceY * sphere1DistanceY + sphere1DistanceZ * sphere1DistanceZ);

                    float sphere2DistanceX = bullet->GetX() - sphere2->GetX();
                    float sphere2DistanceY = bullet->GetY() - sphere2->GetY();
                    float sphere2DistanceZ = bullet->GetZ() - sphere2->GetZ();
                    float sphere2Distance = sqrt(sphere2DistanceX * sphere2DistanceX + sphere2DistanceY * sphere2DistanceY + sphere2DistanceZ * sphere2DistanceZ);

                    float sphere3DistanceX = bullet->GetX() - sphere3->GetX();
                    float sphere3DistanceY = bullet->GetY() - sphere3->GetY();
                    float sphere3DistanceZ = bullet->GetZ() - sphere3->GetZ();
                    float sphere3Distance = sqrt(sphere3DistanceX * sphere3DistanceX + sphere3DistanceY * sphere3DistanceY + sphere3DistanceZ * sphere3DistanceZ);

                    float sphere4DistanceX = bullet->GetX() - sphere4->GetX();
                    float sphere4DistanceY = bullet->GetY() - sphere4->GetY();
                    float sphere4DistanceZ = bullet->GetZ() - sphere4->GetZ();
                    float sphere4Distance = sqrt(sphere4DistanceX * sphere4DistanceX + sphere4DistanceY * sphere4DistanceY + sphere4DistanceZ * sphere4DistanceZ);


                    // If the bullet hits sphere1, remove it and update the score
                    if (sphere1Distance < 10.0f)
                    {
                        sphere1->SetY(-100.0f); 
                        bullet->SetPosition(0, 5.0f, 0);
                        bulletFired = false;
                        score += 10;
                        sphereMovementSpeed += 0.25f * sphereMovementSpeed;
                        sphereRotationSpeed += 0.25f * sphereRotationSpeed;
                        sphere2MovementSpeed += 0.25f * sphere2MovementSpeed;
                        sphere2RotationSpeed += 0.25f * sphere2RotationSpeed;
                    }

                    // If the bullet hits sphere2, remove it and update the score
                    if (sphere2Distance < 10.0f)
                    {
                        sphere2->SetY(-100.0f); 
                        bullet->SetPosition(0, 5.0f, 0);
                        bulletFired = false;
                        score += 10;
                        sphereMovementSpeed += 0.25f * sphereMovementSpeed;
                        sphereRotationSpeed += 0.25f * sphereRotationSpeed;
                        sphere2MovementSpeed += 0.25f * sphere2MovementSpeed;
                        sphere2RotationSpeed += 0.25f * sphere2RotationSpeed;
                    }

                    // If the bullet hits sphere3
                    if (sphere3Distance < 10.0f)
                    {
                        if (sphere3shot == 1) {
                            sphere3->SetSkin("regular.jpg");
                            bullet->SetPosition(0, 5.0f, 0);
                            bulletFired = false;
                            score += 10;
                            sphere3shot = 0;
                        }
                        else {
                            sphere3->SetY(-100.0f);
                            bullet->SetPosition(0, 5.0f, 0);
                            bulletFired = false;
                            score += 10;
                        }
                        sphereMovementSpeed += 0.25f * sphereMovementSpeed;
                        sphereRotationSpeed += 0.25f * sphereRotationSpeed;
                        sphere2MovementSpeed += 0.25f * sphere2MovementSpeed;
                        sphere2RotationSpeed += 0.25f * sphere2RotationSpeed;
                    }

                    // If the bullet hits sphere4
                    if (sphere4Distance < 10.0f)
                    {
                        if (sphere4shot == 1) {
                            sphere4->SetSkin("regular.jpg");
                            bullet->SetPosition(0, 5.0f, 0);
                            bulletFired = false;
                            score += 10;
                            sphere4shot = 0;
                        }
                        else {
                            sphere4->SetY(-100.0f);
                            bullet->SetPosition(0, 5.0f, 0);
                            bulletFired = false;
                            score += 10;
                        }
                        sphereMovementSpeed += 0.25f * sphereMovementSpeed;
                        sphereRotationSpeed += 0.25f * sphereRotationSpeed;
                        sphere2MovementSpeed += 0.25f * sphere2MovementSpeed;
                        sphere2RotationSpeed += 0.25f * sphere2RotationSpeed;
                    }



                    if (sphere1->GetY() == -100.0f && sphere2->GetY() == -100.0f && sphere3->GetY() == -100.0f && sphere4->GetY() == -100.0f)
                    {
                        allSpheresRemoved = true;
                    }
                }
            }
        }

        if (myEngine->KeyHit(Key_R))
        {
            clockwiseRotation = !clockwiseRotation;
        }

        if (myEngine->KeyHit(Key_Escape))
        {
            myEngine->Stop();
        }


        // Check for keyboard input Space
        if (myEngine->KeyHit(Key_Space))
        {
            // Restart game
            bullets = 10;
            score = 0;
            sphere3shot = 1;
            sphere4shot = 1;
            currentState = Playing;
            sphereMovementSpeed = 50.0f;
            sphereRotationSpeed = 150.0f;
            sphere2MovementSpeed = 50.0f;
            sphere2RotationSpeed = 150.0f;
            sphere1->SetPosition(-85.0f, 10.0f, -85.0f);
            sphere2->SetPosition(85.0f, 10.0f, 85.0f);
            sphere3->SetPosition(-85.0f, 10.0f, 85.0f);
            sphere4->SetPosition(85.0f, 10.0f, -85.0f);
            sphere3->SetSkin("super.jpg");
            sphere4->SetSkin("super.jpg");
            allSpheresRemoved = false;
            clockwiseRotation = true;
        }



        if (currentState == Paused){
            font->Draw("Game Paused", (screenWidth) / 2, (screenHeight) / 2, kYellow, kCentre, kVCentre);
        }
        else if (currentState == GameWon) {
            font->Draw("Winner !!!", (screenWidth) / 2, (screenHeight) / 2, kGreen, kCentre, kVCentre);
        }
        else if (currentState == GameOver) {
            font->Draw("Game Over !!", (screenWidth) / 2, (screenHeight) / 2, kRed, kCentre, kVCentre);

        }


        // Check if all spheres are removed and display the winning message
        if (allSpheresRemoved) {
            currentState = GameWon;
        }

        if (bullets <= 0) {

            // Calculate the distance between the bullet and the cube
            float cubeDistanceX = bullet->GetX() - cube->GetX();
            float cubeDistanceY = bullet->GetY() - cube->GetY();
            float cubeDistanceZ = bullet->GetZ() - cube->GetZ();

            // Calculate the overall distance using the length of the vector
            float cubeDistance = sqrt(cubeDistanceX * cubeDistanceX + cubeDistanceY * cubeDistanceY + cubeDistanceZ * cubeDistanceZ);

            if (cubeDistance > 199) {
                currentState = GameOver;
                bullet->SetPosition(0, -500.0f, 0);
            }
        }

        // Draw the scene
        myEngine->DrawScene();

		/**** Update your scene each frame here ****/

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
