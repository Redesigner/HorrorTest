class_name Bullet

extends Node3D

@onready var light = get_node("OmniLight3D");

func _ready():
	var timer = get_tree().create_timer(0.02);
	timer.connect("timeout", destroy);
	
func destroy():
	light.queue_free();
