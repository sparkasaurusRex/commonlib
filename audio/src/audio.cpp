#include "audio.h"

using namespace Audio;

void AudioImp::init()
{
  Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG);
}

void AudioImp::simulate(const float game_time, const float frame_time)
{

}

void AudioImp::deinit()
{
  Mix_Quit();
}
