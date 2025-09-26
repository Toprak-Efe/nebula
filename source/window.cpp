#include <window.hpp>

using str = const char *;

void nebula::windows::SDLWindow::initialize() {
    logger.log<INFO>("Initializing SDL.");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logger.log<ERROR>("Failed to initialize SDL: {}", SDL_GetError());
        exit(1);
    }

    logger.log<INFO>("Platform: {}", SDL_GetPlatform());
    logger.log<INFO>("CPU count: {:d}", SDL_GetCPUCount());
    logger.log<INFO>("RAM: {:d} MB", SDL_GetSystemRAM());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
    
    window = SDL_CreateWindow(
        "Nebula",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1080, 720, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!window) {
        logger.log<ERROR>("Failed to create window: {}", SDL_GetError());
        exit(1);
    } else {
        logger.log<INFO>("Window successfully created.");
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        logger.log<ERROR>("Failed to create OpenGL context: {}", SDL_GetError());
        exit(1);
    } else {
        logger.log<INFO>("OpenGL context successfully created.");
    }

    SDL_GL_MakeCurrent(window, context);
    if (!SDL_GL_SetSwapInterval(-1)) {
        SDL_GL_SetSwapInterval(1);
    }
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      
    
    ImGui::StyleColorsDark();

    // Setup scaling
    //ImGuiStyle& style = ImGui::GetStyle();
    //style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    //style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();
    
    logger.log<INFO>("OpenGL Vendor: {}", (str) glGetString(GL_VENDOR));
    logger.log<INFO>("OpenGL Version: {}", (str) glGetString(GL_VERSION));
    logger.log<INFO>("OpenGL Renderer: {}", (str) glGetString(GL_RENDERER));
    logger.log<INFO>("OpenGL Shading Language Version: {}", (str) glGetString(GL_SHADING_LANGUAGE_VERSION));

    if (!opengl_load_functions()) {
        logger.log<ERROR>("Failed to load OpenGL functions.");
        exit(1);
    }

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    
}

nebula::windows::SDLWindow::~SDLWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    if (context) {
        SDL_GL_DeleteContext(context);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void nebula::windows::SDLWindow::draw() {
}

