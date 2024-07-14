#include "gd_spc_player.h"
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/file_access.hpp>

using namespace godot;

void GDSpcPlayer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_data"), &GDSpcPlayer::get_data);
	ClassDB::bind_method(D_METHOD("get_sample_rate"), &GDSpcPlayer::get_sample_rate);

	ClassDB::bind_method(D_METHOD("open", "path"), &GDSpcPlayer::open);
}

GDSpcPlayer::GDSpcPlayer() {
	this->snes_spc = NULL;
	this->filter = NULL;
}

GDSpcPlayer::~GDSpcPlayer() {
	if(this->snes_spc) {
		delete this->snes_spc;
	}
	if(this->filter) {
		delete this->filter;
	}
}


Error GDSpcPlayer::open(const String &path) {
	PackedByteArray spcData = FileAccess::get_file_as_bytes(path);
	this->snes_spc = new SNES_SPC;
	this->snes_spc->init();
	this->filter = new SPC_Filter;
	if(snes_spc->load_spc(spcData.ptr(), spcData.size())) {
		return Error::FAILED;
	}

	/* Most SPC files have garbage data in the echo buffer, so clear that */
	this->snes_spc->clear_echo();

	/* Clear filter before playing */
	this->filter->clear();

	return Error::OK;
}

int GDSpcPlayer::get_sample_rate() {
	return SNES_SPC::sample_rate;
}

PackedByteArray GDSpcPlayer::get_data() const {
	int framesPerBuffer = 2048;
	PackedByteArray array;
	array.resize(framesPerBuffer);

	snes_spc->play(framesPerBuffer, (short*)array.ptrw());
	filter->run((short*)array.ptrw(), framesPerBuffer);

	return array;
}
