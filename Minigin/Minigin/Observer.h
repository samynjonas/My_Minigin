#pragma once

namespace dae
{
	class subject;

	enum Event
	{
		PlayerDied,
		ScoreUpdated,
		CollisionEnter,
		CollisionStay,
		CollisionExit,
		TriggerEnter,
		TriggerStay,
		TriggerExit
	};


	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event currEvent, subject* actor) = 0;

	};	
}

