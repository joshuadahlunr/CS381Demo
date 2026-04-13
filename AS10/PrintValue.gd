extends VSlider

@export var number : float
@export var slider : VSlider
#@onready var slider = $"."
@onready var camera = $"../../../Camera3D"

func _process(delta: float) -> void:
	print(slider.value)

func _on_value_changed(value: float) -> void:
	#print(value)
	pass
