#ifndef __SDL_FILES_INCLUDED__
  #define __SDL_FILES_INCLUDED__

  #ifdef __unix__
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_mixer.h>
    
    #elif defined (_WIN32) || defined (WIN32)
      #include <SDL.h>
      #include <SDL_image.h>
      #include <SDL_ttf.h>
      #include <SDL_mixer.h>
    #endif

#endif