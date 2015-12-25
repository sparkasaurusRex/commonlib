#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>

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

    unsigned int load_sample(std::string fname);
    unsigned int load_music(std::string fname);

    void play_sample(unsigned int id, int num_loops);
    void play_music(unsigned int id, int num_loops);
  private:
    std::vector<Mix_Chunk *> samples; //unordered_map instead?
    std::vector<Mix_Music *> music_tracks;
  };
};
#endif //__AUDIO_H__
