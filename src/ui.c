#define FPL_IMPLEMENTATION
#include "final_platform_layer.h"

int main(int argc, char **args) {
  // Create default settings
  fplSettings settings = fplMakeDefaultSettings();

  // Overwrite the video backend
  settings.video.backend = fplVideoBackendType_OpenGL;

  
  // Legacy OpenGL
  // settings.video.graphics.opengl.compabilityFlags =
  // fplOpenGLCompabilityFlags_Legacy;

  // or

  // Modern OpenGL
  settings.video.graphics.opengl.compabilityFlags = fplOpenGLCompabilityFlags_Core;
  settings.video.graphics.opengl.majorVersion = 3;
  settings.video.graphics.opengl.minorVersion = 3;

  if (fplPlatformInit(fplInitFlags_Video, &settings)) {
    // Event/Main loop
    while (fplWindowUpdate()) {
      // Poll events
      fplEvent ev;
      while (fplPollEvent(&ev)) {
        /// ...
        if(ev.keyboard.keyCode > 7 && ev.keyboard.keyCode < 255) {
        	printf("%li\n", ev.keyboard.keyCode);
       	}
      }

      // your code goes here

      fplVideoFlip();
    }
    fplPlatformRelease();
    return 0;
  } else {
    return -1;
  }
}