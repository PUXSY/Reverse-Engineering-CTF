#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_EASY_FONT_IMPLEMENTATION
#include "./Assets/stb_easy_font.h"
#include <winsock2.h>
#include <string.h>
#include <set>
#include <sstream>

struct Color {
    float r, g, b;
};  

struct Vec2f 
{
    float x, y;
};

constexpr int pt = 1;
// 2 ^ 16 = 65536 bits, enough for a large text buffer 
char text_buffer[65536]; 
const Color Black = {0.0f, 0.0f, 0.0f};
const Color White = {1.0f, 1.0f, 1.0f}; 

bool my_first_condition(int argc, const char** argv) {
    if (argc > 1) {
        if(strcmp(argv[1], "0x66757879") == 0) {
            if(argc <= 2) {
                if(pt == 1) {
                    return true; 
                }
            }
        }
    }
    return false; 
}

std::string WINAPI ReadFromAFile(const std::string& filename, size_t maxSize = 1024)
{
    std::string result;

    FILE* fileReader = fopen(filename.c_str(), "r");
    if (!fileReader)
        return {}; // return empty string on failure

    char* buffer = new char[maxSize + 1];
    buffer[maxSize] = '\0';

    size_t bytesRead = fread(buffer, 1, maxSize, fileReader);
    fclose(fileReader);

    result.assign(buffer, bytesRead);
    delete[] buffer;
    return result;
}

bool my_second_condition()
{
    const std::string fileContent = ReadFromAFile("./Permissions.txt", 64);

    if (fileContent.empty())
        return false;

    // Numbers we REQUIRE (exactly these, no more, no less)
    std::set<int> required = {102, 108, 97, 103};
    std::set<int> found;

    std::istringstream stream(fileContent);
    std::string line;

    while (std::getline(stream, line))
    {
        // Remove possible \r (Windows line endings)
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        // Skip empty lines
        if (line.empty())
            continue;

        // Try to parse the line as an integer
        std::istringstream lineStream(line);
        int value;
        
        // Check if we can read an integer and that nothing else follows
        if (lineStream >> value)
        {
            // Check if there's anything else on the line (besides whitespace)
            std::string remainder;
            if (lineStream >> remainder)
            {
                // There's extra content on the line - fail
                return false;
            }
            
            found.insert(value);
        }
        else
        {
            // Line contains non-integer content - fail
            return false;
        }
    }

    // Check if found set exactly matches required set
    return found == required;
}

bool anti_debug_check(){
    if ( IsDebuggerPresent() )
    {
      MessageBoxA(0LL, "Debugger Detected!", "Bye", 0);
      ExitProcess(0);
    }
    return CloseHandle((HANDLE)0xDEADC0DELL);
}

void drawText(const Vec2f& pos2D, const char* text, const Color& color, float scale = 1.0f) {
   // Convert color to unsigned char format
   unsigned char textColor[4] = {
       (unsigned char)(color.r * 255),
       (unsigned char)(color.g * 255),
       (unsigned char)(color.b * 255),
       255
   };
   
   int quads = stb_easy_font_print(0, 0, (char*)text, textColor, text_buffer, sizeof(text_buffer));

   // Get text dimensions for centering
   int text_width = stb_easy_font_width((char*)text);
   int text_height = stb_easy_font_height((char*)text);

   // Save current matrix
   glPushMatrix();
   
   // Move to the desired position
   glTranslatef(pos2D.x, pos2D.y, 0);
   // Scale from center
   glScalef(scale, scale, 1.0f);
   // Offset by half the text size to center it
   glTranslatef(-text_width * 0.5f, -text_height * 0.5f, 0);

   // Enable vertex arrays
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
   
   // Set up vertex pointer for 3D coordinates (x, y, z)
   glVertexPointer(3, GL_FLOAT, 16, text_buffer);
   // Set up color pointer (4 bytes for RGBA, offset by 12 bytes)
   glColorPointer(4, GL_UNSIGNED_BYTE, 16, text_buffer + 12);
   
   glDrawArrays(GL_QUADS, 0, quads * 4);
   
   glDisableClientState(GL_COLOR_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);
   
   // Restore matrix
   glPopMatrix();
}

void successWindow(){
    glClearColor(0.0f, 255.0f, 0.0f, 1.0f); 
    drawText({400, 200}, "Flag", White, 10.0);
}

void mediumWindow(){
    glClearColor(255.0f, 126.0f, 0.0f, 1.0f); 
    drawText({400, 200}, "Almost but not enough", White, 5.0f);
}

void standardWindow(){
    glClearColor(255.0f, 0.0f, 0.0f, 1.0f);
    drawText({400, 200}, "No Flag", White, 10.0);
}

int main(int argc, const char** argv) {
    if (!glfwInit()) return -1;
    anti_debug_check();
    
    // Use compatibility profile for legacy OpenGL functions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 400, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) return -1;

    // Enable blending for text rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Set orthographic projection for 2D drawing
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 800, 400, 0, -1, 1); // Flip Y: (0,0) is top-left
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();


        // Set background and draw text
        if (my_first_condition(argc, argv) == true) {
            if (my_second_condition() == true) {
                successWindow();
            } else {
                mediumWindow();
            }
        } else {
            standardWindow();
        }

        // Restore matrices
        glPopMatrix();           // MODELVIEW
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();           // PROJECTION
        glMatrixMode(GL_MODELVIEW);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// g++ -std=c++11 -mwindows -g -o C:\Users\User\source\Projects\Reverse-Engineering-CTF\src\Utilities\..\CTF_Level11.exe C:\Users\User\source\Projects\Reverse-Engineering-CTF\src\Utilities\..\CTF_Level11.cpp -IC:/vcpkg/installed/x64-mingw-static/include -LC:/vcpkg/installed/x64-mingw-static/lib -lglew32 -lglfw3 -lopengl32 -lgdi32