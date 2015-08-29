#ifndef __FLUID_CHANNELS_H__
#define __FLUID_CHANNELS_H__

enum FluidChannelType
{
	FLUID_CHANNEL_VEL_X,
	FLUID_CHANNEL_VEL_Y,
	FLUID_CHANNEL_DENS_R,
	FLUID_CHANNEL_DENS_G,
	FLUID_CHANNEL_DENS_B,
	NUM_FLUID_CHANNELS
};

template<typename T, size_t N>
class FluidVector
{
	public:
		FluidVector()
		{
			//data = new T[N];
			for(int i = 0; i < N; i++)
			{
				data[i] = 0.0f;
			}
		}

		~FluidVector() { }
	//private:
		T data[N];
};

typedef FluidVector<float, NUM_FLUID_CHANNELS> FluidChannels;

#endif //__FLUID_CHANNELS_H__
