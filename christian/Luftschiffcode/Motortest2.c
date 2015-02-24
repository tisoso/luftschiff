// serial.c / serial.cpp
// A simple serial port writing example
// Written by Ted Burke - last updated 13-2-2013
// http://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinputkeyboard.html

// To compile with MinGW: gcc -o serial.exe serial.c
//
// To compile with cl, the Microsoft compiler: cl serial.cpp

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define CMD_FORWARD 'a'
#define CMD_BACKWARD 'b'
#define CMD_LEFT 'c'
#define CDM_RIGHT 'd'
#define CMD_UP 'e'
#define CMD_DOWN 'f'
#define CMD_STOP 'g'
#define CMD_END '/'
#define CMD_SPEED_LOW 1
#define CMD_SPEED_HIGH 5
#define SDL_SPEED 2

char int2Char[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

SDL_Window *window;
SDL_Surface *screen;
SDL_Surface *luftschiff, *hoch[3], *runter[3], *vor[3], *rueck[3], *links[3], *rechts[3];
/*TTF_Font *font;
SDL_Surface *text;
SDL_Color text_color = {255, 255, 255};*/
SDL_Event e;
SDL_Rect pos;

int initView() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    TTF_Init();
    window = SDL_CreateWindow("Luftschiff Controller v0.1", 400, 100, 700, 400, 0);

    // Lade Grafiken
    luftschiff = SDL_LoadBMP("icons/luftschiff.bmp");
    hoch[0] = SDL_LoadBMP("icons/hoch0.bmp");
    hoch[1] = SDL_LoadBMP("icons/hoch1.bmp");
    hoch[2] = SDL_LoadBMP("icons/hoch2.bmp");

    runter[0] = SDL_LoadBMP("icons/runter0.bmp");
    runter[1] = SDL_LoadBMP("icons/runter1.bmp");
    runter[2] = SDL_LoadBMP("icons/runter2.bmp");

    vor[0] = SDL_LoadBMP("icons/vor0.bmp");
    vor[1] = SDL_LoadBMP("icons/vor1.bmp");
    vor[2] = SDL_LoadBMP("icons/vor2.bmp");

    rueck[0] = SDL_LoadBMP("icons/rueck0.bmp");
    rueck[1] = SDL_LoadBMP("icons/rueck1.bmp");
    rueck[2] = SDL_LoadBMP("icons/rueck2.bmp");

    links[0] = SDL_LoadBMP("icons/links0.bmp");
    links[1] = SDL_LoadBMP("icons/links1.bmp");
    links[2] = SDL_LoadBMP("icons/links2.bmp");

    rechts[0] = SDL_LoadBMP("icons/rechts0.bmp");
    rechts[1] = SDL_LoadBMP("icons/rechts1.bmp");
    rechts[2] = SDL_LoadBMP("icons/rechts2.bmp");

    // Lade Font
//    font = TTF_OpenFont("FreeSans.ttf", 24);

    screen = SDL_GetWindowSurface(window);
    SDL_BlitSurface(luftschiff, NULL, screen, NULL);
    SDL_UpdateWindowSurface(window);

    SDL_Joystick *joystick;
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);


    return 0;
}

