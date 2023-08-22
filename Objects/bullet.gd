class_name Bullet

extends Node3D

var velocity;

func _physics_process(delta):
	velocity = transform.basis.z * -100;
	position += velocity * delta;
