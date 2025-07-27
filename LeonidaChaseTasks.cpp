#pragma once
#include "core/GameScript.h"
#include "core/Ped.h"
#include "core/Vehicle.h"
#include "world/LeonidaWorld.h"
#include "ai/NPCDialogueManager.h"
#include "ai/VehicleChaseFramework.h"

namespace leonida_chase_tasks
{
    const float CHASE_SPEED_LIMIT = 60.0f;
    const int CHASE_DURATION_MS = 90000;

    void UpdateChaseIntensity(CPed* pPed, float intensityIncrement)
    {
        if (pPed && pPed->IsAlive())
        {
            pPed->chaseIntensity = min(pPed->chaseIntensity + intensityIncrement, 100.0f);
            if (pPed->chaseIntensity > 80.0f)
            {
                NPCDialogueManager::TriggerRandomDialogue(pPed, "LEONIDA_CHASE_QUOTE");
            }
        }
    }

    void CommandTaskJasonCarChase(int pedIndex, int vehicleIndex, int targetPedIndex)
    {
        CVehicle* pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(vehicleIndex);
        CPed* pTarget = CTheScripts::GetEntityToQueryFromGUID<CPed>(targetPedIndex);
        if (pVehicle && pTarget)
        {
            CTask* pTask = rage_new CTaskVehicleChase(pVehicle, pTarget, CHASE_SPEED_LIMIT, CHASE_DURATION_MS / 1000.0f);
            CScriptPeds::GivePedScriptedTask(pedIndex, pTask, SCRIPT_TASK_JASON_CAR_CHASE, "TASK_JASON_CAR_CHASE");
            NPCDialogueManager::QueueDialogue(pedIndex, "JASON_CHASE_TAUNT", "Fuck you, you’re not getting away!");
        }
    }

    void CommandTaskLuciaEvadeCops(int pedIndex, const scrVector& evadePos)
    {
        Vector3 pos(evadePos);
        CTask* pTask = rage_new CTaskSmartFlee(CAITarget(pos), 100.0f, -1);
        CScriptPeds::GivePedScriptedTask(pedIndex, pTask, SCRIPT_TASK_LUCIA_EVADE_COPS, "TASK_LUCIA_EVADE_COPS");
        NPCDialogueManager::QueueDialogue(pedIndex, "LUCIA_EVADE_YELL", "Fuck the cops, they’ll never catch me!");
    }

    void CommandTaskNPCCopTaunt(int pedIndex, int targetPedIndex)
    {
        CPed* pTarget = CTheScripts::GetEntityToQueryFromGUID<CPed>(targetPedIndex);
        if (pTarget)
        {
            CTask* pTask = rage_new CTaskCombat(pTarget, -1, CombatFlags::CF_Taunt);
            CScriptPeds::GivePedScriptedTask(pedIndex, pTask, SCRIPT_TASK_NPC_COP_TAUNT, "TASK_NPC_COP_TAUNT");
            NPCDialogueManager::QueueRandomDialogue(pedIndex, "LEONIDA_COP_TAUNT", {
                "You’re fucked now, punk!",
                "Stop running, you piece of shit!",
                "Leonida PD’s got your ass!"
            });
        }
    }

    void RegisterChaseTasks()
    {
        SCR_REGISTER_SECURE(TASK_JASON_CAR_CHASE, 0xb2c3d4e5f67890a1, CommandTaskJasonCarChase);
        SCR_REGISTER_SECURE(TASK_LUCIA_EVADE_COPS, 0xc3d4e5f67890a1b2, CommandTaskLuciaEvadeCops);
        SCR_REGISTER_SECURE(TASK_NPC_COP_TAUNT, 0xd4e5f67890a1b2c3, CommandTaskNPCCopTaunt);
    }
} // namespace leonida_chase_tasks