//  enter user name: "TEST / ADMIN"
// user level test: 10
// user level admin: 12
// corect level: 15 - hexadecimal F .

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

#define STB_EASY_FONT_IMPLEMENTATION
#include "./Assets/stb_easy_font.h"
#include "./Assets/sha256/sha256.h"

template <typename T> struct Vec2d
{
    T x, y;
};

struct Color
{
    float r, g, b;
};

const Color Black = {0.0f, 0.0f, 0.0f};
const Color White = {255.0f, 255.0f, 255.0f}; 

const unsigned char adminHash[SHA256_DIGEST_SIZE] = {
    0x5e,0x88,0x41,0x1d,0x62,0x9b,0x93,0x4c,0xaa,0x7c,0xa9,0x0b,0xad,0x98,0xfa,0x6b,
    0xd4,0x2f,0xb0,0xd6,0x15,0xd6,0xa3,0xb8,0xf8,0xf4,0xea,0xaf,0xca,0xe1,0x24,0x67
};


class User {
    private:
        std::string username;
        int level;
        unsigned char password[SHA256_DIGEST_SIZE];

    public:
        User(const std::string& name, int lvl) : username(name), level(lvl) {}

        bool corectLevel() const {
            return level == 15; // Check if the user level is 15 (0xF in hexadecimal)
        }
        
        void setPassword(const unsigned char* pwd) {
            memcpy(password, pwd, SHA256_DIGEST_SIZE);
        }

        bool checkPassword(const std::string& input) const {
            uint8_t inputHash[SHA256_DIGEST_SIZE];
            SHA256((const uint8_t*)input.c_str(), input.length(), inputHash);
            return memcmp(inputHash, password, SHA256_DIGEST_SIZE) == 0;
        }


        void displayInfo() const {
            if (level > 15){
                MessageBoxA(NULL, "Access Denied: Level too high!", "Error", 0);
            } else if (level <= 0){
                MessageBoxA(NULL, "Access Denied: Level too low!", "Error", 0);
            } 
            
            else {
                if (corectLevel()) {
                    MessageBoxA(NULL, "Access Granted: Welcome!", "Success", 0);
                } 
                else {
                    MessageBoxA(NULL, "Access Denied: Incorrect level!", "Error", 0);
                }
            }
            MessageBoxA(NULL, ("User: " + username + " Level: " + std::to_string(level)).c_str(), "User Info", 0);
            if (username == "ADMIN") {
                MessageBoxA(NULL, "Hint: Is the SHA-256 corect?", "Hint", 0);
            }
        }

    ~User() = default;


};

class GLMessageBox
{
public:
    GLMessageBox(const std::string& title, const std::string& message)
        : m_title(title), m_message(message) {}

    ~GLMessageBox()
    {
        Cleanup();
        return;
    }

    std::string Show()
    {
        if (!Init())
            return "";

        Loop();
        Cleanup();
        return m_result;
    }

private:
    GLFWwindow* m_window = nullptr;
    std::string m_title;
    std::string m_message;
    std::string m_input;
    std::string m_result;

    const Vec2d<int> m_size = {420, 200};

    bool m_done = false;
    bool m_ok = false;

