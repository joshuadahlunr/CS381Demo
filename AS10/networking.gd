extends Control

# Preload the Player scene so it can be instantiated quickly when needed
const PLAYER_SCENE = preload("res://Player.tscn")

# Port used for hosting/joining the multiplayer session
const PORT = 9999

# Reference to the input field where the user types the server IP address
@onready var IP_FIELD = $Menu/PanelContainer/VBoxContainer/IP

# Reference to a UI container (likely a Label or Control node) where player instances will be added
@onready var player_ids = $Label/PlayerIDs

# Create a new ENet multiplayer peer (used for both server and client)
var peer : ENetMultiplayerPeer = ENetMultiplayerPeer.new()


# Common setup for both hosting and joining:
# - Hide the menu UI
# - Show the player label/container UI
func button_common():
	hide()
	$Label.show()


# Called when the "Host" button is pressed
func _on_host_pressed() -> void:
	button_common()
	
	# Start a server on the specified port
	peer.create_server(PORT)
	
	# Assign this peer to the multiplayer system
	multiplayer.multiplayer_peer = peer
	
	# Connect signals to track when players join or leave
	multiplayer.peer_connected.connect(add_player)
	multiplayer.peer_disconnected.connect(remove_player)
	
	# Add the host player manually (server is also a player)
	add_player(multiplayer.get_unique_id())


# Called when the "Join" button is pressed
func _on_join_pressed() -> void:
	button_common()
	
	# Connect to a server using the IP from the input field
	peer.create_client(IP_FIELD.text, PORT)
	
	# Assign this peer to the multiplayer system
	multiplayer.multiplayer_peer = peer
	

# Called when a new peer connects (or manually for the host)
func add_player(peer_id):
	# Create a new player instance from the preloaded scene
	var player = PLAYER_SCENE.instantiate()
	
	# TODO: Assign some identifier (like peer_id) to the player
	# This could be used later to track and remove the correct player
	
	# Add the player node to the UI/container
	player_ids.add_child(player)
	

# Called when a peer disconnects
func remove_player(peer_id):
	# TODO: Find the player node associated with this peer_id
	# This requires storing a mapping (e.g., dictionary of peer_id -> player node)
	
	# if player: player.queue_free()
	pass
