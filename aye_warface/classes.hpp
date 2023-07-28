#include "vector3.hpp"

bool CustomW2S(Vec3 vPos, Vec3& vOut);

template <typename T>
struct Plane_tpl {
    Vec3_tpl<T> n;
    T d;
};


class CCamera {
public:
    struct ScissorInfo {

        unsigned __int16 x1;
        unsigned __int16 y1;
        unsigned __int16 x2;
        unsigned __int16 y2;
    };

public:
    Matrix34_tpl<float> m_Matrix;
    float m_fov;
    int m_Width;
    int m_Height;
    float m_ProjectionRatio;
    float m_PixelAspectRatio;
    Vec3_tpl<float> m_edge_nlt;
    Vec3_tpl<float> m_edge_plt;
    Vec3_tpl<float> m_edge_flt;
    float m_asymL;
    float m_asymR;
    float m_asymB;
    float m_asymT;
    Vec3_tpl<float> m_cltp;
    Vec3_tpl<float> m_crtp;
    Vec3_tpl<float> m_clbp;
    Vec3_tpl<float> m_crbp;
    Vec3_tpl<float> m_cltn;
    Vec3_tpl<float> m_crtn;
    Vec3_tpl<float> m_clbn;
    Vec3_tpl<float> m_crbn;
    Vec3_tpl<float> m_cltf;
    Vec3_tpl<float> m_crtf;
    Vec3_tpl<float> m_clbf;
    Vec3_tpl<float> m_crbf;
    Plane_tpl<float> m_fp[6];
    unsigned int m_idx1[6];
    unsigned int m_idy1[6];
    unsigned int m_idz1[6];
    unsigned int m_idx2[6];
    unsigned int m_idy2[6];
    unsigned int m_idz2[6];
    float m_zrangeMin;
    float m_zrangeMax;
    int m_nPosX;
    int m_nPosY;
    int m_nSizeX;
    int m_nSizeY;
    interface IVisArea* m_pPortal;
    ScissorInfo m_ScissorInfo;
    void* m_pMultiCamera;
    Vec3_tpl<float> m_OccPosition;
    int m_JustActivated;
};

struct SProjectToScreen {

    SProjectToScreen() { }
    SProjectToScreen(Vec3_tpl<float> to, Vec3_tpl<float>& out)
        : ptx(to.x), pty(to.y), ptz(to.z), sx(&out.x), sy(&out.y), sz(&out.z) { }

    float ptx;
    float pty;
    float ptz;
    float* sx;
    float* sy;
    float* sz;
};

enum EActionEvent {

    eAE_channelCreated = 0x0,
    eAE_channelDestroyed = 0x1,
    eAE_connectFailed = 0x2,
    eAE_connected = 0x3,
    eAE_disconnected = 0x4,
    eAE_clientDisconnected = 0x5,
    eAE_resetBegin = 0x6,
    eAE_resetEnd = 0x7,
    eAE_resetProgress = 0x8,
    eAE_preSaveGame = 0x9,
    eAE_postSaveGame = 0xA,
    eAE_inGame = 0xB,
    eAE_serverName = 0xC,
    eAE_serverIp = 0xD,
    eAE_earlyPreUpdate = 0xE,
    eAE_demoRecorderCreated = 0xF,
    eAE_mapCmdIssued = 0x10,
    eAE_unloadLevel = 0x11,
    eAE_postUnloadLevel = 0x12,
    eAE_loadLevel = 0x13,
    eAE_demoRecorderDestroyed = 0x14,
};

struct SActionEvent {

    EActionEvent m_event;
    int m_value;
    const char* m_description;
};

struct IGameFrameworkListener {
public:
    virtual void Function0() { };
    virtual void OnPostUpdate(float fDeltaTime) { };
    virtual void OnSaveGame(struct ISaveGame* pSaveGame) { };
    virtual void OnLoadGame(struct ILoadGame* pLoadGame) { };
    virtual void OnLevelEnd(const char* nextLevel) { };
    virtual void OnActionEvent(const SActionEvent& event) { };
    virtual void OnPreRender() { };
};

