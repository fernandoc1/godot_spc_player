#ifndef __GD_SPC_PLAYER_H__
#define __GD_SPC_PLAYER_H__

#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_generator.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_playback.hpp>

#include "snes_spc/SNES_SPC.h"
#include "snes_spc/SPC_Filter.h"

namespace godot {

class GDSpcPlayer : public Resource {
	GDCLASS(GDSpcPlayer, Resource)

private:
	SNES_SPC* snes_spc;
	SPC_Filter* filter;

	void fillBuffer();

protected:
	static void _bind_methods();

public:
	GDSpcPlayer();
	~GDSpcPlayer();

	Error open(const String &path);
	int get_sample_rate();
	PackedVector2Array get_data(int framesPerBuffer) const;
};

}

#endif
