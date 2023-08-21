#pragma once

namespace dae
{
	class Subject;

	enum Event //TODO improve event system
	{
		LevelLoad,
		LevelUnload,
		PausedGame,

		LiveLost,
		ObjectDied,

		ScoreUpdated,
		CounterFinished,

		CollisionEnter,
		CollisionExit,

		TriggerEnter,
		TriggerExit,

		ButtonPressed,
		OpenDoor
	};


	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event currEvent, Subject* actor) = 0;

	};	
}