struct ISkeletonPose
{
public:
    QuatT& GetAbsJointByID(int nJointID) { return GetVFunc<QuatT & (__thiscall*)(PVOID, int)>(this, 0x38)(this, nJointID); }
    int16 GetJointIdByName(const char* sz_joint_name) { return GetVFunc<int16(__fastcall*)(PVOID64, const char*)>(this, 0x18)(this, sz_joint_name); }
    const char* GetszNameByJointId(int16 sz_joint_name) { return GetVFunc<const char* (__fastcall*)(PVOID64, int16)>(this, 0x30)(this, sz_joint_name); }
};
struct ICharacterInstance
{
public:
    ISkeletonPose* GetISkeletonPose() {
        return GetVFunc<ISkeletonPose * (__thiscall*)(PVOID)>(this, 0x28)(this);
    }
};
struct AABB {
    Vec3_tpl<float> min;
    Vec3_tpl<float> max;
};
struct IEntityClass {
public:
    const char* GetName() { return GetVFunc<const char* (__fastcall*)(PVOID64)>(this, 0x10)(this); }
};
struct IEntity {
public:
    IEntityClass* GetEntityClass() { return GetVFunc<IEntityClass * (__fastcall*)(PVOID64)>(this, 0x18)(this); }
    ICharacterInstance* GetCharacter(int nSlot) {
        return GetVFunc<ICharacterInstance * (__thiscall*)(PVOID, int)>(this, 0x2A0)(this, nSlot);//maybe
    }

    EntityId GetEntityId() {
        return GetVFunc<EntityId(__thiscall*)(PVOID)>(this, 0x8)(this);
    }

    const char* GetName() {
        return GetVFunc<const char* (__thiscall*)(PVOID)>(this, 0x60)(this);
    }

    int GetID() {
        return *(int*)((uintptr_t)this + 0x18); // check offset
    }

   
    Matrix34 GetEntityWorldTM() { return *(Matrix34*)((uintptr_t)this + 0x40); }
    
    Vec3 GetBonePos(ISkeletonPose* pLocalSkeleton, EBone eBone)
    {
        Matrix34 World = GetEntityWorldTM();
        Matrix34 SkeletonAbs = Matrix34(pLocalSkeleton->GetAbsJointByID(eBone));
        Matrix34 matWorld = World;

        float m03, m13, m23;
        m03 = (World.m00 * SkeletonAbs.m03) + (World.m01 * SkeletonAbs.m13) + (World.m02 * SkeletonAbs.m23) + World.m03;
        matWorld.m03 = m03;
        m13 = (World.m10 * SkeletonAbs.m03) + (World.m11 * SkeletonAbs.m13) + (World.m12 * SkeletonAbs.m23) + World.m13;
        matWorld.m13 = m13;
        m23 = (World.m20 * SkeletonAbs.m03) + (World.m21 * SkeletonAbs.m13) + (World.m22 * SkeletonAbs.m23) + World.m23;
        matWorld.m23 = m23;

        return matWorld.GetTranslation();
    }
};
class QueryWorld {
public:
    EntityId LookAtEntity() { return in_lookAtEntityId; }

private:
    char pad_0x0000[0x170]; //0x0000
    EntityId in_lookAtEntityId; //0x0170
};
struct Object_Game {

