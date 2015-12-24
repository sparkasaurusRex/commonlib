#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <SDL2/SDL_mixer.h>

namespace Audio
{
  class AudioImp
  {
  public:
    AudioImp() {}
    ~AudioImp() {}

    void init();
    void simulate(const float game_time, const float frame_time);
    void deinit();
  private:

  };
};
#endif //__AUDIO_H__