    // ---------------- INIT ----------------
    bool Init()
    {
        if (!glfwInit())
            return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_window = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), nullptr, nullptr);
        if (!m_window)
            return false;

        glfwMakeContextCurrent(m_window);

        if (glewInit() != GLEW_OK)
            return false;

        glfwSetWindowUserPointer(m_window, this);
        glfwSetCharCallback(m_window, CharCallback);
        glfwSetKeyCallback(m_window, KeyCallback);
        glfwSetMouseButtonCallback(m_window, MouseCallback);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_TEXTURE_2D);

        return true;
    }

    // ---------------- INPUT ----------------
    static void CharCallback(GLFWwindow* win, unsigned int c)
    {
        auto* self = (GLMessageBox*)glfwGetWindowUserPointer(win);
        if (c >= 32 && c <= 126)
            self->m_input.push_back((char)c);
    }

    static void KeyCallback(GLFWwindow* win, int key, int, int action, int)
    {
        auto* self = (GLMessageBox*)glfwGetWindowUserPointer(win);
        if (action != GLFW_PRESS) return;

        if (key == GLFW_KEY_BACKSPACE && !self->m_input.empty())
            self->m_input.pop_back();

        if (key == GLFW_KEY_ENTER)
        {
            self->m_ok = true;
            self->m_done = true;
        }

        if (key == GLFW_KEY_ESCAPE)
            self->m_done = true;
    }

    static void MouseCallback(GLFWwindow* win, int button, int action, int)
    {
        if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
            return;

        auto* self = (GLMessageBox*)glfwGetWindowUserPointer(win);

        double x, y;
        glfwGetCursorPos(win, &x, &y);

        // OK button
        if (x >= 220 && x <= 300 && y >= 120 && y <= 148)
        {
            self->m_ok = true;
            self->m_done = true;
        }

        // Cancel button
        if (x >= 310 && x <= 390 && y >= 120 && y <= 148)
        {
            self->m_done = true;
        }
    }

    // ---------------- DRAW ----------------
    void Begin2D()
    {
        int w, h;
        glfwGetFramebufferSize(m_window, &w, &h);

        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 420, 200, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void Rect(float x, float y, float w, float h, float r, float g, float b)
    {
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
    }

    void Text(float x, float y, const std::string& txt, float scale = 2.0f)
    {
        char buf[65536]; // 2 ^ 16 bits, enough for a large text buffer

        int quads = stb_easy_font_print(
            0.0f, 0.0f,                 // draw at origin (we transform instead)
            (char*)txt.c_str(),
            nullptr,
            buf,
            sizeof(buf)
        );

        glColor3f(0, 0, 0);

        glPushMatrix();

        // Move to desired position
        glTranslatef(x, y, 0.0f);

        // Scale text uniformly
        glScalef(scale, scale, 1.0f);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 16, buf);
        glDrawArrays(GL_QUADS, 0, quads * 4);
        glDisableClientState(GL_VERTEX_ARRAY);

        glPopMatrix();
    }

    // ---------------- LOOP ----------------
    void Loop()
    {
        while (!glfwWindowShouldClose(m_window) && !m_done)
        {
            Begin2D();

            glClearColor(0.94f, 0.94f, 0.94f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            Text(20, 40, m_message);

            Rect(20, 70, 380, 28, 1, 1, 1);
            Text(26, 90, m_input + "_");

            Rect(220, 120, 80, 28, 0.88f, 0.88f, 0.88f);
            Text(250, 140, "OK");

            Rect(310, 120, 80, 28, 0.88f, 0.88f, 0.88f);
            Text(325, 140, "Cancel");

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }

        if (m_ok)
            m_result = m_input;
    }

    // ---------------- CLEANUP ----------------
    void Cleanup()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
};

User admin("ADMIN", 0xF);
User test("TEST", 0xC);

void DebugMode(std::string username) {
    MessageBoxA(NULL, "Debug mode detected! Access Denied!", "Error", 0);
    if (username == "ADMIN") {
        admin.displayInfo();
    } else {
        test.displayInfo();
    }
}

int main(int argc, const char** argv)
{


    GLMessageBox usernameBox("Info", "Enter username:");
    std::string username = usernameBox.Show();

    if (username.empty()) return 0;
    if (username != "ADMIN" && username != "TEST") {
        MessageBoxA(NULL, "Access Denied: Invalid username!", "Error", 0);
        return 0;
    }

    // if run in /d do this 
    if (argc > 1 && (strcmp(argv[1], "/d") == 0 || strcmp(argv[1], "-d") == 0) ) {
        DebugMode(username);
        return 0;
    }
    
    if (username == "TEST") {
        test.displayInfo();
        return 0;
    }

    admin.setPassword(adminHash);
    GLMessageBox passwordBox("Info", "Enter password:");
    std::string password = passwordBox.Show();

    if (username.empty()) return 0;
    if (admin.checkPassword(password)) {
        MessageBoxA(NULL, "Access Granted!", "Success", 0);
    } else {
        MessageBoxA(NULL, "Access Denied: Invalid password!", "Error", 0);
    }

    return 0;
};

// cd "c:\Users\User\source\Projects\Reverse-Engineering-CTF\src" && gcc -c ./Assets/sha256/sha256.c -o sha256.o && g++ -std=c++11 -g CTF_Level12.cpp sha256.o -o CTF_Level12.exe  -IC:/vcpkg/installed/x64-mingw-static/include -LC:/vcpkg/installed/x64-mingw-static/lib -lglew32 -lglfw3 -lopengl32 -lgdi32 -static