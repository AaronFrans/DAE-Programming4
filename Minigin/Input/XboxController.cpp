#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")

#include "XboxController.h"

using namespace dae;

class XboxController::XboxControllerImpl final
{
public:
	XboxControllerImpl(unsigned int controllerIndex)
	{
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		m_ControllerIndex = controllerIndex;
	}

	~XboxControllerImpl() = default;

	XboxControllerImpl(const XboxControllerImpl& other) = delete;
	XboxControllerImpl(XboxControllerImpl&& other) = delete;
	XboxControllerImpl& operator=(const XboxControllerImpl& other) = delete;
	XboxControllerImpl& operator=(XboxControllerImpl&& other) = delete;

	void Update()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

		XInputGetState(m_ControllerIndex, &m_CurrentState);

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_PreviousState.Gamepad.wButtons);
	}

	bool IsDownThisFrame(unsigned int button) const { return m_ButtonsPressedThisFrame & button; };
	bool IsUpThisFrame(unsigned int button) const {return m_ButtonsReleasedThisFrame & button;};
	bool IsPressedThisFrame(unsigned int button) const {return m_CurrentState.Gamepad.wButtons & button;};



private:
	XINPUT_STATE m_PreviousState;
	XINPUT_STATE m_CurrentState;

	WORD m_ButtonsPressedThisFrame;
	WORD m_ButtonsReleasedThisFrame;

	unsigned int m_ControllerIndex;
};

void dae::XboxController::Update()
{
	m_pImpl->Update();
}

bool dae::XboxController::IsDown(ControllerButton button) const
{
	return m_pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool dae::XboxController::IsUp(ControllerButton button) const
{
	return m_pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool dae::XboxController::IsPressed(ControllerButton button) const
{
	return m_pImpl->IsPressedThisFrame(static_cast<unsigned int>(button));
}

dae::XboxController::XboxController(unsigned int controllerIndex)
{
	m_pImpl = std::make_unique<XboxControllerImpl>(controllerIndex);
}

dae::XboxController::~XboxController()
{
}
