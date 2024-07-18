# State machine

This plugin is a simple state machine that flows from one state to another automatically.

## Usage
Place a SM_StateMachine in the world and set the initial state class (State0).
This StateMachine will spawn the initial state and check if the state can transition or not.
State0 defines a default transition by overriding the function GetDefaultTransition.
If the state can transition (GetDefaultTransition returns true and another state) then the state machine spawns the new state (State1).
This state becomes the current state and it's transitions are checked.

This plugin supports nested state machines.

## Master state

the Master state is the common base for both StateMachine and "simple" States.

## Possible improvements

Delete state on exit

