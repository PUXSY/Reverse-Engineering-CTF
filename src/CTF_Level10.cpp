#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_EASY_FONT_IMPLEMENTATION
#include "./Assets/stb_easy_font.h"
#include <windows.h>


struct Color {
    float r, g, b;
};  

struct Vec2f 
{
    float x, y;
};

inline constexpr int pt = 0;
// 2 ^ 16 = 65536 bits, enough for a large text buffer 
char text_buffer[65536]; 
const Color Black = {0.0f, 0.0f, 0.0f};
const Color White = {1.0f, 1.0f, 1.0f};

bool my_condition(int argc, const char** argv) {
    if (argc > 1) {
        if(argv[1] == "0x66757879") {
            if(argc <= 2) {
                if(pt == 1) {
                    return true; 
                }
            }
        }
    }
    return false; 
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

void BgGreenColor(){
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f); 
    drawText({400, 200}, "Flag", White, 10.0);
}

void BgRedColor(){
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); 
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
        if (my_condition(argc, argv)) {
            BgGreenColor(); 
        } else {
            BgRedColor();
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