    QueryWorld* Query() {
        int64_t ixtensions = *(int64_t*)(this + 0x68);

        if (ixtensions != *(int64_t*)(this + 0x70)) { ixtensions += 0x20i64; return *(QueryWorld**)ixtensions; }

        return nullptr;
    }
};
enum EGameStatType : int16_t
{
    eGS_InvalidStat = -1,
    eGS_AmmoAmount = 5,
    eGS_BayonetMeleeDuration = 13,
    eGS_BayonetMeleeOutDuration = 14,
    eGS_BoltAction = 17,
    eGS_BoltActionDuration = 18,
    eGS_BulletPierceability = 19,
    eGS_ClipSize = 22,
    eGS_Damage = 26,
    eGS_DamageMin = 29,
    eGS_DamageUnderbarrel = 30,
    eGS_HeadDamageMult = 39,
    eGS_MeleeDamage = 57,
    eGS_MeleeDuration = 58,
    eGS_MeleeDurationBuff = 59,
    eGS_MeleeOutDuration = 63,
    eGS_MeleeRadius = 64,
    eGS_MeleeRange = 65,
    eGS_Range = 67,
    eGS_RclMax = 71,
    eGS_RclRnd = 72,
    eGS_RclSmth = 73,
    eGS_ReloadDuration = 74,
    eGS_ReloadDurationBuff = 75,
    eGS_ReloadUnderbarrelDuration = 76,
    eGS_Rpm = 82,
    eGS_SecondaryMeleeDuration = 85,
    eGS_SecondaryMeleeDurationBuff = 86,
    eGS_SecondaryMeleeOutDuration = 87,
    eGS_SecondaryMeleeRadius = 88,
    eGS_SecondaryMeleeRange = 89,
    eGS_SprdMax = 97,
    eGS_SprdMin = 98,
    eGS_ReloadAmmoCount = 148
};
class CStatsArray
{
public:
    float operator[](EGameStatType type)
    {
        return GetValue(type);
    }
    void SetValue(EGameStatType type, float value)
    {
        auto it = m_stats.find(type);
        if (it != m_stats.end())
            it->second = value;
    }
    float GetValue(EGameStatType type)
    {
        auto it = m_stats.find(type);
        if (it == m_stats.end()) return 1072.0f;
        return it->second;
    }
    auto begin() { return m_stats.begin(); }
    auto end() { return m_stats.end(); }
private:
    std::map<int16_t, TMaskedFloat> m_stats;
};
struct CWeaponParamsTwo
{
public:
    CStatsArray* GetStats() { return &m_stats; }
    enum eWeaponStats : INT16
    {
        change_weapons_delay = 31,
        basic_melee_distance = 65,
        basic_min_melee_attack_speed = 58,
        basic_melee_attack_animation = 59,
        basic_max_melee_attack_speed = 63,
        extra_min_melee_attack_speed = 85,
        reload_delay = 74,
        reload_animation_delay = 75,
        extra_melee_attack_animation = 86,
        extra_max_melee_attack_speed = 87,
        min_spread_delay = 97,
        max_spread_delay = 98,
        min_sway_camera_delay = 104,
        scope_magnification_parameters = 110
    };

private:
    char pad_0x0000[0x68];
    CStatsArray m_stats; //0x0068 
};
template <typename TYPE, std::uintptr_t INDEX, typename ...ARGUMENTS>
__forceinline auto VTable(void* __ptr64 pointer, ARGUMENTS... arguments) {
    return ((*reinterpret_cast<TYPE(__thiscall***)(void*, ARGUMENTS...)>(pointer))[INDEX])(pointer, arguments...);
}

template <typename TYPE, typename ...ARGUMENTS>
__forceinline auto IVTable(void* __ptr64 pointer, std::uintptr_t INDEX, ARGUMENTS... arguments) {
    return ((*reinterpret_cast<TYPE(__thiscall***)(void*, ARGUMENTS...)>(pointer))[INDEX])(pointer, arguments...);
}
class IClipMode
{
public:
    virtual ~IClipMode() = 0;
    virtual int GetAmmoCount() = 0;
    virtual void SetAmmoCount(int count) = 0;
    virtual void ExpendAmmo() = 0;
    virtual bool IsEmpty() = 0;
    virtual bool IsBarrel() = 0;
    virtual bool IsRechargeable() = 0;
    virtual bool RunLevelNotification() = 0;
};
class SFireAmmoParams
{
public:
    void UnlimitedAmmo(bool infinite) { *(bool*)((DWORD64)this + 0x8) = infinite; }
    int GetCounterAmmo() { return (int)*(DWORD64*)((DWORD64)this + 0xC); }
    void SetCounterAmmo(int value) { *(int*)((DWORD64)this + 0xC) = value; }

};
class CWeaponSpecific
{
public:

