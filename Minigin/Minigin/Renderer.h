#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <vector>

#include "imgui_plot.h"

namespace dae
{
	//struct TransformTemp
	//{
	//	float matrix[16] =
	//	{
	//		1, 0, 0, 0,
	//		1, 0, 0, 0,
	//		1, 0, 0, 0,
	//		1, 0, 0, 0
	//	};
	//};
	//
	//class GameObject3D
	//{
	//public:
	//	TransformTemp transform{};
	//	int ID{ 1 };
	//};
	//
	//class GameObject3DAlt
	//{
	//public:
	//	TransformTemp* transform{ nullptr };
	//	int ID{ 1 };
	//};



	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
		bool m_showDemo{ true };

		//const float x_data[11]{ 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
		//float y_data_Ex01[11]{};
		//float y_data_Ex02_Norm[11]{};
		//float y_data_Ex02_Alt[11]{};
		//
		//int runTimesEx1 = 1;
		//int runTimesEx2 = 1;
		//
		//std::vector<float> m_vecExerciseOneTimings{};
		//std::vector<float> m_vecExerciseTwoNormalTimings{};
		//std::vector<float> m_vecExerciseTwoAlteredTimings{};
		//
		////functions
		//std::vector<float> ExerciseOneTimings(int runTimes);
		//
		//std::vector<float> ExerciseTwoNormalTimings(int runTimes);
		//std::vector<float> ExerciseTwoAlteredTimings(int runTimes);


	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float angle = 0.f, SDL_RendererFlip flip = SDL_FLIP_NONE) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

