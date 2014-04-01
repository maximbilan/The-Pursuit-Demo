counter = 0
currentIndex = 0
lastIndex = 0
respawnPlayer = 0

function this.OnRespawnPlayer ( this )
	
	if lastIndex == 0 then
		this:respawn_player( "SpawnPoint008" )
	elseif lastIndex == 1 then
		this:respawn_player( "SpawnPoint001" )
	elseif lastIndex == 2 then
		this:respawn_player( "SpawnPoint002" )
	elseif lastIndex == 3 then
		this:respawn_player( "SpawnPoint003" )
	elseif lastIndex == 4 then
		this:respawn_player( "SpawnPoint004" )
	elseif lastIndex == 5 then
		this:respawn_player( "SpawnPoint005" )
	elseif lastIndex == 6 then
		this:respawn_player( "SpawnPoint006" )
	elseif lastIndex == 7 then
		this:respawn_player( "SpawnPoint007" )
	end
	
end

function this.TriggerFinishOnEnter ( this )
	this:gameplay_log( "TriggerFinishOnEnter" )
	
	if counter >= 7 then
		counter = 0
		lastIndex = 0
		respawnPlayer = 0
		this:time_attack_finish_lap()
	else
		this:gameplay_log( "Wrong direction!" )
		respawnPlayer = 1
	end
	
end

function this.TriggerFinishOnLeave ( this )
	this:gameplay_log( "TriggerFinishOnLeave" )
	
	if respawnPlayer == 1 then
		respawnPlayer = 0
		this:respawn_player( "SpawnPoint008" )
	end
end

function this.Trigger01OnEnter ( this )
	this:gameplay_log( "Trigger01OnEnter" )
	
	currentIndex = 1
	if lastIndex == 0 then
		lastIndex = currentIndex
		counter = counter + 1
		respawnPlayer = 0
	else
		this:gameplay_log( "Wrong direction!" )
		respawnPlayer = 1
	end
end

function this.Trigger01OnLeave ( this )
	this:gameplay_log( "Trigger01OnLeave" )
	
	if respawnPlayer == 1 then
		respawnPlayer = 0
		this:respawn_player( "SpawnPoint001" )
	end
end

function this.Trigger02OnEnter ( this )
	this:gameplay_log( "Trigger02OnEnter" )
	
	currentIndex = 2
	if lastIndex == 1 then
		lastIndex = currentIndex
		counter = counter + 1
		respawnPlayer = 0
	else
		this:gameplay_log( "Wrong direction!" )
		respawnPlayer = 1
	end
end

function this.Trigger02OnLeave ( this )
	this:gameplay_log( "Trigger02OnLeave" )
	
	if respawnPlayer == 1 then
		respawnPlayer = 0
		this:respawn_player( "SpawnPoint002" )
	end
end

function this.Trigger03OnEnter ( this )
	this:gameplay_log( "Trigger03OnEnter" )
	
	currentIndex = 3
	if lastIndex == 2 then
		lastIndex = currentIndex
		counter = counter + 1
		respawnPlayer = 0
	else
		this:gameplay_log( "Wrong direction!" )
		respawnPlayer = 1
	end
end

function this.Trigger03OnLeave ( this )
	this:gameplay_log( "Trigger03OnLeave" )
	
	if respawnPlayer == 1 then
		respawnPlayer = 0
		this:respawn_player( "SpawnPoint003" )
	end
end

function this.Trigger04OnEnter ( this )
	this:gameplay_log( "Trigger04OnEnter" )
	
	currentIndex = 4
	if lastIndex == 3 then
		lastIndex = currentIndex
		counter = counter + 1
		respawnPlayer = 0
	else
		this:gameplay_log( "Wrong direction!" )
		respawnPlayer = 1
	end
end

function this.Trigger04OnLeave ( this )
	this:gameplay_log( "Trigger04OnLeave" )
	
	if respawnPlayer == 1 then
		respawnPlayer = 0
		this:respawn_player( "SpawnPoint004" )
	end
end

function this.Trigger05OnEnter ( this )
	this:gameplay_log( "Trigger05OnEnter" )
	
	currentIndex = 5
	if lastIndex == 4 then
		lastIndex = currentIndex
		counter = counter + 1
		respawnPlayer = 0
	else
		this:gameplay_log( "Wrong direction!" )
		respawnPlayer = 1
	end
end

function this.Trigger05OnLeave ( this )
	this:gameplay_log( "Trigger05OnLeave" )
	
	if respawnPlayer == 1 then
		respawnPlayer = 0
		this:respawn_player( "SpawnPoint005" )
	end
end

function this.Trigger06OnEnter ( this )
	this:gameplay_log( "Trigger06OnEnter" )
	
	currentIndex = 6
	if lastIndex == 5 then
		lastIndex = currentIndex
		counter = counter + 1
		respawnPlayer = 0
	else
		this:gameplay_log( "Wrong direction!" )
		respawnPlayer = 1
	end
end

function this.Trigger06OnLeave ( this )
	this:gameplay_log( "Trigger06OnLeave" )
	
	if respawnPlayer == 1 then
		respawnPlayer = 0
		this:respawn_player( "SpawnPoint006" )
	end
end

function this.Trigger07OnEnter ( this )
	this:gameplay_log( "Trigger07OnEnter" )
	
	currentIndex = 7
	if lastIndex == 6 then
		lastIndex = currentIndex
		counter = counter + 1
		respawnPlayer = 0
	else
		this:gameplay_log( "Wrong direction!" )
		respawnPlayer = 1
	end
end

function this.Trigger07OnLeave ( this )
	this:gameplay_log( "Trigger07OnLeave" )
	
	if respawnPlayer == 1 then
		respawnPlayer = 0
		this:respawn_player( "SpawnPoint007" )
	end
end