    IClipMode* GetClipMode() { return *(IClipMode**)((uintptr_t)this + 0xC0); }
    CWeaponParamsTwo* GetIWeaponParamsTwo() { return (CWeaponParamsTwo*)*(DWORD64*)((DWORD64)this + 0xB8); }

    void GetShutter(int8 Value) { *(int8*)((DWORD64)this + 0xD3) = Value; }
    const char* GetWeaponName() { return (const char*)*(uint64*)((uint64)this + 0x28); }
    void IsFiring(bool Status) { *(DWORD64*)((DWORD64)this + 0xB8) = Status; }
    bool IsFiring()
    {
        DWORD64 Firing = *(DWORD64*)((DWORD64)this + 0xB8);
        return (Firing == 3 || Firing == 6 || Firing == 12 || Firing == 13 || Firing == 14 || Firing == 15 || Firing == 16 || Firing == 48 || Firing == 49);
    }
    void SetShootAmmo(int Value)
    {
        *(int*)((DWORD64) * (DWORD64*)((DWORD64)this + 0x48) + 0x10) = Value;
    }
    SFireAmmoParams* GetFireAmmoParams() { return (SFireAmmoParams*)*(DWORD64*)((DWORD64)this + 0x68); }

};
struct IWeapon
{
public:
    CWeaponSpecific* GetIWeaponSpecific() { return (CWeaponSpecific*)*(DWORD64*)((DWORD64)this + 0x8); }
    void Start_Fire() { GetVFunc<void(__thiscall*)(PVOID)>(this, 0x58)(this); }
    void Stop_Fire() { GetVFunc<void(__thiscall*)(PVOID)>(this, 0x60)(this); }
    void SetFiringPos(Vec3& Position) { return GetVFunc<void(__fastcall*)(PVOID, Vec3&)>(this, 0x2F0)(this, Position); }
};
struct IItem
{
public:
    IWeapon* GetIWeapon() { return (IWeapon*)((uintptr_t)this + 0x28); }
    //IWeapon* GetIWeapon() { return GetVFunc<IWeapon * (__fastcall*)(PVOID)>(this, 0x280)(this); }//0x280, 0x278
    //IWeapon* _GetIWeapon() { return &m_weapon; }

private:
    IWeapon m_weapon; //0x0028
};
struct IAnimationGraphState
{
public:
    const char* GetCurrentStateName() { return GetVFunc<const char* (__thiscall*)(PVOID)>(this, 0x100)(this); }
};
interface IActor {
public:
    int GetHealth() {
        //return IVTable< int >(this, 28);
        return GetVFunc<int(__thiscall*)(PVOID)>(this, 0xE0)(this);
    }
    Object_Game* GetGameObject() { return *(Object_Game**)((uintptr_t)this + 0x18); }
    IAnimationGraphState* GetAnimationGraphState() { return GetVFunc<IAnimationGraphState * (__fastcall*)(PVOID)>(this, 0x130)(this); }//0x128 mb cahnge

    IItem* GetCurrentItem(bool includeVehicle = false) {
        return GetVFuncI<IItem * (__thiscall*)(PVOID, bool)>(this, 59)(this, includeVehicle);
        //return GetVFunc<IItem * (__thiscall*)(PVOID, bool)>(this, 0x1D0)(this, includeVehicle);
        //return IVTable<IItem * (__thiscall*)(PVOID, bool)>(this, 59, includeVehicle); 0x1D0
    }
    bool IsDead() {
        return this->GetHealth() <= 0;
    }
    uint32_t GetTeamId() {
        return *(uint32_t*)((uint64_t)this + 0x3E0);
    }
    /*IItemProxy* GetCurrentItem() {
        return IVTable< IItemProxy* >(this, 28);
    }*/

    /*IMovementController* GetMovementController() {
        return IVTable< IMovementController* >(this, data::GetMovementController);
    }*/

    /*void SetThirdPerson(bool bValue) {
        VTable< void, 67 >(this, bValue);
    }*/

