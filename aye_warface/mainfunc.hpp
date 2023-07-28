namespace globals {
    static bool g_initialized = false;
    static bool open = true;
    static bool esp = false;
    static bool espBox = false;
    static bool autoShot = false;
    static bool silent = false;
    int bone;
    static float silentFOV = 70;
}
bool GetTeam(IActor* pClientActor, IActor* pActor) {
    return (pClientActor->GetTeamId() != pActor->GetTeamId() || pClientActor->GetTeamId() == 0);
}
void GetMyestBonePosition(IEntity* pExp, Vec3& vBonePos, EBone bone) {
    if (!pExp->GetCharacter(0))
        return;
    ISkeletonPose* m_pSkeleton = pExp->GetCharacter(0)->GetISkeletonPose();
    vBonePos = pExp->GetBonePos(m_pSkeleton, bone);
}

void drawBox(int x, int y, int w, int h, ImU32 color, int thickness) {
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 1, 0, thickness);
}
float Distance(Vec3 VecA, Vec3 VecB)
{
    return sqrt(((VecA.x - VecB.x) * (VecA.x - VecB.x)) + ((VecA.y - VecB.y) * (VecA.y - VecB.y)) + ((VecA.z - VecB.z) * (VecA.z - VecB.z)));
};
Vec3 GetPlayerPos(IEntity* pEntit)
{
    Vec3 vOffset = Vec3();
    Matrix34 pWorld = pEntit->GetEntityWorldTM();
    vOffset = pWorld.GetTranslation();
    return vOffset;
}

float calculateFov(Vec3 to) {

    Vec3 out;

    if (!CustomW2S(to, out))
        return 9999.9f;

    float cx = static_cast<float>(gEnv->pRenderer->GetWidth() / 2);
    float cy = static_cast<float>(gEnv->pRenderer->GetHeight() / 2);

    float px = out.x > cx ? out.x - cx : cx - out.x;
    float py = out.y > cy ? out.y - cy : cy - out.y;

    return static_cast<float>(sqrt(px * px + py * py));
}
int GetBoneObject(ISkeletonPose* m_pSkeleton, const char* Name) {
    return m_pSkeleton->GetJointIdByName(Name);
}
bool IsEnumerationBone(const char* Name, IEntity* pExp, Vec3& vBonePos) {
    if (!pExp->GetCharacter(0))
        return false;

    ISkeletonPose* m_pSkeleton = pExp->GetCharacter(0)->GetISkeletonPose();

    __int32 nBoneId = GetBoneObject(m_pSkeleton, Name);
    if (!nBoneId) return false;

    vBonePos = pExp->GetBonePos(m_pSkeleton, (EBone)nBoneId);

    return true;
}

void entryPoint() {
    Vec3 obnylitt = ZERO;
    auto pPhisicalWorld = gEnv->pPhysicalWorld;
    if (!pPhisicalWorld) return;
    auto pEntitySystem = gEnv->pEntitySystem;
    if (!pEntitySystem) return;
    IGameFramework* pFramework = gEnv->pGame->GetIGameFramework();
    if (!pFramework) return;
    IActor* LocalActor = nullptr;
    if (pFramework->GetClientActorInternal(&LocalActor) == false) return;
    IItem* pItem = LocalActor->GetCurrentItem();
    if (!pItem) return;
    IWeapon* pWeapon = pItem->GetIWeapon();
    if (!pWeapon) return;
    

    if (IEntityIt* pEntityIt = gEnv->pEntitySystem->GetEntityIterator()) {
        while (IEntity* pEntity = pEntityIt->Next()) {
            auto pActor = pFramework->GetIActorSystem()->GetActor(pEntity->GetEntityId());
            if (pActor) {
                if (!GetTeam(LocalActor, pActor)) continue;
                if (pActor->IsDead()) continue;
                IEntityClass* pEntityClass = pEntity->GetEntityClass();
                if (!pEntityClass) continue;
                const char* pName = pEntity->GetName();
                const char* pClassName = pEntityClass->GetName();
                const char* pAnim = pActor->GetAnimationGraphState()->GetCurrentStateName();
                //autoshot
                if (globals::autoShot) {
                    if (IActor* LoocAtActor = pFramework->GetIActorSystem()->GetActor(LocalActor->GetGameObject()->Query()->LookAtEntity())) {
                        pWeapon->Start_Fire();
                        pWeapon->Stop_Fire();
                    }
                }

                

                static EBone target[3] =
                { Bone_Head, Bone_Neck, Bone_Spine };

                static const char* targetString[3] = {
                    "Bip01 Head", "Bip01 Neck", "Bip01 Spine"
                };

                Vec3 bynameHead = ZERO;
                IsEnumerationBone(targetString[globals::bone], pEntity, bynameHead);

                Vec3 OutSilent = ZERO;
                //Vec3 silentHead = ZERO;
                //GetMyestBonePosition(pEntity, silentHead, target[globals::bone]);
                if (!CustomW2S(bynameHead, OutSilent)) 
                    continue;//silentHead
                //silent
                if (globals::silent) {
                    ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(gEnv->pRenderer->GetWidth() / 2, gEnv->pRenderer->GetHeight() / 2), globals::silentFOV, IM_COL32(255, 0, 0, 255), 100, 1.0f);
                    if (calculateFov(bynameHead) <= globals::silentFOV) { //silentHead
                        ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(OutSilent.x, OutSilent.y), 10, IM_COL32(255, 0, 0, 255), 100);
                        ImGui::GetBackgroundDrawList()->AddText(ImVec2(OutSilent.x, OutSilent.y - 20), IM_COL32(255, 0, 0, 255), "TARGET");
                        pWeapon->SetFiringPos(bynameHead);//silentHead
                    }
                    else {
                        //bynameHead = { 0,0,0 };
                        pWeapon->SetFiringPos(obnylitt);
                    }
                }

                Vec3 Out = ZERO;
                Vec3 LPOut = ZERO;

                int distance = (int)Distance(GetPlayerPos(LocalActor->GetEntity()), GetPlayerPos(pEntity));
                int healthentity = pFramework->GetIActorSystem()->GetActor(pEntity->GetEntityId())->GetHealth();
                char buffer[64];
                snprintf(buffer, sizeof buffer, "[%d M] | [%d hp]", distance, healthentity);

                Vec3 HeadPosition = ZERO;
                GetMyestBonePosition(pEntity, HeadPosition, Bone_Head);
                Vec3 FootPositrion = ZERO;
                GetMyestBonePosition(pEntity, FootPositrion, Bone_L_Foot);

                Vec3 HeadOut = ZERO;
                Vec3 FootOut = ZERO;

                if (CustomW2S(GetPlayerPos(pEntity), Out) && CustomW2S(HeadPosition, HeadOut) && CustomW2S(FootPositrion, FootOut)) {
                    float Height = (HeadOut.y - FootOut.y);
                    float Width = Height / 2.f;

                    ImGui::GetBackgroundDrawList()->AddText(ImVec2(Out.x, Out.y - 10), ImColor(238.0f, 0.0f, 255.0f, 1.0f), buffer);
                    drawBox(FootOut.x - (Width / 2.0f), FootOut.y, Width, Height, ImColor(230.0f, 0.0f, 255.0f, 1.0f), 2);
                }
            }
        }
        pEntityIt->Release();
    }
}