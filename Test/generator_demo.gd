extends Node

var sample_hz = 22050.0 # Keep the number of samples to mix low, GDScript is not super fast.
var pulse_hz = 440.0
var phase = 0.0

var playback: AudioStreamPlayback = null # Actual playback stream, assigned in _ready().
var spc_player: GDSpcPlayer = GDSpcPlayer.new()

func _fill_buffer():
	var to_fill = playback.get_frames_available()
	var array = spc_player.get_data(to_fill)
	for i in array.size() / 2:
		#var vec = Vector2(array.decode_u16(i*2), i);
		#playback.push_frame(vec)
		playback.push_frame(Vector2.ONE * array.decode_s16(i*2))  # Audio frames are stereo.
		#playback.push_frame(Vector2.ONE * array.decode_s16(i*2))  # Audio frames are stereo.
		#playback.push_frame(Vector2.ONE * (i / 65535))  # Audio frames are stereo.

func _process(_delta):
	_fill_buffer()


func _ready():
	spc_player.open("res://Audio/test.spc");
	# Setting mix rate is only possible before play().
	$Player.stream.mix_rate = spc_player.get_sample_rate() * 2;
	$Player.play()
	playback = $Player.get_stream_playback()
	# `_fill_buffer` must be called *after* setting `playback`,
	# as `fill_buffer` uses the `playback` member variable.
	_fill_buffer()


func _on_frequency_h_slider_value_changed(value):
	%FrequencyLabel.text = "%d Hz" % value
	pulse_hz = value


func _on_volume_h_slider_value_changed(value):
	# Use `linear_to_db()` to get a volume slider that matches perceptual human hearing.
	%VolumeLabel.text = "%.2f dB" % linear_to_db(value)
	$Player.volume_db = linear_to_db(value)