    /*bool GetBonePos(Vec3_tpl<float>* pos, EBonesID ID, int slot, int* pBoneIndex) {
        return VTable< bool, 158 >(this, pos, ID, slot, pBoneIndex);
    }*/

    /*void Revive(bool bFromInit) {
        return VTable< void, 127 >(this, bFromInit);
    }

    void LocalRevive() {
        return VTable< void, 167 >(this);
    }

    void SetActorModel() {
        return VTable< void, 169 >(this);
    }

    void NetworkUpdateCharacterParams() {
        return VTable< void, 181 >(this);
    }*/

    IEntity* GetEntity() {
        return *(IEntity**)((uintptr_t)this + 0x10);
    }

    IEntity* getIEntity() {
        return (IEntity*)*(DWORD*)((DWORD)this + 0x08); //fake
        //return *(IEntity*)((uintptr_t)this + 0x08);
    }; //check offset
};

struct IActorSystem {
    IActor* GetActor(int entityId) {
        //return IVTable<IActor*>(this, 15);
        return VTable<IActor*, 15>(this, entityId);
    }
    IActor* GetClientActor()
    {
        IActor* pLocalActor = 0;
        bool bGetActorResult = GetVFunc<bool(__fastcall*)(PVOID, IActor**)>(this, 0x470)(this, &pLocalActor);
        if (!bGetActorResult) pLocalActor = 0;
        return pLocalActor;
    }


};

struct IGameRules {
    int GetTeam(int entityId) {
        return IVTable<int>(this, 100);
    }
};

struct IGameFramework {
    void RegisterListener(IGameFrameworkListener* pGameFrameworkListener, const char* name,
        EFRAMEWORKLISTENERPRIORITY eFrameworkListenerPriority) {
        VTable< void, 108 >(this, pGameFrameworkListener, name, eFrameworkListenerPriority);
    }
    void UnregisterListener(IGameFrameworkListener* pGameFrameworkListener) {
        VTable< void, 109 >(this, pGameFrameworkListener);
    }

    char GetClientActorInternal(IActor** pOutActor) {
        return IVTable< char >(this, 142, pOutActor);
    }

    IActorSystem* GetIActorSystem() {
        // return IVTable<IActorSystem*>(this, 25);
        return VTable<IActorSystem*, 25>(this);
    }

    IGameRules* GetIGameRules() {
        return *(IGameRules**)((DWORD)this + 0x618); // find offset
    }
};

//struct IGame {
//public:
//    IGameFramework* GetIGameFramework() {
//        return IVTable<IGameFramework*>(this, 15);
//    }
//};
struct IGame
{
    __forceinline IGameFramework* GetIGameFramework() { return pGameFramework; }
private:
    char pad_0x0000[0x48]; //0x0000
    IGameFramework* pGameFramework; //0x0048
};
struct IGameObjectExtension {

};

//struct IEntityIt {
//public: 
//    CREATE_FUNCTOR(IEntity*, Next, 0x19 / 4);
//};
struct IEntityIt
{
    virtual ~IEntityIt() {}
    virtual void AddRef() = 0;
    virtual void Release() = 0;
    virtual void MoveFirst() = 0;
    virtual bool IsEnd() = 0;
    virtual IEntity* This() = 0;
    virtual IEntity* Next() = 0;
};
struct IPhysicalEntity
{
public:
    void* GetForeignData(int itype = 0) { return GetVFunc<void* (__thiscall*)(PVOID, int)>(this, 0x68)(this, itype); }
};
struct IEntitySystem {
    virtual ~IEntitySystem() {}//0
    virtual void Relese() = 0;//1
    virtual void OnNetworkGameSync(unsigned int syncType) = 0;//2
    virtual void PrePhysicsUpdate() = 0;//3
    virtual void Update() = 0;//4
    virtual void Reset() = 0;//5
    virtual void Unload() = 0;//6
    virtual void DeletePendingEntities() = 0;//7
    virtual void Function8(); //
    virtual void Function9(); //
    virtual void Function10(); //
    virtual IEntity* GetEntity(EntityId id) = 0; //
    virtual void Function12(); //
    virtual void Function13(); //
    virtual void Function14(); //
    virtual void Function15(); //
    virtual void Function16(); //
    virtual void Function17(); //
    virtual void Function18(); //
    virtual void Function19(); //by classname
    virtual void Function20(); //by class
    virtual IEntityIt* GetEntityIterator() = 0; //
    virtual void Function22(); //
    virtual void Function23(); //
    virtual void Function24(); //
    virtual void Function25(); //
    virtual void Function26(); //
    IEntity* GetEntityFromPhysics(IPhysicalEntity* pPhysEntity) { return GetVFunc<IEntity * (__fastcall*)(PVOID, IPhysicalEntity*)>(this, 0xD0)(this, pPhysEntity); }

};




