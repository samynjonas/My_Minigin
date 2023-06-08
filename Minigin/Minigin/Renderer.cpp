#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

#include "imgui.h"
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl2.h>

#include <iostream>
#include <chrono>

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render() 
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float angle, SDL_RendererFlip flip) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	//SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, angle, NULL, flip);
}


//std::vector<float> dae::Renderer::ExerciseOneTimings(int runTimes)
//{
//	std::vector<float> timings;
//
//	const int size{ 10'000'000 };
//	int* myArray = new int[size] {};
//
//	const int steps = 11;
//	float arrTimings[steps]{};
//
//	for (int index = 0; index < runTimes; index++)
//	{
//		int stepNumber = 0;
//		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
//		{
//			auto start = std::chrono::high_resolution_clock::now();
//			for (int i = 0; i < size; i += stepsize)
//			{
//				myArray[i] *= 2;
//			}
//			auto end = std::chrono::high_resolution_clock::now();
//			auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//
//			float timeInMs{ elapsedTime / 1000.f };
//			arrTimings[stepNumber] += timeInMs;
//			stepNumber++;
//		}
//	}
//
//	//End result averages
//	for (int i = 0; i < steps; i++)
//	{
//		arrTimings[i] /= runTimes;
//		timings.push_back(arrTimings[i]);
//	}
//
//	delete[] myArray;
//	return timings;
//}
//
//std::vector<float> dae::Renderer::ExerciseTwoNormalTimings(int runTimes)
//{
//	const int size{ 10'000'000 };
//	GameObject3D* myArray = new GameObject3D[size]{};
//
//	const int steps = 11;
//	float arrTimings[steps]{};
//
//	for (int index = 0; index < runTimes; index++)
//	{
//		//std::cout << "run " << (index + 1) << " timings: \n";
//
//		int stepNumber = 0;
//		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
//		{
//			auto start = std::chrono::high_resolution_clock::now();
//			for (int i = 0; i < size; i += stepsize)
//			{
//				myArray[i].ID *= 2;
//			}
//			auto end = std::chrono::high_resolution_clock::now();
//			auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//
//			float timeInMs{ elapsedTime / 1000.f };
//			arrTimings[stepNumber] += timeInMs;
//
//			//std::cout << "\t -step " << (stepNumber + 1) << " took " << timeInMs << "ms\n";
//			stepNumber++;
//		}
//	}
//	//End result GameObject3D averages
//
//	std::vector<float> vecTimings;
//	for (int i = 0; i < steps; i++)
//	{
//		arrTimings[i] /= runTimes;
//		vecTimings.push_back(arrTimings[i]);
//	}
//
//	delete[] myArray;
//	return vecTimings;
//}
//std::vector<float> dae::Renderer::ExerciseTwoAlteredTimings(int runTimes)
//{
//	const int size{ 10'000'000 };
//	GameObject3DAlt* myArray = new GameObject3DAlt[size]{};
//
//	const int steps = 11;
//	float arrTimings[steps]{};
//
//	for (int index = 0; index < runTimes; index++)
//	{
//		//std::cout << "run " << (index + 1) << " timings: \n";
//
//		int stepNumber = 0;
//		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
//		{
//			auto start = std::chrono::high_resolution_clock::now();
//			for (int i = 0; i < size; i += stepsize)
//			{
//				myArray[i].ID *= 2;
//			}
//			auto end = std::chrono::high_resolution_clock::now();
//			auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//
//			float timeInMs{ elapsedTime / 1000.f };
//			arrTimings[stepNumber] += timeInMs;
//
//			//std::cout << "\t -step " << (stepNumber + 1) << " took " << timeInMs << "ms\n";
//			stepNumber++;
//		}
//	}
//	//End result GameObject3DAlt averages:
//
//	std::vector<float> vecTimings;
//	for (int i = 0; i < steps; i++)
//	{
//		arrTimings[i] /= runTimes;
//		vecTimings.push_back(arrTimings[i]);
//	}
//
//	delete[] myArray;
//	return vecTimings;
//}

inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
