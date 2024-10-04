#include "Scene/SceneTitle.h"
#include "Scene/SceneManager.h"
#include <memory>
#include <sstream>
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include <thread>

#include "System/Framework.h"

// ���������Ԋu�ݒ�
static const int syncInterval = 0;
// �������������Ɏw��̃t���[�����[�g��ݒ肵�����ꍇ
static const int frameRate = 60;

// �R���X�g���N�^
Framework::Framework(HWND hWnd)
	: hWnd(hWnd)
	, input(hWnd)
	, graphics(hWnd)
{
	hDC = GetDC(hWnd);
	
	// �^�C�g�����
	SceneManager::Instance().ChangeScene(new SceneTitle);
}

// �f�X�g���N�^
Framework::~Framework()
{
	SceneManager::Instance().Clear();


	ReleaseDC(hWnd, hDC);

}

// �X�V����
void Framework::Update(float elapsedTime/*Elapsed seconds from last frame*/)
{
	// ���͍X�V����
	input.Update();

	// �V�[���X�V����
	SceneManager::Instance().Update(elapsedTime);
}

// �`�揈��
void Framework::Render(float elapsedTime/*Elapsed seconds from last frame*/)
{
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	// IMGUI�t���[���J�n����
	graphics.GetImGuiRenderer()->NewFrame();

	// �V�[���`�揈��
	SceneManager::Instance().Render();

	// IMGUI�`��
	graphics.GetImGuiRenderer()->Render(dc);

	// �o�b�N�o�b�t�@�ɕ`�悵�������ʂɕ\������B
	graphics.GetSwapChain()->Present(syncInterval, 0);
}

// �t���[�����[�g�v�Z
void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hWnd, outs.str().c_str());

		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
}

// �A�v���P�[�V�������[�v
int Framework::Run()
{
	MSG msg = {};

	// �t���[�����[�g�����Ɋւ���ݒ�
	timeBeginPeriod(1);
	std::chrono::time_point<std::chrono::steady_clock> sleepUntil = std::chrono::steady_clock::now();
	constexpr auto oneFrameDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::seconds(1)) / frameRate;

	while (WM_QUIT != msg.message)
	{
		auto frameStartTime = std::chrono::high_resolution_clock::now();
		// ���ݎ��Ԃ��擾

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();
			CalculateFrameStats();
			float elapsedTime = syncInterval == 0
				? timer.TimeInterval()
				: syncInterval / static_cast<float>(GetDeviceCaps(hDC,VREFRESH))
				;
			Update(elapsedTime);
			Render(elapsedTime);


			// 1�t���[�����̎��Ԃ����Z
			sleepUntil += oneFrameDuration;
			// ���Ԃ�����܂ő҂�
			std::this_thread::sleep_until(sleepUntil);
		}
	}
	timeBeginPeriod(0);
	return static_cast<int>(msg.wParam);
}

// ���b�Z�[�W�n���h��
LRESULT CALLBACK Framework::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Graphics::Instance().GetImGuiRenderer()->HandleMessage(hWnd, msg, wParam, lParam))
		return true;
	Mouse& mouse = Input::Instance().GetMouse();
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_MOUSEWHEEL:
		mouse.SetWheel(static_cast<int>((float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA));
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.Start();
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
