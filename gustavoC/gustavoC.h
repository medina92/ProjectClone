#ifndef THIS_FMOD
#define THIS_FMOD

#include "api/inc/fmod.hpp"
#include "api/inc/fmod_errors.h"
#include <stdio.h>

#define MAX_SOUNDS 32
#define BGM 0


extern bool fmod_ok(FMOD_RESULT res);

extern bool fmod_init();

extern bool fmod_createsound(const char *fname, int i);

extern bool fmod_playsound(int i);

extern bool fmod_setmode(int i, int mode);

extern bool fmod_getlength(int i, unsigned int *lenms);

extern bool fmod_systemupdate();

extern bool fmod_getchannelsplaying(int *channelsplaying);

extern bool fmod_cleanup();

/*
class Sound
{
    private:
        int count;
        FMOD::System *sys;
        FMOD::Sound *sound[MAX_SOUNDS];
        FMOD::Channel *channel[MAX_SOUNDS];
        bool playsound(int);
        bool createsound(const char *,int);
    public:
        Sound();
        ~Sound();
        bool isOk(FMOD_RESULT);
        bool setmode(int,int);
        void update();
        bool getlength(int, unsigned int *);
        void getchannelsplaying(int *);
        bool playbgm(const char *);
        bool playsfx(const char *);
};
*/
#endif
