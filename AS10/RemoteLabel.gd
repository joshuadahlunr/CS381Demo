extends Label

var value = 0

func update_value(new_value, sender_id = 1):
	if !(is_multiplayer_authority() or sender_id == 0):
		update_value_rpc.rpc_id(1, new_value, multiplayer.get_unique_id())
		return
		
	value = new_value
	text = "Value: " + String.num_int64(value)
	
	if is_multiplayer_authority() and sender_id != 0:
		print(sender_id, " has set the value to: ", value)
		update_value_rpc.rpc(new_value, 0)

@rpc("any_peer")
func update_value_rpc(new_value, sender_id):
	if is_multiplayer_authority() or sender_id == 0:
		update_value(new_value, sender_id)

func _ready():
	if is_multiplayer_authority():
		update_value(0)

# HOMEWORK: Listen to the buttons
