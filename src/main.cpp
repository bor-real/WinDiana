#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>

// 2023-05-23 23:03:44 this code is cursed

struct App {
    std::string appName;
    SDL_Texture* appIcon;
    void (*appFunction)();
    bool graphical;
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

short currentStage = 0;

bool mouseLeftDown;
bool menuEnabled;

void appLogOff() {
    currentStage = 1;
}

void appShutDown() {
    SDL_Event quitEvent;
    quitEvent.type = SDL_QUIT;
    SDL_PushEvent(&quitEvent);
}

void foo() {
    std::cout << "bar" << std::endl;
}

void renderLoadingStage(SDL_Renderer* renderer, SDL_Texture* loadingTexture, SDL_Texture* logoTexture, Uint32 startTime) {
    Uint32 speed = 10; // Adjust the speed factor as needed
    Uint32 ticks = speed * SDL_GetTicks();
    Uint32 seconds = ticks / 1000;
    Uint32 sprite = seconds % 17;

    const int spriteWidth = 175;
    const int spriteHeight = 42;

    const int logoWidth = 175;
    const int logoHeight = 175;

    const int logoX = (SCREEN_WIDTH - logoWidth) / 2;
    const int logoY = (SCREEN_HEIGHT - logoHeight) / 2;

    const int spriteX = (SCREEN_WIDTH - spriteWidth) / 2;
    const int spriteY = (SCREEN_HEIGHT - spriteHeight) * 0.75;

    SDL_Rect srcrect1 = { 0, sprite * spriteHeight, spriteWidth, spriteHeight };
    SDL_Rect dstrect1 = { spriteX, spriteY, spriteWidth, spriteHeight };
    SDL_Rect srcrect2 = { logoX, logoY, logoWidth, logoHeight };

    SDL_RenderCopy(renderer, loadingTexture, &srcrect1, &dstrect1); // Render the loading bar animation
    SDL_RenderCopy(renderer, logoTexture, NULL, &srcrect2); // Render the logo image

    // Check if 5 seconds have elapsed and switch to the next stage
    if (SDL_GetTicks() - startTime >= 1000) {
        currentStage++;
    }
}

void renderLoginStage(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Texture* adminTexture, SDL_Texture* loginTexture) {
    const int adminWidth = 240;
    const int adminHeight = 60;

    const int loginWidth = 240;
    const int loginHeight = 120;

    const int adminX = (SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 30);
    const int adminY = (SCREEN_HEIGHT - adminHeight) / 2;

    const int loginX = (SCREEN_WIDTH - loginWidth) * 0.15;
    const int loginY = (SCREEN_HEIGHT - loginHeight)  / 2;


    int hovered = 0;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX >= adminX && mouseX <= adminX + adminWidth && mouseY >= adminY && mouseY < adminY + adminHeight) {
        hovered = adminHeight;
        if (mouseLeftDown) {
            currentStage++;
        }
    }
    
    SDL_Rect srcrect1 = { 0, hovered, adminWidth, adminHeight };
    SDL_Rect dstrect1 = { adminX, adminY, adminWidth, adminHeight };

    SDL_Rect srcrect2 = { 0, 0, loginWidth, loginHeight };
    SDL_Rect dstrect2 = { loginX, loginY, loginWidth, loginHeight };

    SDL_Rect lineRect = { SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 8), 2, (SCREEN_HEIGHT / 8) * 6};

    //At 640x480, we want lineRect.h to be equal to 360

    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // Render the loading bar animation
    SDL_RenderCopy(renderer, adminTexture, &srcrect1, &dstrect1); // Render the logo image
    SDL_RenderCopy(renderer, loginTexture, &srcrect2, &dstrect2); // Render the login text image


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // Set color to white for the line
    SDL_RenderFillRect(renderer, &lineRect); // Render the horizontal line
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset color to black

}

Uint32 startTime;
std::vector<App> currentApps;

