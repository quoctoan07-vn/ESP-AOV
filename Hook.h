#pragma once
#include "ESP.h"
#include "structheroname.h"
#include "tmg.h"

// ====================== CẤU HÌNH ESP ======================
struct ESP_Config {
    bool Enable = true;
    bool Line = true;
    bool Box = true;
    bool Cooldown = true;
    bool HP = true;
    bool Map = true;
    bool VisibleCheck = false;
    bool PlayerInfo = true;
    bool Alert = true;
    bool HeroImage = true;

    bool Line2 = true;      // Monster
    bool Box2 = true;
    bool VisibleCheck2 = false;
    bool PlayerInfo2 = true;
    bool HP2 = true;
    bool Map2 = true;
    bool Target = true;
} espConfig;

struct Response {
	Player players[140];
	Monster sMonster[140];
	int count;
	int c_mon;
};
Response Response{};

void (*_LateUpdate_CameraSystem)(void *ins);
void LateUpdate_CameraSystem_Hook(void *ins) {
    if (!ins || !_get_actorManager_KyriosFramework()) {
        _LateUpdate_CameraSystem(ins);
        return;
    }

    // Reset data
    Response.count = 0;
    Response.c_mon = 0;

    void* myActorLinker = nullptr;
    void* myMonsterLinker = nullptr;

    monoList<void**>* GetAllHeros = _GetAllHeros_ActorManager(_get_actorManager_KyriosFramework());
    if (GetAllHeros && GetAllHeros->getSize() > 0) {
        void** ActorLinkerItems = (void**)GetAllHeros->getItems();
        myActorLinker = getMyActorLinker(GetAllHeros);

        if (LGameActorMgr) {
            monoList<void**>* LGetAllHeros = _GetAllHeros_LGameActorMgr(LGameActorMgr);
            if (LGetAllHeros) {
                void** LActorRootItems = (void**)LGetAllHeros->getItems();
                Response.count = LGetAllHeros->getSize();

                for (int i = 0; i < Response.count && i < 140; i++) {
                    void* LActorRoot = LActorRootItems[i * 2 + 1];
                    void* ActorLinker = ActorLinkerItems[i * 2 + 1];
                    if (!LActorRoot || !ActorLinker) continue;

                    Player& p = Response.players[i];

                    p.visible = _get_bVisible_ActorLinker(ActorLinker);
                    if (espConfig.VisibleCheck && p.visible) continue;

                    p.configID = *(int*)((uintptr_t)LActorRoot + _actorConfig);

                    // HP
                    void* ValueComponent = *(void**)((uintptr_t)LActorRoot + _ValueComponentO);
                    if (ValueComponent) {
                        p.actorHp = _get_actorHp_ValueComponent(ValueComponent);
                        p.actorHpTotal = _get_actorHpTotal_ValueComponent(ValueComponent);
                    }

                    p.isEnemy = (_GiveMyEnemyCamp_LActorRoot(LActorRoot) == _get_objCamp_ActorLinker(myActorLinker));
                    p.camp = _get_objCamp_ActorLinker(ActorLinker);

                    Vector3 pos = VInt2Vector(_get_location_LActorRoot(LActorRoot), _get_forward_LActorRoot(LActorRoot));
                    p.PosEnemy = pos;
                    p.position = _WorldToScreenPoint_Camera(_get_main_Camera(), pos);
                    p.distance = (int)Vector3::Distance(_getPosition_ActorLinker(myActorLinker), pos);

                    // Skill CD
                    void* AsHero = _AsHero_LActorRoot(LActorRoot);
                    if (AsHero) {
                        p.skill1CD = _GetHeroWrapSkillData_LHeroWrapper(AsHero, 1).Skill1SlotCD / 1000;
                        p.skill2CD = _GetHeroWrapSkillData_LHeroWrapper(AsHero, 2).Skill1SlotCD / 1000;
                        p.skill3CD = _GetHeroWrapSkillData_LHeroWrapper(AsHero, 3).Skill1SlotCD / 1000;
                        p.skill4CD = _GetHeroWrapSkillData_LHeroWrapper(AsHero, 4).Skill1SlotCD / 1000;
                        p.skill5CD = _GetHeroWrapSkillData_LHeroWrapper(AsHero, 5).Skill1SlotCD / 1000;
                    }
                }
            }
        }
    }

    monoList<void**>* GetAllMonsters = _GetAllMonsters_ActorManager(_get_actorManager_KyriosFramework());
    if (GetAllMonsters && GetAllMonsters->getSize() > 0) {
        void** MonsterLinkerItems = (void**)GetAllMonsters->getItems();
        myMonsterLinker = getMyActorLinker(GetAllMonsters);

        if (LGameActorMgr) {
            monoList<void**>* LGetAllMonsters = _GetAllMonsters_LGameActorMgr(LGameActorMgr);
            if (LGetAllMonsters) {
                void** LMonsterRootItems = (void**)LGetAllMonsters->getItems();
                Response.c_mon = LGetAllMonsters->getSize();

                for (int i = 0; i < Response.c_mon && i < 140; i++) {
                    void* LMonsterRoot = LMonsterRootItems[i * 2 + 1];
                    void* MonsterLinker = MonsterLinkerItems[i * 2 + 1];
                    if (!LMonsterRoot) continue;

                    Monster& m = Response.sMonster[i];

                    m.configID = *(int*)((uintptr_t)LMonsterRoot + _actorConfig);

                    void* ValueComponent = *(void**)((uintptr_t)LMonsterRoot + _ValueComponentO);
                    if (ValueComponent) {
                        m.Hp = _get_actorHp_ValueComponent(ValueComponent);
                        m.HpTotal = _get_actorHpTotal_ValueComponent(ValueComponent);
                    }

                    m.isEnemy = (_GiveMyEnemyCamp_LActorRoot(LMonsterRoot) == _get_objCamp_ActorLinker(myMonsterLinker));

                    Vector3 pos = VInt2Vector(_get_location_LActorRoot(LMonsterRoot), _get_forward_LActorRoot(LMonsterRoot));
                    m.Pos = pos;
                    m.position = _WorldToScreenPoint_Camera(_get_main_Camera(), pos);
                    m.distance = (int)Vector3::Distance(_getPosition_ActorLinker(myMonsterLinker), pos);
                    m.camp = _get_objCamp_ActorLinker(MonsterLinker);
                }
            }
        }
    }

    _LateUpdate_CameraSystem(ins);
}
