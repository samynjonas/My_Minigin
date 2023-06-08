#pragma once

namespace dae
{
	class subject;

	enum Event
	{
		PlayerDied,
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