interface IRenderer {

    CCamera* SetGetCamera(const CCamera* pCamera, bool bSet) {
        return VTable< CCamera*, 68 >(this, pCamera, bSet);
    }
    Vec3 GetViewCamera() { return *(Vec3*)((uintptr_t)this + 0x1720); }

    float GetHypotenuse(Vec3 vPos) {
        Vec3 vScreen;
        {
            FLOAT PositionX = 0.f;
            FLOAT PositionY = 0.f;
            FLOAT ScreenCenterX = (GetWidth() / 2.0f);
            FLOAT ScreenCenterY = (GetHeight() / 2.0f);
            if (CustomW2S(vPos, vScreen) == false)
                return 9999.0f;
            PositionX = vScreen.x > ScreenCenterX ? vScreen.x - ScreenCenterX : ScreenCenterX - vScreen.x;
            PositionY = vScreen.y > ScreenCenterY ? vScreen.y - ScreenCenterY : ScreenCenterY - vScreen.y;
            return sqrt(PositionX * PositionX + PositionY * PositionY);
        }
        return 9999.0f;
    }
    bool IsFov(Vec3 vBonePos, int fov_) {
        return GetHypotenuse(vBonePos) <= fov_ * 4;
    }
    int GetWidth() {
        return VTable< int, 102 >(this);
    }

    int GetHeight() {
        return VTable< int, 103 >(this);
    }

    bool ProjectToScreenInternal(SWorldToScreen* proj) {
        return VTable< bool, 112 >(this, proj);//101 maybe
    }

    void GetWindowSize(int* width, int* height) {
        *width = this->GetWidth();
        *height = this->GetHeight();
    }

    IDXGISwapChain* GetSwapChain() {
        return *(IDXGISwapChain**)((uintptr_t)this + 0x14D000);
    }
    void SetSwapChain(IDXGISwapChain* pSwapChain) {
        *(IDXGISwapChain**)((uintptr_t)this + 0x14D000) = pSwapChain;
    }
};
struct IPhysicalWorld {
    enum entity_query_flagsentity_query_flags {
        ent_static = 0x1,
        ent_sleeping_rigid = 0x2,
        ent_rigid = 0x4,
        ent_living = 0x8,
        ent_independent = 0x10,
        ent_deleted = 0x80,
        ent_terrain = 0x100,
        ent_areas = 0x20,
        ent_triggers = 0x40,
        ent_ignore_noncolliding = 0x10000,
        ent_sort_by_mass = 0x20000,
        ent_allocate_list = 0x40000,
        ent_addref_results = 0x100000,
        ent_water = 0x200,
        ent_no_ondemand_activation = 0x80000,
        ent_delayed_deformations = 0x80000,

        pef_update = 0x800,

