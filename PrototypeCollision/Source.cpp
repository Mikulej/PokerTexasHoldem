#include "Source.h"

extern unsigned int texture_box, texture_karty;
extern double mx = 0, my = 0;//mouse x,y position
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

static void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}
//settings
//16:9
extern int SCR_WIDTH = 800;//1280
extern int SCR_HEIGHT = 450;//720
enum { main_menu, options, exit_program, play_state } game_state;

int main()
{

    glfwSetErrorCallback(error_callback);
    ///*  glfw: initialize and configure
    //  ------------------------------*/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    srand((unsigned int)time(NULL));
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Poker Texas Hold'em", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("vs.glsl", "fs.glsl");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices_center[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    float vertices_bottom_left[] = {
        // positions          // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
        0.0f, 0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        0.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO, VBO_text, VAO_text;//zrobic tablice VAO

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_center), vertices_center, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &texture_box);
    glBindTexture(GL_TEXTURE_2D, texture_box);
    //set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("box.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

/////////////////////////TEKSTURA RGBA
    glGenTextures(1, &texture_karty);
    glBindTexture(GL_TEXTURE_2D, texture_karty);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    data = stbi_load("kartypoker.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    //////////////////////////////////////////////
    // Enable blending
    //glEnable(GL_CULL_FACE);//do tekstu?~!!!!!!!!!!!
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // TEXT
    // -----------
     // compile and setup the shader
    // ----------------------------
    Shader textShader("textvs.glsl", "textfs.glsl");
    textShader.use();
    Text::ftInit();
    // INITIALIZE
    // -----------
    glfwSwapInterval(1); // wlacz V-sync
    StaticObject::collisionList.reserve(300); //zeby nie wykonywac automatyczne resize vectora(robi copy constructor)
    main_menu_init();
    Karta::KartaInit();
    Gracz::graczList.push_back(Gracz());
    Gracz::graczList.push_back(Gracz());
    // RENDER LOOP
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
        processInput(window);
        // render
        // ------
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.18f, 0.37f, 0.27f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Image::RenderAll(VAO, ourShader);
        Text::RenderAll(textShader);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

int mouse_state = 0, mouse_old_state = 0;
bool waiting = false; bool waited = false;
time_t start, now;
inline bool mouse_click() {
    return mouse_state == GLFW_PRESS && mouse_old_state == GLFW_RELEASE;
}
int ending_game = 2; // 0 - gre wygral gracz 1 - gre wygral komputer 2 - gra sie toczy 3 - remis
//INTERFACE HANDLERS
inline void main_menu_handler() {
    if (!StaticObject::collisionList.empty()) {
        if (StaticObject::collisionList.at(0).pointed_by_mouse()) {
            if (mouse_click()) {
                std::cout << "Clicked start!" << std::endl;
                //GRAJ
                play_init();
                return;
            }
        }
        if (StaticObject::collisionList.at(1).pointed_by_mouse()) {
            if (mouse_click()) {
                //OPCJE
                options_init();
                return;
            }
        }
        if (StaticObject::collisionList.at(2).pointed_by_mouse()) {
            if (mouse_click()) {
                //WYJDZ
                exit_program_init();
                return;
            }
        }
    }
}
inline void main_menu_init() {
    StaticObject::DelAll(); Text::DelAll();
    StaticObject::AddItem(0, 0, -0.1f);  Text::Add("Start", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 0, true);
    StaticObject::AddItem(0, 0, -0.4f);  Text::Add("Options", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 1, true);
    StaticObject::AddItem(0, 0, -0.7f);  Text::Add("Exit", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 2, true);

    game_state = main_menu;
}
inline void options_init() {
    StaticObject::DelAll(); Text::DelAll();
    StaticObject::AddItem(0, 0, -0.85f);  Text::Add("Back", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 0, true);
    //SELECT CARD REVERSE
    StaticObject::AddItem(10, 0.3f, 0.7f); Text::AddRaw("Card Reverse", -0.3f, 0.7f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
    StaticObject::AddItem(1, 0.5f, 0.7f); Text::Add(">", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 2, true);
    StaticObject::AddItem(1, 0.1f, 0.7f); Text::Add("<", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 3, true);
    //SELECT STARTING CREDITS
    Text::AddRaw("Starting credits", -0.3f, 0.3f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
    StaticObject::AddItem(1, 0.5f, 0.3f); Text::Add(">", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 4, true);
    StaticObject::AddItem(1, 0.1f, 0.3f); Text::Add("<", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 5, true);
    Text::AddRaw(std::to_string(Game::starting_credits), 0.15f, 0.27f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), true);
    //SELECT MIN RAISE
    Text::AddRaw("Minimal raise", -0.3f, 0.0f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
    StaticObject::AddItem(1, 0.5f, 0.0f); Text::Add(">", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 6, true);
    StaticObject::AddItem(1, 0.1f, 0.0f); Text::Add("<", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 7, true);
    Text::AddRaw(std::to_string(Game::minimal_raise), 0.15f, -0.03f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), true);
    //SELECT STARTING DEALER
    Text::AddRaw("Starting dealer", -0.3f, -0.3f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
    StaticObject::AddItem(1, 0.5f, -0.3f); Text::Add(">", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 8, true);
    StaticObject::AddItem(1, 0.1f, -0.3f); Text::Add("<", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 9, true);
    if (Game::dealer_option == 0) Text::AddRaw("Player", 0.3f, -0.3f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
    else if (Game::dealer_option == 1) Text::AddRaw("Enemy", 0.3f, -0.3f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
    else if (Game::dealer_option == 2) Text::AddRaw("Random", 0.3f, -0.3f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
    //SELECT STARTING CREDITS
    Text::AddRaw("Starting credits", -0.3f, -0.6f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
    StaticObject::AddItem(1, 0.5f, -0.6f); Text::Add(">", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 10, true);
    StaticObject::AddItem(1, 0.1f, -0.6f); Text::Add("<", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 11, true);
    Text::AddRaw(std::to_string(Game::starting_credits), 0.15f, -0.635f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), true);
    game_state = options;
}
inline void options_handler() {
    if (StaticObject::collisionList.at(0).pointed_by_mouse()) {
        if (mouse_click()) {//WROC DO MAIN_MENU        
            main_menu_init();
            return;
        }
    }
    //SELECT CARD REVERSE
    if (StaticObject::collisionList.at(2).pointed_by_mouse()) {
        if (mouse_click()) {
            Karta::next_rewers();
            StaticObject::collisionList.at(1).set_offset(Karta::get_rewers(), 0);
        }
    }
    if (StaticObject::collisionList.at(3).pointed_by_mouse()) {
        if (mouse_click()) {
            Karta::prev_rewers();
            StaticObject::collisionList.at(1).set_offset(Karta::get_rewers(), 0);
        }
    }
    //SELECT STARTING CREDITS
    if (StaticObject::collisionList.at(4).pointed_by_mouse()) {
        if (mouse_click()) {
            Game::starting_credits += 500;
            if (Game::starting_credits > 10000) { Game::starting_credits = 500; }
            options_init(); return;
        }
    }
    if (StaticObject::collisionList.at(5).pointed_by_mouse()) {
        if (mouse_click()) {
            Game::starting_credits -= 500;
            if (Game::starting_credits < 500) { Game::starting_credits = 10000; }
            options_init(); return;
        }
    }
    //SELECT MIN RAISE
    if (StaticObject::collisionList.at(6).pointed_by_mouse()) {
        if (mouse_click()) {
            Game::minimal_raise += 100;
            if (Game::minimal_raise > 1000) { Game::minimal_raise = 100; }
            options_init(); return;
        }
    }
    if (StaticObject::collisionList.at(7).pointed_by_mouse()) {
        if (mouse_click()) {
            Game::minimal_raise -= 100;
            if (Game::minimal_raise < 100) { Game::minimal_raise = 1000; }
            options_init(); return;
        }
    }
    //SELECT STARTING DEALER
    if (StaticObject::collisionList.at(8).pointed_by_mouse()) {
        if (mouse_click()) {
            Game::dealer_option++;
            if (Game::dealer_option > 2) { Game::dealer_option = 0; }
            options_init(); return;
        }
    }
    if (StaticObject::collisionList.at(9).pointed_by_mouse()) {
        if (mouse_click()) {
            Game::dealer_option--;
            if (Game::dealer_option < 0) { Game::dealer_option = 2; }
            options_init(); return;
        }
    }
    //SELECT SMALL BIND?
    if (StaticObject::collisionList.at(10).pointed_by_mouse()) {
        if (mouse_click()) { //options_init() placeholder

        }
    }
    if (StaticObject::collisionList.at(11).pointed_by_mouse()) {
        if (mouse_click()) { //options_init() placeholder

        }
    }
}
#define WAIT_TIME 3 //czas myslenia przeciwnika
inline void play_handler() {
    if (Game::whowins != 2) {//KONIEC RUNDY
        if (mouse_click()) {            
            if (ending_game != 2) { //KONIEC GRY
                Game::end_game(ending_game);
                main_menu_init();
                return;
            }
            Game::whowins = 2;
            play_init();
            return;
        }
    }
    else { //GRA
        if (Game::enemy_turn) {//RUCH KOMPUTERA
            if (waiting) {
                time(&now);
                //std::cout << difftime(now, start) << std::endl;
                if (difftime(now, start) >= WAIT_TIME) { waited = true; waiting = false; }
                else { return; }
            }
            if (!waited) { time(&start); waiting = true; Game::enemy_desc = "Enemy thinks."; play_init(); return; }
            else {
                Gracz::graczList[1].bot_action();
                Game::enemy_turn = false;
                waited = false;
                play_init(); return;
            }
        }
        else {//RUCHA GRACZA
            if (StaticObject::collisionList.at(0).pointed_by_mouse()) {
                if (mouse_click()) { //raise confirm
                    Gracz::graczList[0].raise(Game::raise);
                    Game::enemy_turn = true;
                    play_init();
                    return;
                }
            }
            if (StaticObject::collisionList.at(1).pointed_by_mouse()) {
                if (mouse_click()) { //bigger raise 
                    Game::raise += 100;
                    if (Game::raise > Gracz::graczList[0].get_credits()) {
                        Game::raise = Game::minimal_raise;
                    }
                    play_init(); return;
                }
            }
            if (StaticObject::collisionList.at(2).pointed_by_mouse()) {
                if (mouse_click()) {//smaller raise
                    Game::raise -= 100;
                    if (Game::raise < Game::minimal_raise) {
                        Game::raise = Gracz::graczList[0].get_credits();
                    }
                    play_init(); return;
                }
            }
            if (StaticObject::collisionList.at(3).pointed_by_mouse()) {
                if (mouse_click()) { //call 
                    Gracz::graczList[0].call(0);
                    Game::enemy_turn = true;
                    play_init(); return;
                }
            }
            if (StaticObject::collisionList.at(4).pointed_by_mouse()) {
                if (mouse_click()) {//fold    
                    Gracz::graczList[0].fold(0);
                    play_init(); return;
                }
            }
        }
    }
    
}
inline void play_init() {
    StaticObject::DelAll(); Text::DelAll();
    game_state = play_state;
    //Game::Initialize()?
    if (!Game::game_started) {
        Gracz::graczList[0].reka.clear(); Gracz::graczList[1].reka.clear(); Game::talia.clear(); Game::stol.clear();
        if (Game::first_round) {
             Gracz::graczList[0].add_credits(Game::starting_credits - 3800);//testowo te -3800
             Gracz::graczList[1].add_credits(Game::starting_credits);
             if (Game::dealer_option == 2) { Game::whos_dealer = rand() % 2; }
             else { Game::whos_dealer = Game::dealer_option;  Game::enemy_turn = Game::whos_dealer; }
            Game::first_round = false;
        }
        else{
            Game::enemy_turn = Game::whos_dealer = 1 - Game::whos_dealer;
        }
        Gracz::graczList[0].checks = false;
        Gracz::graczList[1].checks = false;
        Game::stworz_talie(Game::talia);
        Gracz::graczList[0].wez_karty_z_talii(); 
        Gracz::graczList[1].wez_karty_z_talii(); 
        Game::stworz_stol();       
        Game::blinds(Game::whos_dealer);
        Game::raise = Game::minimal_raise;
        Game::checked_cards = 0;
        Game::game_started = true;
    }
    else {
        Game::won_prize = 0;
    }
    //NO CREDITS CONDITION
    if (Gracz::graczList[0].get_credits() == 0 || Gracz::graczList[1].get_credits() == 0) {
        Game::checked_cards = 4;
        Gracz::graczList[0].checks = false;
        Gracz::graczList[1].checks = false;
    }
    else if (Gracz::graczList[0].checks && Gracz::graczList[1].checks) {//CHECK CONDITION
        //UWAGA uwzglednij sytuacje gdy robimy all in!
            Game::checked_cards++;
            Gracz::graczList[0].checks = false;
            Gracz::graczList[1].checks = false;
    }

    if (Game::checked_cards == 4) {
        //wybor wygrywajacego
        Gracz::graczList[0].calculate_power();
        Gracz::graczList[1].calculate_power();
        int power0 = Gracz::graczList[0].get_power();
        int power1 = Gracz::graczList[1].get_power();
        int sub_power0 = Gracz::graczList[0].get_sub_power();
        int sub_power1 = Gracz::graczList[1].get_sub_power();
        if ((power0 > power1) || (power0 == power1 && sub_power0 > sub_power1)) {
            Game::end_round(0);
        }
        else if ((power1 > power0) || (power0 == power1 && sub_power1 > sub_power0)) {
            Game::end_round(1);
        }
        else {//remis
            Game::end_round(2);
        }
        
    }

    //BUTTONS
    if(Game::raise == Gracz::graczList[0].get_credits()){ StaticObject::AddItem(2, 0.0f, -0.7f);  Text::Add("All in", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 0, true); }
    else { StaticObject::AddItem(2, 0.0f, -0.7f);  Text::Add("Raise", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 0, true); }
    StaticObject::AddItem(1, 0.2f, -0.9f);  Text::Add(">", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 1, true);
    StaticObject::AddItem(1, -0.2f, -0.9f);  Text::Add("<", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 2, true);
    if (Gracz::graczList[0].gave_to_pool >= Gracz::graczList[1].gave_to_pool) { StaticObject::AddItem(2, 0.4f, -0.7f);  Text::Add("Check", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 3, true); }
    else { StaticObject::AddItem(2, 0.4f, -0.7f);  Text::Add("Call", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 3, true); }
    StaticObject::AddItem(2, 0.8f, -0.7f);  Text::Add("Fold", 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), 4, true);
    //PLAYERS HAND
    StaticObject::AddItem(11, -0.5f, -0.7f, Gracz::graczList[0].reka[0].get_numer(), Gracz::graczList[0].reka[0].get_kolor());//5
    StaticObject::AddItem(11, -0.8f, -0.7f, Gracz::graczList[0].reka[1].get_numer(), Gracz::graczList[0].reka[1].get_kolor());//6
    //TEXT & UI
    switch (Game::whowins) {
    case 0:
        if (ending_game == 1) {
            Text::AddRaw("Victory", 0.75f, 0.1f, 0.001f, 0.001f, glm::vec3(0.56f, 0.93f, 0.56f));
            Text::AddRaw("(Click to return to menu)", 0.775f, 0.0f, 0.00045f, 0.00045f, glm::vec3(0.8f, 0.8f, 0.8f));
        }
        else if (ending_game == 2) {
            Text::AddRaw("You Win!", 0.75f, 0.2f, 0.00075f, 0.00075f, glm::vec3(0.56f, 0.93f, 0.56f));
            Text::AddRaw(std::to_string(Game::won_prize), 0.6f, 0.0f, 0.001f, 0.001f, glm::vec3(0.56f, 0.93f, 0.56f), true);
            Text::AddRaw("(Click to continue)", 0.75f, -0.1f, 0.0005f, 0.0005f, glm::vec3(0.8f, 0.8f, 0.8f));
        }
        break;
    case 1:      
        if (ending_game == 1) {
            Text::AddRaw("Defeat", 0.75f, 0.1f, 0.001f, 0.001f, glm::vec3(1.0f, 0.447f, 0.463f));
            Text::AddRaw("(Click to return to menu)", 0.775f , 0.0f, 0.00045f, 0.00045f, glm::vec3(0.8f, 0.8f, 0.8f));
        }
        else if (ending_game == 2) {
            Text::AddRaw("Enemy Wins!", 0.75f, 0.2f, 0.00075f, 0.00075f, glm::vec3(1.0f, 0.447f, 0.463f));
            Text::AddRaw(std::to_string(Game::won_prize), 0.6f, 0.0f, 0.001f, 0.001f, glm::vec3(1.0f, 0.447f, 0.463f), true);
            Text::AddRaw("(Click to continue)", 0.75f, -0.1f, 0.0005f, 0.0005f, glm::vec3(0.8f, 0.8f, 0.8f));
        }     
        break;
    case 2:
        Text::AddRaw("Pool:", 0.75f, 0.2f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
        Text::AddRaw(std::to_string(Game::get_pool()), 0.6f, 0.0f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), true);
        break;
    case 3:
        Text::AddRaw("Draw!", 0.75f, 0.2f, 0.00075f, 0.00075f, glm::vec3(0.8f, 0.8f, 0.8f));
        Text::AddRaw("(Click to continue)", 0.75f, -0.1f, 0.0005f, 0.0005f, glm::vec3(0.8f, 0.8f, 0.8f));
        break;
    }

    
    Text::AddRaw("Your credits:", 0.2f, -0.5f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
    Text::AddRaw(std::to_string(Gracz::graczList[0].get_credits()) + "(" + std::to_string(Gracz::graczList[0].gave_to_pool) + ")", 0.5f, -0.53f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), true);
    Text::AddRaw("Enemy credits:", 0.2f, 0.5f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f));
    Text::AddRaw(std::to_string(Gracz::graczList[1].get_credits())+ "(" + std::to_string(Gracz::graczList[1].gave_to_pool) + ")", 0.5f, 0.47f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), true);
    Text::AddRaw(Game::enemy_desc, -0.3f, 0.8f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), true); 
    Text::AddRaw(std::to_string(Game::raise), -0.15f, -0.93f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), true); 
    Text::AddRaw("Dealer:", 0.5f, -0.9f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f)); 
    if (Game::whos_dealer == 0) Text::AddRaw("You", 0.65f, -0.93f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), true);
    else if (Game::whos_dealer == 1)  Text::AddRaw("Enemy", 0.65f, -0.93f, 0.001f, 0.001f, glm::vec3(0.8f, 0.8f, 0.8f), true);

    //KARTA NR1
    if (Game::checked_cards == 0) { StaticObject::AddItem(10, -0.8f, 0.0f); }
    else{ StaticObject::AddItem(11, -0.8f, 0.0f,Game::stol[0].get_numer(), Game::stol[0].get_kolor()); }
    //KARTA NR2
    if (Game::checked_cards == 0) { StaticObject::AddItem(10, -0.5f, 0.0f); }
    else { StaticObject::AddItem(11, -0.5f, 0.0f, Game::stol[1].get_numer(), Game::stol[1].get_kolor()); }
    //KARTA NR3
    if (Game::checked_cards == 0) { StaticObject::AddItem(10, -0.2f, 0.0f); }
    else { StaticObject::AddItem(11, -0.2f, 0.0f, Game::stol[2].get_numer(), Game::stol[2].get_kolor()); }
    //KARTA NR4  
    if (Game::checked_cards <= 1) { StaticObject::AddItem(10, 0.1f, 0.0f); }
    else { StaticObject::AddItem(11, 0.1f, 0.0f, Game::stol[3].get_numer(), Game::stol[3].get_kolor()); }
    //KARTA NR5  
    if (Game::checked_cards <= 2) { StaticObject::AddItem(10, 0.4f, 0.0f); }
    else { StaticObject::AddItem(11, 0.4f, 0.0f, Game::stol[4].get_numer(), Game::stol[4].get_kolor()); }
    //ENDGAME & ENEMY HAND
    if (Game::checked_cards == 4) {
        StaticObject::AddItem(11, -0.5f, 0.7f, Gracz::graczList[1].reka[0].get_numer(), Gracz::graczList[1].reka[0].get_kolor());
        StaticObject::AddItem(11, -0.8f, 0.7f, Gracz::graczList[1].reka[1].get_numer(), Gracz::graczList[1].reka[1].get_kolor());
    }
    else {
        StaticObject::AddItem(10, -0.5f, 0.7f);
        StaticObject::AddItem(10, -0.8f, 0.7f);
    }
   
}
inline void exit_program_init() {
    game_state = exit_program;
}
void processInput(GLFWwindow* window)
{
    //MOUSE 
    glfwGetCursorPos(window, &mx, &my);
    //std::cout << "Before X: " << mx << std::endl;
    mx = (mx / (double)SCR_WIDTH * 2) - 1;
    my = 1 - my / (double)SCR_HEIGHT * 2;
    //std::cout << "X: " << mx << " Y: " << my << std::endl;
    //std::cout << "After X: "<< (double)(mx + 1) * (double)SCR_WIDTH / (double)2 << std::endl;
    mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    //SCREEN STATE HANDLER
    switch (game_state) {
    case main_menu:
        main_menu_handler();
        break;
    case options:
        options_handler();
        break;
    case exit_program:
        glfwSetWindowShouldClose(window, true);
        break;
    case play_state:
        play_handler();
        break;
    }
    //testy
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        SCR_WIDTH = 800; SCR_HEIGHT = 450;
        glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        Gracz::next_move = 0;//random
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        Gracz::next_move = 1;//raise
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        Gracz::next_move = 2;//call
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        Gracz::next_move = 3;//fold
    }
    //OLD STATES
    mouse_old_state = mouse_state;
}

/*glfw: whenever the window size changed (by OS or user resize) this callback function executes
---------------------------------------------------------------------------------------------*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}