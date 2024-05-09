#include "Events.h"
#include "Settings.h"
#include "Utility.h"

namespace Events
{
    RE::BSEventNotifyControl InputEventHandler::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) noexcept
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (const auto ui{ RE::UI::GetSingleton() }; !ui->IsApplicationMenuOpen() && !ui->GameIsPaused() && !ui->IsItemMenuOpen() && !ui->IsModalMenuOpen()) {
            if (const auto control_map{ RE::ControlMap::GetSingleton() }; control_map->IsMovementControlsEnabled()) {
                for (auto e{ *a_event }; e != nullptr; e = e->next) {
                    if (const auto btn_event{ e->AsButtonEvent() }) {
                        const auto keycode{ btn_event->GetIDCode() };
                        if (btn_event->GetDevice() == RE::INPUT_DEVICE::kKeyboard && keycode == Settings::wait_hotkey) {
                            if (const auto timer{ RE::BSTimer::GetSingleton() }) {
                                if (btn_event->IsHeld()) {
                                    if (Utility::waiting) {
                                        if (Settings::ramp_up_multiplier) {
                                            Utility::multiplier_accumulator += 0.005;
                                            Utility::multiplier_accumulator = std::clamp(Utility::multiplier_accumulator, Settings::wait_multiplier, Settings::clamp_multiplier);
                                            timer->SetGlobalTimeMultiplier(Utility::multiplier_accumulator, true);
                                        }
                                    }
                                    else {
                                        Utility::waiting = true;
                                        RE::DebugNotification("Waiting...");
                                        timer->SetGlobalTimeMultiplier(Settings::wait_multiplier, true);
                                    }
                                }
                                if (btn_event->IsUp()) {
                                    timer->SetGlobalTimeMultiplier(1.0f, true);
                                    Utility::multiplier_accumulator = Settings::wait_multiplier;
                                    RE::DebugNotification("Finished waiting.");
                                    if (const auto calendar{ RE::Calendar::GetSingleton() }) {
                                        const auto hour_float{ calendar->GetHour() };
                                        const auto floor{ std::floor(hour_float) };
                                        auto       hour{ static_cast<int>(floor) };
                                        const auto minutes{ static_cast<int>(std::ceil((hour_float - floor) * 60)) };
                                        auto       am_pm{ "AM"s };
                                        if (hour >= 12) {
                                            hour -= 12;
                                            am_pm = "PM"s;
                                        }
                                        RE::DebugNotification(fmt::format("Current time: {}:{} {}", hour, minutes, am_pm).c_str());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }
} // namespace Events
