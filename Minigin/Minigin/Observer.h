#pragma once

namespace dae
{
	class subject;

	enum Event //TODO improve event system
	{
		LiveLost,
		ObjectDied,

		ScoreUpdated,

		CollisionEnter,
		CollisionExit,

		TriggerEnter,
		TriggerExit
	};


	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event currEvent, subject* actor) = 0;

	};	
}