        ent_flagged_only = pef_update,
        ent_skip_flagged = pef_update * 2,
        ent_all = ent_static | ent_sleeping_rigid | ent_rigid | ent_living | ent_independent | ent_terrain,
    };
public:
    struct ray_hit
    {
        ray_hit() { RtlSecureZeroMemory(this, sizeof(*this)); }
        float dist;
        IPhysicalEntity* pCollider;
        int ipart;
        int partid;
        short surface_idx;
        short idmatOrg;
        int foreignIdx;
        int iNode;
        Vec3 pt;
        Vec3 n;
        int bTerrain;
        int iPrim;
        ray_hit* next;
    };
    struct ray_hit_cached : ray_hit
    {
        ray_hit_cached() { pCollider = 0; ipart = 0; }
        ray_hit_cached(const ray_hit& hit)
        {
            pCollider = hit.pCollider; ipart = hit.ipart;
            iNode = hit.iNode;
        }
        ray_hit_cached& operator=(const ray_hit& hit)
        {
            pCollider = hit.pCollider; ipart = hit.ipart;
            iNode = hit.iNode; return *this;
        }
    };
    int RayWorldIntersection(const Vec3& org, const Vec3& dir, int objtypes, unsigned int flags, ray_hit* hits, int nMaxHits, IPhysicalEntity** pSkipEnts = 0, int nSkipEnts = 0, void* pForeignData = 0, int iForeignData = 0, const char* pNameTag = "RayWorldIntersection(Game)", ray_hit_cached* phitLast = 0, int iCaller = 4)
    {
        return GetVFuncI<int(__fastcall*)(PVOID, const Vec3&, const Vec3&, int, unsigned int, ray_hit*, int, IPhysicalEntity**, int, void*, int, const char*, ray_hit_cached*, int)>(this, 97)(this, org, dir, objtypes, flags, hits, nMaxHits, pSkipEnts, nSkipEnts, pForeignData, iForeignData, pNameTag, phitLast, iCaller);
    }
    bool isVisible(IEntitySystem* pEntitySystem, IEntity* pEntity, Vec3 shootPos, Vec3 pos, IPhysicalEntity** pSkipEnts = 0, int nSkipEnts = 0, int pierceability = 10) {
        ray_hit rayhit;
        if (this->RayWorldIntersection(shootPos, pos - shootPos, ent_static | ent_terrain, 0xA0001500 | pierceability, &rayhit, 1, pSkipEnts, nSkipEnts) > 0) {
            return pEntitySystem->GetEntityFromPhysics(rayhit.pCollider) == pEntity;
        }
        return true;
    }

};
struct ISystem {

};
struct ILog {

};
struct IInput {

};
struct ITimer {

};
struct SSystemGlobalEnvironment
{
    char pad_0x0000[0x20]; //0x0000
    ILog* pLog; //0x0020
    char pad_0x0028[0x20]; //0x0028
    IRenderer* pRenderer; //0x0048
    char pad_0x0050[0x20]; //0x0050
    IPhysicalWorld* pPhysicalWorld; //0x0070
    IInput* pInput; //0x0078
    char pad_0x0080[0x40]; //0x0080
    ITimer* pTimer; //0x00C0
    char pad_0x00C8[0x8]; //0x00C8
    IGame* pGame; //0x00D0
    char pad_0x00D8[0x8]; //0x00D8
    IEntitySystem* pEntitySystem; //0x00E0
    char pad_0x00E8[0x8]; //0x00E8
    ISystem* pSystem; //0x00F0
};
#define gEnv (*(SSystemGlobalEnvironment**)0x142304198)//0x142304198 0x1422CF218
bool CustomW2S(Vec3 vPos, Vec3& vOut) {
    SWorldToScreen pWorldToScreen;

    pWorldToScreen.toX = vPos.x;
    pWorldToScreen.toY = vPos.y;
    pWorldToScreen.toZ = vPos.z;
    pWorldToScreen.outX = &vOut.x;
    pWorldToScreen.outY = &vOut.y;
    pWorldToScreen.outZ = &vOut.z;

    gEnv->pRenderer->ProjectToScreenInternal(&pWorldToScreen);

    vOut.x *= (gEnv->pRenderer->GetWidth() * 0.01f);
    vOut.y *= (gEnv->pRenderer->GetHeight() * 0.01f);
    vOut.z *= 1.0f;

    return ((vOut.z < 1.0f) && (vOut.x > 0) && (vOut.x < gEnv->pRenderer->GetWidth()) && (vOut.y > 0) && (vOut.y < gEnv->pRenderer->GetHeight()));
}