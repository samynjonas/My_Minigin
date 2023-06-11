#pragma once

namespace dae
{
	class subject;

	enum Event //TODO improve event system
	{
		LevelLoad,
		LevelUnload,

		LiveLost,
		ObjectDied,

		ScoreUpdated,

		CollisionEnter,
		CollisionExit,

		TriggerEnter,
		TriggerExit,

		Teleport
	};


	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event currEvent, subject* actor) = 0;

	};	
}

