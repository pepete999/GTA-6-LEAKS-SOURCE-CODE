#pragma once
#include "core/GameScript.h"
#include "core/Ped.h"
#include "world/LeonidaWorld.h"
#include "ai/NPCDialogueManager.h"
#include "ai/ScenarioManager.h"

namespace vice_beach_tasks
{
    const float PARTY_VIBE_LEVEL = 90.0f;
    const int SCENARIO_DURATION_MS = 60000;

    void UpdateBeachPartyVibe(CPed* pPed, float vibeIncrement)
    {
        if (pPed && pPed->IsAlive())
        {
            pPed->vibeLevel = min(pPed->vibeLevel + vibeIncrement, 100.0f);
            if (pPed->vibeLevel > PARTY_VIBE_LEVEL)
            {
                NPCDialogueManager::TriggerRandomDialogue(pPed, "VICE_BEACH_VIBE");
            }
        }
    }

    void CommandTaskJasonStartParty(int pedIndex, const scrVector& partyPos)
    {
        Vector3 pos(partyPos);
        CTask* pTask = rage_new CTaskStartScenario("BEACH_PARTY", pos, -1);
        CScriptPeds::GivePedScriptedTask(pedIndex, pTask, SCRIPT_TASK_JASON_START_PARTY, "TASK_JASON_START_PARTY");
        NPCDialogueManager::QueueDialogue(pedIndex, "JASON_PARTY_YELL", "Fuck yeah, let’s burn Vice Beach down!");
    }

    void CommandTaskLuciaFlamingoFight(int pedIndex, int targetPedIndex)
    {
        CPed* pTarget = CTheScripts::GetEntityToQueryFromGUID<CPed>(targetPedIndex);
        if (pTarget)
        {
            CTask* pTask = rage_new CTaskCombatMelee(pTarget, "FLAMINGO_WEAPON", -1);
            CScriptPeds::GivePedScriptedTask(pedIndex, pTask, SCRIPT_TASK_LUCIA_FLAMINGO_FIGHT, "TASK_LUCIA_FLAMINGO_FIGHT");
            NPCDialogueManager::QueueDialogue(pedIndex, "LUCIA_FIGHT_TAUNT", "Get fucked by my flamingo, bitch!");
            NPCDialogueManager::QueueDialogue(targetPedIndex, "TARGET_FIGHT_RESPONSE", "You’re fuckin’ crazy, Lucia!");
        }
    }

    void CommandTaskNPCBeachGossip(int pedIndex, int duration)
    {
        if (SCRIPT_VERIFY((duration == -1) || (duration > 0), "TASK_NPC_BEACH_GOSSIP - Duration must be -1 or > 0"))
        {
            CTask* pTask = rage_new CTaskChat(NULL, duration, "VICE_BEACH_CHAT");
            CScriptPeds::GivePedScriptedTask(pedIndex, pTask, SCRIPT_TASK_NPC_BEACH_GOSSIP, "TASK_NPC_BEACH_GOSSIP");
            NPCDialogueManager::QueueRandomDialogue(pedIndex, "VICE_BEACH_GOSSIP", {
                "Fuck the cops, they’re ruining the beach vibe!",
                "Jason’s party last night was fuckin’ wild!",
                "Lucia’s got Vice Beach in a damn frenzy!"
            });
        }
    }

    void RegisterBeachTasks()
    {
        SCR_REGISTER_SECURE(TASK_JASON_START_PARTY, 0x890a1b2c3d4e5f67, CommandTaskJasonStartParty);
        SCR_REGISTER_SECURE(TASK_LUCIA_FLAMINGO_FIGHT, 0x90a1b2c3d4e5f678, CommandTaskLuciaFlamingoFight);
        SCR_REGISTER_SECURE(TASK_NPC_BEACH_GOSSIP, 0xa1b2c3d4e5f67890, CommandTaskNPCBeachGossip);
    }
} // namespace vice_beach_tasks