void renderDesktopStage(SDL_Renderer* renderer, std::vector<App>& appVector, SDL_Texture* backgroundTexture, SDL_Texture* menuTexture) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    
    const int menuWidth  = 88;
    const int menuHeight = 88;

    const int menuX = 0;
    const int menuY = SCREEN_HEIGHT - menuHeight; // Updated calculation for menuY

    int hovered = 0;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX >= menuX && mouseX < menuX + menuWidth && mouseY >= menuY && mouseY < menuY + menuHeight) {
        hovered = menuHeight;
        if (mouseLeftDown) {
            menuEnabled = !menuEnabled;
        }
    }

    if (menuEnabled) {
        // Render the extended bar
        SDL_Rect lineRect2 = {menuHeight, SCREEN_HEIGHT - menuHeight, (appVector.size()) * (menuHeight / 2), menuHeight / 2};
        hovered = menuHeight * 2;

        SDL_SetRenderDrawColor(renderer, 138, 70, 255, 0); // Set color to blue for the extended bar
        SDL_RenderFillRect(renderer, &lineRect2);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset color to black

        // Iterate through the appVector
        int appIndex = 0;
        for (const App& app : appVector) {
            SDL_Rect appRect = {menuWidth + (appIndex * (menuWidth / 2)), 
                                SCREEN_HEIGHT - menuHeight, 
                                menuWidth / 2, 
                                menuHeight / 2};
            
            SDL_RenderCopy(renderer, app.appIcon, NULL, &appRect); // Render the app

            if(mouseX >= appRect.x && mouseX < appRect.x + appRect.w && mouseY >= appRect.y && mouseY < appRect.y + appRect.h && mouseLeftDown) {
                menuEnabled = false;
                app.appFunction();
                if
            }

            appIndex++;
        }
    }

    SDL_Rect srcrect1 = {0, hovered, menuWidth, menuHeight};
    SDL_Rect dstrect1 = {0, SCREEN_HEIGHT - menuHeight, menuWidth, menuHeight};

    SDL_Rect lineRect = {0, SCREEN_HEIGHT - (menuHeight / 2), SCREEN_WIDTH, menuHeight / 2};

    SDL_SetRenderDrawColor(renderer, 13, 13, 58, 0); // Set color to white for the line
    SDL_RenderFillRect(renderer, &lineRect); // Render the horizontal line
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset color to black
    SDL_RenderCopy(renderer, menuTexture, &srcrect1, &dstrect1);

}

int main(int argc, char** argv) {
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("WinDiana 2.0",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    //TODO: Get relative paths working 2023-05-23 20:28:50

    const std::string assetPath = "/home/boreal/Projects/WinDiana2.0/assets/";
    const std::vector<std::string> fileNames = {"loading.png", 
                                                "logo.png", 
                                                "background.png", 
                                                "admin.png", 
                                                "login.png", 
                                                "desktop.png",
                                                "menu.png",
                                                "power.png",
                                                "key.png",
                                                "suphoto.png"};
    std::vector<SDL_Surface*> images(fileNames.size());
    std::vector<SDL_Texture*> textures(fileNames.size());

    for (size_t i = 0; i < fileNames.size(); i++) {
        std::string path = assetPath + fileNames[i];
        images[i] = IMG_Load(path.c_str());
        textures[i] = SDL_CreateTextureFromSurface(renderer, images[i]);
    }

    std::vector<App> apps;

    App powerOff = {"Power Off", textures[7], appShutDown, false};
    App logOff = {"Log Off", textures[8], appLogOff, false};
    App supHoto = {"SupHoto", textures[9], foo, true};

    apps.push_back(supHoto);
    apps.push_back(logOff);
    apps.push_back(powerOff);

    Uint32 startTime = SDL_GetTicks(); // Get the start time

    while (!quit) {
        while (SDL_PollEvent(&event) != NULL) {
            switch(event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseLeftDown = true;
                }
                break;
            }
        }

        SDL_RenderClear(renderer);

        // Render different stages based on the currentStage variable
        switch (currentStage) {
        case 0:
            renderLoadingStage(renderer, textures[0], textures[1], startTime);
            break;
        case 1:
            renderLoginStage(renderer, textures[2], textures[3], textures[4]);
            break;
        case 2:
            renderDesktopStage(renderer, apps, textures[5], textures[6]);
            break;
        }

        mouseLeftDown = false;

        SDL_RenderPresent(renderer);
    }

    appShutDown();
}