int main(int argc, char* argv[]) {
    initView();

    // Define the five bytes to send ("hello")
    char bytes_to_send[5];
    bytes_to_send[0] = 104;
    bytes_to_send[1] = 101;
    bytes_to_send[2] = 108;
    bytes_to_send[3] = 108;
    bytes_to_send[4] = 111;

    // Declare variables and structures
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};


    // Open the highest available serial port number
    fprintf(stderr, "Opening serial port...");
    hSerial = CreateFile("\\\\.\\COM3", GENERIC_READ|GENERIC_WRITE, 0, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if (hSerial == INVALID_HANDLE_VALUE)
    {
            fprintf(stderr, "Error\n");
            return 1;
    }
    else fprintf(stderr, "OK\n");

    // Set device parameters (38400 baud, 1 start bit,
    // 1 stop bit, no parity)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error getting device state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(SetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error setting device parameters\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if(SetCommTimeouts(hSerial, &timeouts) == 0)
    {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }


    // Mainloop
    int quit = 0, serCommandPos = 0, i, v, axe1v, axe1vAlt = -1, axe4v, axe4vAlt = -1;
    char serCommand[7];
    for (i = 0; i < sizeof(serCommand); i++) {
        serCommand[i] = '\0';
    }

    int forward = 0, backward = 0, left = 0, right = 0, up = 0, down = 0, commandsOld = 0, commands = 0, speed = 1, sdlSpeed = 1;

    while(!quit) {
        while(SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                printf("mouse\n");
                break;
            case SDL_JOYBUTTONDOWN:
                //printf("Joystick-Button: %d\n", e.jbutton.button);
                if (e.jbutton.button == 11) {
                    quit = 1;
                }
                break;
            case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
                //if ( ( e.jaxis.value < -3200 ) || (e.jaxis.value > 3200 ) )  {
                v = e.jaxis.value;
                    // Axe 0: links <-> rechts
                    if( e.jaxis.axis == 0)
                    {
                        //printf("Joystick left <-> right %d\n", e.jaxis.value);
                        if (v < -8000 && v > -23000) {
                            left = 1;  right = 0;
                        }
                        else if (v <= -23000) {
                            left = 2; right = 0;
                        }
                        else if(v > 8000 && v < 23000) {
                            left = 0; right = 1;
                        }
                        else if (v >= 23000) {
                            left = 0; right = 2;
                        }
                        else {
                            left = 0; right= 0;
                        }
                    }

                    // Axe 1: hoch <-> runter SERVO
                    if( e.jaxis.axis == 1)  {
                        printf("%d\n", (32768 - v) * 127 / 32768);
                        axe1v = (32768 - v) * 127 / 32768;
                    // * 255 / 32678 + 127; //255 - (v * (-1) * 255 / 32678 + 255) / 2; //(v * 255 / 32678 + 255);//(v * (-1) * 255 / 32678);
                        printf("Joystick up <-> down %d\n", axe1v);
                        if (v < -8000 && v > -23000) {
                            forward = 1;  backward = 0;
                        }
                        else if (v <= -23000) {
                            forward = 2; backward = 0;
                        }
                        else if(v > 8000 && v < 23000) {
                            forward = 0; backward = 1;
                        }
                        else if (v >= 23000) {
                            forward = 0; backward = 2;
                        }
                        else {
                            forward = 0; backward = 0;
                        }
                    }

                    // Axe 4: hoch MOTOR2
                    if( e.jaxis.axis == 4)  {
                        axe4v = 255 - (v * (-1) * 255 / 32678 + 255) / 2;
                        printf("Joystick 4 %d\n", axe4v);
                        if (v > -31000 && v <= 0) {
                            up = 1; down = 0;
                        }
                        else if (v > 0) {
                            up = 2; down = 0;
                        }
                        else {
                            up = 0; down = 0;
                        }
                    }

                    // Axe 5: rechts
                    if( e.jaxis.axis == 5)  {
                        //printf("Joystick 5 %d\n", e.jaxis.value);
                        if (v > -31000 && v <= 0) {
                            up = 0; down = 1;
                        }
                        else if (v > 0) {
                            up = 0; down = 2;
                        }
                        else {
                            up = 0; down = 0;
                        }
                    }

                //}
                break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym) {
                    case SDLK_w:
                        forward = 1;
                        backward = 0;
                        break;
                    case SDLK_s:
                        backward = 1;
                        forward = 0;
                        break;
                    case SDLK_a:
                        left = 1;
                        right = 0;
                        break;
                    case SDLK_d:
                        right = 1;
                        left = 0;
                        break;
                    case SDLK_v:
                        up = 1;
                        down = 0;
                        break;
                    case SDLK_SPACE:
                        down = 1;
                        up = 0;
                        break;
                    case SDLK_RSHIFT:
                    case SDLK_LSHIFT:
                        speed = CMD_SPEED_HIGH;
                        sdlSpeed = SDL_SPEED;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(e.key.keysym.sym) {
                    case SDLK_w:
                        forward = 0;
                        break;
                    case SDLK_s:
                        backward = 0;
                        break;
                    case SDLK_a:
                        left = 0;
                        break;
                    case SDLK_d:
                        right = 0;
                        break;
                    case SDLK_v:
                        up = 0;
                        break;
                    case SDLK_SPACE:
                        down = 0;
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        speed = CMD_SPEED_LOW;
                        sdlSpeed = 1;
                        break;
                }
                break;
            }

            commandsOld = commands;
            commands = 64 * speed + 32 * up + 16 * down + 8*forward + 4*backward + 2 * left + 1 * right;


            // Wenn Kommandos verändert wurden
            if (commandsOld != commands || axe1v != axe1vAlt || axe4v != axe1vAlt) {
                // Debug
                system("cls");
                printf("  %d    %d\n%d   %d  %d\n  %d\n\n", forward * speed, up * speed, left * speed, right * speed, down * speed, backward * speed);

                // Serial command
                serCommandPos = 0;
                if (forward) {
                    serCommand[serCommandPos] = CMD_FORWARD;
                    serCommand[serCommandPos+1] = int2Char[forward];
                    serCommandPos+=2;
                }
                if (backward) {
                    serCommand[serCommandPos] = CMD_BACKWARD;
                    serCommand[serCommandPos+1] = int2Char[backward];
                    serCommandPos+=2;
                }
                if (left) {
                    serCommand[serCommandPos] = CMD_LEFT;
                    serCommand[serCommandPos+1] = int2Char[left];
                    serCommandPos+=2;
                }
                if (right) {
                    serCommand[serCommandPos] = CDM_RIGHT;
                    serCommand[serCommandPos+1] = int2Char[right];
                    serCommandPos+=2;
                }
                if (up) {
                    serCommand[serCommandPos] = CMD_UP;
                    serCommand[serCommandPos+1] = int2Char[up];
                    serCommandPos+=2;
                }
                if (down) {
                    serCommand[serCommandPos] = CMD_DOWN;
                    serCommand[serCommandPos+1] = int2Char[down];
                    serCommandPos+=2;
                }
                if (!serCommandPos) {
                    serCommand[serCommandPos] = CMD_STOP;
                    serCommand[serCommandPos+1] = '0';
                    serCommandPos+=2;
                }

                serCommand[serCommandPos] = CMD_END;

                for (i = 0; i <= serCommandPos; i++) {
                    printf("%c", serCommand[i]);
                }
                serCommand[i] = '\0';
                printf("\n");

                // TEMP
                axe1vAlt = axe1v;
                axe4vAlt = axe4v;

                serCommand[1] = (axe1v / 16 * 16) + (axe4v / 16);
                serCommand[0] = 0;
                printf("%d\n", serCommand[0]);
                //fprintf(stderr, "%d\n", serCommand[0]);


                // Send specified m,text (remaining command line arguments)
                DWORD bytes_written, total_bytes_written = 0;
                fprintf(stderr, "Sending bytes...");
                if(!WriteFile(hSerial, serCommand, 2, &bytes_written, NULL))
                {
                    fprintf(stderr, "Error\n");
                    CloseHandle(hSerial);
                    return 1;
                }
                fprintf(stderr, "%d bytes written: %d, %d\n", bytes_written, serCommand[0], axe1v);


                // SDL
                // runter
                pos.x = 319; pos.y = 300;
                SDL_BlitSurface(runter[down], NULL, screen, &pos);
                SDL_UpdateWindowSurface(window);
                // hoch
                pos.x = 319; pos.y = 30;
                SDL_BlitSurface(hoch[up], NULL, screen, &pos);
                SDL_UpdateWindowSurface(window);
                // vor
                pos.x = 605; pos.y = 166;
                SDL_BlitSurface(vor[forward], NULL, screen, &pos);
                SDL_UpdateWindowSurface(window);
                // rueck
                pos.x = 36; pos.y = 166;
                SDL_BlitSurface(rueck[backward], NULL, screen, &pos);
                SDL_UpdateWindowSurface(window);
                // links
                pos.x = 110; pos.y = 290;
                SDL_BlitSurface(links[left], NULL, screen, &pos);
                SDL_UpdateWindowSurface(window);
                // rechts
                pos.x = 527; pos.y = 290;
                SDL_BlitSurface(rechts[right], NULL, screen, &pos);
                // Text
                //text = TTF_RenderText_Solid(font, "x" ,text_color);
                //SDL_BlitSurface(text, NULL, screen, NULL);

                SDL_SetWindowTitle(window, serCommand);
                SDL_UpdateWindowSurface(window);
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
/*
    // Send specified m,text (remaining command line arguments)
    DWORD bytes_written, total_bytes_written = 0;
    fprintf(stderr, "Sending bytes...");
    if(!WriteFile(hSerial, bytes_to_send, 5, &bytes_written, NULL))
    {
        fprintf(stderr, "Error\n");
        CloseHandle(hSerial);
        return 1;
    }
    fprintf(stderr, "%d bytes written\n", bytes_written);*/

    // Close serial port
    fprintf(stderr, "Closing serial port...");
    if (CloseHandle(hSerial) == 0)
    {
        fprintf(stderr, "Error\n");
        return 1;
    }
    fprintf(stderr, "OK\n");

    // exit normally
    return 0;
}
