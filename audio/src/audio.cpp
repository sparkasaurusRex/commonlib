#include <iostream>
#include <string>
#include <assert.h>
#include "audio.h"

using namespace Audio;
using namespace std;

void AudioImp::init()
{
  int freq, channels;
  Uint16 format;

  Mix_QuerySpec(&freq, &format, &channels);
  cout<<"Audio Hardware Support:"<<endl;
  cout<<"\tOutput Frequency: "<<freq<<"Hz"<<endl;
  cout<<"\tFormat: ";
  switch(format) {
    case AUDIO_U8: cout<<"U8"; break;
    case AUDIO_S8: cout<<"S8"; break;
    case AUDIO_U16LSB: cout<<"U16LSB"; break;
    case AUDIO_S16LSB: cout<<"S16LSB"; break;
    case AUDIO_U16MSB: cout<<"U16MSB"; break;
    case AUDIO_S16MSB: cout<<"S16MSB"; break;
  }
  cout<<endl;
  cout<<"\tChannels: "<<channels<<endl;

  Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG);
  int res = Mix_OpenAudio(44100/*MIX_DEFAULT_FREQUENCY*/, MIX_DEFAULT_FORMAT, 2, 4096);//1024);
  if(res != 0)
  {
    valid = false;
    cout<<"Mix_Init(): "<<Mix_GetError()<<endl;
  }
  else
  {
    valid = true;
  }
}

void AudioImp::simulate(const float game_time, const float frame_time)
{

}

void AudioImp::deinit()
{
  Mix_Quit();
}

unsigned int AudioImp::load_music(string fname)
{
  if(!valid)
  {
    cout<<"AudioImp::load_sample(): Audio Context invalid!"<<endl;
    return -1;
  }
  Mix_Music *music;
  cout<<"loading music file "<<fname.c_str()<<endl;
  music = Mix_LoadMUS(fname.c_str());
  if(music)
  {
    music_tracks.push_back(music);
    return (unsigned int)music_tracks.size() - 1;
  }
  cout<<"Mix_LoadMus: "<<Mix_GetError()<<endl;
  return -1;
}

unsigned int AudioImp::load_sample(string fname)
{
  if(!valid)
  {
    cout<<"AudioImp::load_sample(): Audio Context invalid!"<<endl;
    return -1;
  }

  Mix_Chunk *sample;
  cout<<"loading "<<fname.c_str()<<endl;
  sample = Mix_LoadWAV(fname.c_str());
  if(sample)
  {
    samples.push_back(sample);
    return (unsigned int)samples.size() - 1;
  }

  cout<<"Mix_LoadWAV: "<<Mix_GetError()<<endl;
  return -1;
}

void AudioImp::play_sample(unsigned int sample_id, int num_loops)
{
  if(!valid)
  {
    cout<<"AudioImp::load_sample(): Audio Context invalid!"<<endl;
    return;
  }
  assert(sample_id >= 0 && sample_id < samples.size());
  Mix_Chunk *sample = samples[sample_id];
  Mix_PlayChannel(-1, sample, num_loops);
}

void AudioImp::play_music(unsigned int music_id, int num_loops)
{
  if(!valid)
  {
    cout<<"AudioImp::load_sample(): Audio Context invalid!"<<endl;
    return;
  }
  assert(music_id >= 0 && music_id < music_tracks.size());
  Mix_Music *music = music_tracks[music_id];
  Mix_PlayMusic(music, num_loops);
}
